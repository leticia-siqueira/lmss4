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
        printf("Erro: Task sem programa.\n");
        return;
    }

    for(int i = 0; i < *quantidade_tarefas; i++){
    if(strcmp(cadastros[i].nome, argumentos[1]) == 0){
        printf("Erro: essa tarefa ja existe\n");
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

void run_task(char *argumentos[], int numero_argumentos, Task cadastros[], int quantidade_tarefas){
    if(numero_argumentos < 2){
        printf("Erro: argumentos insuficientes\n");
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
        printf("Erro: Tarefa nao existe.\n");
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
        printf("Erro: Falha na criacao do processo.\n");
        return;
    } else if(pid == 0){
        execvp(cadastros[indice].programa, argv_exec);

        printf("Erro: nao pode ser executado.\n");
        exit(1); 
    } else {
        int status;
        waitpid(pid, &status, 0);
    }
}

void sequential(char *argumentos[], int numero_argumentos, Task cadastros[], int quantidade_tarefas){
    
    if (numero_argumentos < 3){
        printf("Erro: Voce nao digitou nenhuma tarefa.\n");
        return;
    }

    for (int i = 2; i < numero_argumentos; i++){
        char *lista_comandos_sequential[2];
        lista_comandos_sequential[0] = "run";
        lista_comandos_sequential[1] = argumentos[i];

        run_task(lista_comandos_sequential, 2, cadastros, quantidade_tarefas);
    }
}

void parallel(char *argumentos[], int numero_argumentos, Task cadastros[], int quantidade_tarefas){
    pid_t pid_processos[50];
    int total_processos_paralelos = 0;

    for( int i = 2; i < numero_argumentos; i++){
        char *argv_exec[22]; 
        argv_exec[0] = cadastros[indice].programa;
        int j;
        for(j = 0; j < cadastros[indice].quantidade_arg; j++){
            argv_exec[j + 1] = cadastros[indice].argumento[j];
        }
        argv_exec[j + 1] = NULL; 

        pid_t pid = fork();

        if(pid < 0){
            printf("Erro: Falha na criacao do processo.\n");
            return;
        } else if(pid == 0){
            execvp(cadastros[indice].programa, argv_exec);

            printf("Erro: nao pode ser executado.\n");
            exit(1); 
        } else{
           pid_processos[total_processos_paralelos] = pid;
           total_processos_paralelos++; 
        }

    }

    for(int i = 0, i < total_processos_paralelos; i++){
        int status;
        waitpid(pid_processos[i], &status, 0);
    }
}

int ler_linha(char *linha, Task cadastros[], int *quantidade_tarefas){

    char *argumentos[MAX_argumentos];
    int numero_argumentos = 0;

    char *argumento = strtok(linha, " ");
    while(argumento != NULL && numero_argumentos < MAX_argumentos){
        argumentos[numero_argumentos] = argumento;
        numero_argumentos++;
        argumento = strtok(NULL, " ");
    }

    if(numero_argumentos == 0){
        return 0; 
    }

    if(strcmp(argumentos[0], "exit") == 0){
        return 1; 
    }

    if(strcmp(argumentos[0], "task") == 0){
        cadastro_de_task(argumentos, numero_argumentos, cadastros, quantidade_tarefas);

    } else if(strcmp(argumentos[0], "run") == 0){
        if( numero_argumentos < 2){
            printf("Erro: Numero de argumentos invalidos\n");
        
            } else if (strcmp(argumentos[1], "sequential") == 0){
                sequential(argumentos, numero_argumentos, cadastros, *quantidade_tarefas);

            } else if (strcmp(argumentos[1], "parallel") == 0){
                run_parallel();

            } else if(strcmp(argumentos[1], "pipe") == 0){
                run_pipe();

            } else{
                run_task(argumentos, numero_argumentos, cadastros, *quantidade_tarefas);

            }

    } else if(strcmp(argumentos[0], "workdir") == 0){
        

    } else {
        printf("Erro: comando desconhecido\n");
    }

    return 0; 
}

int main(int argc, char *argv[]){

    int quantidade_tarefas = 0;
    Task cadastros[100];

    if(argc == 1){
        printf("Processo interativo\n"); 

        char comando_digitado[1000];

        while(1){
            printf("processflow> ");

            if(fgets(comando_digitado, sizeof(comando_digitado), stdin) == NULL){
                break;
            }

            char *final_da_linha = strchr(comando_digitado, '\n');
            if(final_da_linha != NULL){
                *final_da_linha = '\0';
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
                if( numero_argumentos < 2){
                    printf("Erro: Numero de argumentos invalidos\n");
        
                } else if (strcmp(argumentos[1], "sequential") == 0){
                    sequential(argumentos, numero_argumentos, cadastros, quantidade_tarefas);

                } else if (strcmp(argumentos[1], "parallel") == 0){
                    //parallel();

                } else if(strcmp(argumentos[1], "pipe") == 0){
                    //pipe();

                } else{
                    run_task(argumentos, numero_argumentos, cadastros, quantidade_tarefas);

                }

            } else if(strcmp(argumentos[0], "workdir") == 0){
            

            } else {
                printf("Erro: processo desconhecido\n");
            }   
        }

    }else if (argc == 2){
        printf("Processo workflow\n");
        
        FILE *arquivo = fopen(argv[1], "r");
        if(arquivo == NULL){
            printf("Erro! Nao foi possível abrir o arquivo\n");
            return -1;
        }

        char linha[1000];
        while(fgets(linha, sizeof(linha), arquivo) != NULL){
            char *final_da_linha = strchr(linha, '\n');
            if(final_da_linha != NULL){
                *final_da_linha = '\0';
            }

            printf("%s\n", linha); 

            int acabou_linha = ler_linha(linha, cadastros, &quantidade_tarefas);

            if(acabou_linha == 1){
                break;
            }
        }

        fclose(arquivo);
        } else {
            return -1;
        }
    return 0;
}