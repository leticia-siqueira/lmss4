#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

#define MAX_argumentos 20

typedef struct Tarefa{
    char nome[50];
    char programa[50];
    char argumento[20][50];
    int quantidade_arg;
}Tarefa;

void cadastro(char *argumentos[], int numero_argumentos, Tarefa cadastros[], int *quantidade_tarefas){

}

int main(int argc, char *argv[]){

    int quantidade_tarefas = 0;
    Tarefa cadastros[50];

    if(argc == 1){
        printf("Processo interativo\n");

        char comando_digitado[99];

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

            for(int i = 0; i < numero_argumentos; i++){
                printf("[%s] ", argumentos[i]);
            }
            printf("\n");

            if ((strcmp(argumentos[0], "exit")) == 0){
                break;
            }

            if(strcmp(argumentos[0], "exit") == 0){
                break;

            } else if(strcmp(argumentos[0], "task") == 0){
                cadastrar_task(comandos, num_comandos, cadastros, &quantidade_tarefas);

            } else if(strcmp(argumentos[0], "run") == 0){
                

            } else if(strcmp(argumentos[0], "workdir") == 0){
            

            } else {
                printf("Erro: comando desconhecido '%s'\n", argumentos[0]);
}
        }

    }else if (argc == 2){
        printf("Processo workflow\n");
    }else{
        return -1;
    }

    return 0;
}