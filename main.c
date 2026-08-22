#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#define MAX_argumentos 20

typedef struct Task{
    char nome[100];
    char programa[100];
    char argumento[100][100];
    int quantidade_arg;


}Task;

void cadastro_de_task(char *argumentos[], int numero_argumentos, Task cadastros[], int *quantidade_tarefas){
    
    if (numero_argumentos < 3){
        printf("Erro! Task sem programa.\n");
        return;
    }

    for(int i = 0; i < *quantidade_tarefas; i++){
    if(strcmp(cadastros[i].nome, argumentos[1]) == 0){
        printf("Erro! Já existe a tarefa '%s'.\n", argumentos[1]);
        return;
        }
    }

    strncpy(cadastros[*quantidade_tarefas].nome, argumentos[1], 99);
    cadastros[*quantidade_tarefas].nome[99] = '\0';

    strncpy(cadastros[*quantidade_tarefas].programa, argumentos[2], 99);
    cadastros[*quantidade_tarefas].programa[99] = '\0';

    for (int i = 3; i < numero_argumentos; i++){
        strncpy(cadastros[*quantidade_tarefas].argumento[i - 3], argumentos[i], 99);
        cadastros[*quantidade_tarefas].argumento[i - 3][99] = '\0';
    }
    cadastros[*quantidade_tarefas].quantidade_arg = numero_argumentos - 3;

    (*quantidade_tarefas)++;

}

void run_task(char *argumentos[], int numero_argumentos, Tarefa cadastros[], int quantidade_tarefas){
    if(numero_argumentos < 2){
        printf("Erro! Uso: run <nome>\n");
        return;
    }

    int indice = -1;
    for(int i = 0; i < quantidade_tarefas; i++){
        if(strcmp(cadastros[i].nome, argumentos[1]) == 0){
            indice = i;
            break;
        }
    }

    if(indice == -1){
        printf("Erro! Tarefa '%s' não existe.\n", argumentos[1]);
        return;
    }

    char *argv_exec[22]; 
    argv_exec[0] = cadastros[indice].programa;
    int i;
    for(i = 0; i < cadastros[indice].quantidade_arg; i++){
        argv_exec[i + 1] = cadastros[indice].argumento[i];
    }
    argv_exec[i + 1] = NULL; 

    pid_t pid = fork();

    if(pid < 0){
        printf("Erro! Falha ao criar processo.\n");
        return;
    } else if(pid == 0){
        execvp(cadastros[indice].programa, argv_exec);

        printf("Erro! '%s' nao pode ser executado.\n", cadastros[indice].programa);
        exit(1); 
    } else {
        int status;
        waitpid(pid, &status, 0);
    }
}

int main(int argc, char *argv[]){

    int quantidade_tarefas = 0;
    Task cadastros[100];

    if(argc == 1){
        printf("Processo interativo\n");

        char comando_digitado[1000];

        while(1){
            printf("processflow> ");
            fgets(comando_digitado, sizeof(comando_digitado), stdin);
            char *encerrar = strchr(comando_digitado, '\n');
            if(encerrar != NULL){
                *encerrar = '\0';
            }

            char *argumentos[MAX_argumentos];
            int numero_argumentos = 0;

            char *argumento = strtok(comando_digitado, " ");
            while(argumento != NULL && numero_argumentos < MAX_argumentos){
                argumentos[numero_argumentos] = argumento;
                numero_argumentos++;
                argumento = strtok(NULL, " ");
            }

            if(numero_argumentos == 0){
                continue;
            }

            if ((strcmp(argumentos[0], "exit")) == 0){
                break;
            }

            if(strcmp(argumentos[0], "task") == 0){
                cadastro_de_task(argumentos, numero_argumentos, cadastros, &quantidade_tarefas);

            } else if(strcmp(argumentos[0], "run") == 0){
                run_task(argumentos, numero_argumentos, cadastros, quantidade_tarefas);

            } else if(strcmp(argumentos[0], "workdir") == 0){
            

            } else {
                printf("Erro! Comando '%s' desconhecido\n", argumentos[0]);
}
        }

    }else if (argc == 2){
        printf("Processo workflow\n");
    }else{
        return -1;
    }

    return 0;
}