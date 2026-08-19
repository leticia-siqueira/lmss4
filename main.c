#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Tarefa{
    char nome[50];
    char programa[50];
    char argumento[20][50];
    int quantidade_arg;
}Tarefa;

int main(int argc, char *argv[]){

    int quantidade_tarefas = 0;

    Tarefa cadastros[10];

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
            printf("Você digitou: [%s]\n", comando_digitado);

            if ((strcmp(comando_digitado, "exit")) == 0){
                break;
            }
        }

    }else if (argc == 2){
        printf("Processo workflow\n");
    }else{
        return -1;
    }

}