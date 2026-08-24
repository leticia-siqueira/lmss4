#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>

#define MAX_argumentos 100

typedef struct Tarefas{
    char nome[100];
    char programa[100];
    char argumento[100][100];
    int quantidade_de_argumentos;

    char input_do_arquivo[100];
    char output_doarquivo[100];
    int estado_do_output;
}Tarefas;

typedef struct Job{
    int id_do_job;
    pid_t pid;
    char nome_da_tarefa[100];
    int job_finalizado; 
}Job;

void cadastro_de_tarefa(char *argumentos[], int contador_de_argumentos, Tarefas cadastradas[], int *quantidade_de_tarefas){
    
    if (contador_de_argumentos < 3){
        printf("Tarefa sem programa.\n");
        return;
    }

    for(int i = 0; i < *quantidade_de_tarefas; i++){
    if(strcmp(cadastradas[i].nome, argumentos[1]) == 0){
        printf("Essa tarefa ja existe\n");
        return;
        }
    }

    strncpy(cadastradas[*quantidade_de_tarefas].nome, argumentos[1], 99);
    cadastradas[*quantidade_de_tarefas].nome[99] = '\0';

    strncpy(cadastradas[*quantidade_de_tarefas].programa, argumentos[2], 99);
    cadastradas[*quantidade_de_tarefas].programa[99] = '\0';

    for (int i = 3; i < contador_de_argumentos; i++){
        strncpy(cadastradas[*quantidade_de_tarefas].argumento[i - 3], argumentos[i], 99);
        cadastradas[*quantidade_de_tarefas].argumento[i - 3][99] = '\0';
    }
    
    cadastradas[*quantidade_de_tarefas].quantidade_de_argumentos = contador_de_argumentos - 3;

    cadastradas[*quantidade_de_tarefas].input_do_arquivo[0] = '\0';
    cadastradas[*quantidade_de_tarefas].output_doarquivo[0] = '\0';
    cadastradas[*quantidade_de_tarefas].estado_do_output = 0;

    (*quantidade_de_tarefas)++;

}

void funcao_run_normal(char *argumentos[], int contador_de_argumentos, Tarefas cadastradas[], int quantidade_de_tarefas){
    if(contador_de_argumentos < 2){
        printf("Argumentos insuficientes\n");
        return;
    }

    int indice = -1;
    for(int i = 0; i < quantidade_de_tarefas; i++){
        if(strcmp(cadastradas[i].nome, argumentos[1]) == 0){
            indice = i;
            break;
        }
    }

    if(indice == -1){
        printf("essa tarefas nao existe\n");
        return;
    }

    char *argv_exec[50]; 
    argv_exec[0] = cadastradas[indice].programa;

    int i;
    for(i = 0; i < cadastradas[indice].quantidade_de_argumentos; i++){
        argv_exec[i + 1] = cadastradas[indice].argumento[i];
    }
    argv_exec[i + 1] = NULL; 

    pid_t pid = fork();

    if(pid < 0){
        printf("Falha ao criar o processo\n");
        return;

    } else if(pid == 0){
        if(cadastradas[indice].input_do_arquivo[0] != '\0'){
        
        int input_do_arquivo = open(cadastradas[indice].input_do_arquivo, O_RDONLY);
        
        if(input_do_arquivo < 0){
            printf("nao daa pra abrir\n");
            exit(1);
        }
        dup2(input_do_arquivo, 0);
        close(input_do_arquivo);
        }

        if(cadastradas[indice].estado_do_output == 1){
            int output_do_arquivo = open(cadastradas[indice].output_doarquivo, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if(output_do_arquivo < 0){
                printf("nao da pra abrir\n");
                exit(1);
            }
            dup2(output_do_arquivo, 1);
            close(output_do_arquivo);

        } else if(cadastradas[indice].estado_do_output == 2){
            int output_do_arquivo = open(cadastradas[indice].output_doarquivo, O_WRONLY | O_CREAT | O_APPEND, 0644);
            if(output_do_arquivo < 0){
                printf("nao da para abrir\n");
                exit(1);
            }
            dup2(output_do_arquivo, 1);
            close(output_do_arquivo);
        }
        execvp(cadastradas[indice].programa, argv_exec);

        printf("Erro: nao pode ser executado.\n");
        exit(1); 
    } else {
        int estado;
        waitpid(pid, &estado, 0);
    }
}

void funcao_sequential(char *argumentos[], int contador_de_argumentos, Tarefas cadastradas[], int quantidade_de_tarefas){
    
    if (contador_de_argumentos < 3){
        printf("Voce nao digitou nenhuma tarefa.\n");
        return;
    }

    for (int i = 2; i < contador_de_argumentos; i++){
        char *lista_comandos_sequential[2];
        lista_comandos_sequential[0] = "run";
        lista_comandos_sequential[1] = argumentos[i];

        funcao_run_normal(lista_comandos_sequential, 2, cadastradas, quantidade_de_tarefas);
    }
}

void funcao_parallel(char *argumentos[], int contador_de_argumentos, Tarefas cadastradas[], int quantidade_de_tarefas){
    pid_t pid_dos_processos[50];
    int total_processos_paralelos = 0;

    for(int i = 2; i < contador_de_argumentos; i++){

        int indice = -1;
        for(int k = 0; k < quantidade_de_tarefas; k++){
            if(strcmp(cadastradas[k].nome, argumentos[i]) == 0){
                indice = k;
                break;
            }
        }

        if(indice == -1){
            printf("essa tarefa nao existe.\n");
            continue;
        }

        char *argv_exec[50]; 
        argv_exec[0] = cadastradas[indice].programa;
        int j;
        for(j = 0; j < cadastradas[indice].quantidade_de_argumentos; j++){
            argv_exec[j + 1] = cadastradas[indice].argumento[j];
        }
        argv_exec[j + 1] = NULL;

        pid_t pid = fork();

        if(pid < 0){
            printf("Falha ao criar o processo.\n");
            return;

        } else if(pid == 0){
            if(cadastradas[indice].input_do_arquivo[0] != '\0'){
        
            int input_do_arquivo = open(cadastradas[indice].input_do_arquivo, O_RDONLY);
        
            if(input_do_arquivo < 0){
                printf("nao da para abrir\n");
                exit(1);
            }
            dup2(input_do_arquivo, 0);
            close(input_do_arquivo);
            }

            if(cadastradas[indice].estado_do_output == 1){
                int output_do_arquivo = open(cadastradas[indice].output_doarquivo, O_WRONLY | O_CREAT | O_TRUNC, 0644);
                if(output_do_arquivo < 0){
                    printf("nao da pra abrir\n");
                    exit(1);
                }
                dup2(output_do_arquivo, 1);
                close(output_do_arquivo);

            } else if(cadastradas[indice].estado_do_output == 2){
                int output_do_arquivo = open(cadastradas[indice].output_doarquivo, O_WRONLY | O_CREAT | O_APPEND, 0644);
                if(output_do_arquivo < 0){
                    printf("nao da pra abrir\n");
                    exit(1);
                }
                dup2(output_do_arquivo, 1);
                close(output_do_arquivo);
            }
            execvp(cadastradas[indice].programa, argv_exec);

            printf("nao deu para ser executado.\n");
            exit(1); 
        } else{
            pid_dos_processos[total_processos_paralelos] = pid;
            total_processos_paralelos++; 
        }
    }

    for(int i = 0; i < total_processos_paralelos; i++){
        int estado;
        waitpid(pid_dos_processos[i], &estado, 0);
    }
}

void funcao_pipe(char *argumentos[], int contador_de_argumentos, Tarefas cadastradas[], int quantidade_de_tarefas){

    if(contador_de_argumentos < 4){
        printf("Coloque pelo menos 2 tarefas para usar o pipe.\n");
        return;
    }

    int tarefas_no_total = contador_de_argumentos - 2; 
    int quantidade_de_pipe = tarefas_no_total - 1;

    int pipes_inseridos[20][2]; 

    for(int a = 0; a < quantidade_de_pipe; a++){
        if(pipe(pipes_inseridos[a]) == -1){
            printf("nao deu para criar pipe.\n");
            return;
        }
    }

    pid_t pid_dos_processos[50];
    int total_de_processos = 0;

    for(int i = 2; i < contador_de_argumentos; i++){

        int ordem_da_tarefa = i - 2; 

        int indice = -1;
        for(int k = 0; k < quantidade_de_tarefas; k++){
            if(strcmp(cadastradas[k].nome, argumentos[i]) == 0){
                indice = k;
                break;
            }
        }

        if(indice == -1){
            printf("nao existe essa tarefa.\n");
            continue;
        }

        char *argv_exec[50];
        argv_exec[0] = cadastradas[indice].programa;
        
        int j;
        for(j = 0; j < cadastradas[indice].quantidade_de_argumentos; j++){
            argv_exec[j + 1] = cadastradas[indice].argumento[j];
        }
        argv_exec[j + 1] = NULL;

        pid_t pid = fork();

        if(pid < 0){
            printf("nao foi possivel criar processo.\n");
            return;

        } else if(pid == 0){

            if(ordem_da_tarefa > 0){
                dup2(pipes_inseridos[ordem_da_tarefa - 1][0], 0);
            }

            if(ordem_da_tarefa < quantidade_de_pipe){
                dup2(pipes_inseridos[ordem_da_tarefa][1], 1);
            }

            for(int a = 0; a < quantidade_de_pipe; a++){
                close(pipes_inseridos[a][0]);
                close(pipes_inseridos[a][1]);
            }

            execvp(cadastradas[indice].programa, argv_exec);

            printf("nao pode ser executado.\n");
            exit(1);

        } else {
            pid_dos_processos[total_de_processos] = pid;
            total_de_processos++;
        }
    }

    for(int a = 0; a < quantidade_de_pipe; a++){
        close(pipes_inseridos[a][0]);
        close(pipes_inseridos[a][1]);
    }

    for(int i = 0; i < total_de_processos; i++){
        int estado;
        waitpid(pid_dos_processos[i], &estado, 0);
    }
}

void funcao_input(char *argumentos[], int contador_de_argumentos, Tarefas cadastradas[], int quantidade_de_tarefas){

    if(contador_de_argumentos < 3){
        printf("argumentos insuficiente\n");
        return;
    }

    int indice = -1;
    for(int i = 0; i < quantidade_de_tarefas; i++){
        if(strcmp(cadastradas[i].nome, argumentos[1]) == 0){
            indice = i;
            break;
        }
    }

    if(indice == -1){
        printf("tarefa nao existe.\n");
        return;
    }

    strncpy(cadastradas[indice].input_do_arquivo, argumentos[2], 99);
    cadastradas[indice].input_do_arquivo[99] = '\0';
}

void funcao_output(char *argumentos[], int contador_de_argumentos, Tarefas cadastradas[], int quantidade_de_tarefas){

    if(contador_de_argumentos < 3){
        printf("argumento insuficientes\n");
        return;
    }

    int indice = -1;
    for(int i = 0; i < quantidade_de_tarefas; i++){
        if(strcmp(cadastradas[i].nome, argumentos[1]) == 0){
            indice = i;
            break;
        }
    }

    if(indice == -1){
        printf("essa tarefa nao existe.\n");
        return;
    }

    strncpy(cadastradas[indice].output_doarquivo, argumentos[2], 99);
    cadastradas[indice].output_doarquivo[99] = '\0';
    cadastradas[indice].estado_do_output = 1;
}

void funcao_append(char *argumentos[], int contador_de_argumentos, Tarefas cadastradas[], int quantidade_de_tarefas){

    if(contador_de_argumentos < 3){
        printf("poucos argumentos\n");
        return;
    }

    int indice = -1;
    for(int i = 0; i < quantidade_de_tarefas; i++){
        if(strcmp(cadastradas[i].nome, argumentos[1]) == 0){
            indice = i;
            break;
        }
    }

    if(indice == -1){
        printf("essa tarefa nao existe.\n");
        return;
    }

    strncpy(cadastradas[indice].output_doarquivo, argumentos[2], 99);
    cadastradas[indice].output_doarquivo[99] = '\0';
    cadastradas[indice].estado_do_output = 2;
}

void funcao_workdir(char *argumentos[], int contador_de_argumentos){

    if(contador_de_argumentos < 2){
        printf("poucos argumentos\n");
        return;
    }

    if(chdir(argumentos[1]) != 0){
        printf("esse diretorio nao existe.\n");
    }
}

void funcao_start(char *argumentos[], int contador_de_argumentos, Tarefas cadastradas[], int quantidade_de_tarefas, Job lista_de_job[], int *job_totais){

    if(contador_de_argumentos < 2){
        printf("Poucos argumentos\n");
        return;
    }

    int indice = -1;
    for(int i = 0; i < quantidade_de_tarefas; i++){
        if(strcmp(cadastradas[i].nome, argumentos[1]) == 0){
            indice = i;
            break;
        }
    }

    if(indice == -1){
        printf("essa tarefa nao existe.\n");
        return;
    }

    char *argv_exec[50];
    argv_exec[0] = cadastradas[indice].programa;
    int i;
    for(i = 0; i < cadastradas[indice].quantidade_de_argumentos; i++){
        argv_exec[i + 1] = cadastradas[indice].argumento[i];
    }
    argv_exec[i + 1] = NULL;

    pid_t pid = fork();

    if(pid < 0){
        printf("Processo falhou ao criar.\n");
        return;

    } else if(pid == 0){
        if(cadastradas[indice].input_do_arquivo[0] != '\0'){
            int input_do_arquivo = open(cadastradas[indice].input_do_arquivo, O_RDONLY);
            if(input_do_arquivo < 0){
                printf("nao deu para abrir\n");
                exit(1);
            }
            dup2(input_do_arquivo, 0);
            close(input_do_arquivo);
        }

        if(cadastradas[indice].estado_do_output == 1){
            int output_do_arquivo = open(cadastradas[indice].output_doarquivo, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if(output_do_arquivo < 0){
                printf("nao deu para abrir\n");
                exit(1);
            }
            dup2(output_do_arquivo, 1);
            close(output_do_arquivo);

        } else if(cadastradas[indice].estado_do_output == 2){
            int output_do_arquivo = open(cadastradas[indice].output_doarquivo, O_WRONLY | O_CREAT | O_APPEND, 0644);
            if(output_do_arquivo < 0){
                printf("nao deu para abrir\n");
                exit(1);
            }
            dup2(output_do_arquivo, 1);
            close(output_do_arquivo);
        }

        execvp(cadastradas[indice].programa, argv_exec);

        printf("nao foi possivel executar.\n");
        exit(1);

    } else {

        int id_novo = *job_totais + 1;

        lista_de_job[*job_totais].id_do_job = id_novo;
        lista_de_job[*job_totais].pid = pid;
        strncpy(lista_de_job[*job_totais].nome_da_tarefa, argumentos[1], 99);
        lista_de_job[*job_totais].nome_da_tarefa[99] = '\0';
        lista_de_job[*job_totais].job_finalizado = 0;

        (*job_totais)++;

        printf("[%d] %d\n", id_novo, pid);
    }
}

void funcao_do_job(Job lista_de_job[], int job_totais){

    if(job_totais == 0){
        printf("Nada de job por aqui.\n");
        return;
    }

    for(int i = 0; i < job_totais; i++){

        if(lista_de_job[i].job_finalizado == 0){
            int status;
            pid_t resultado = waitpid(lista_de_job[i].pid, &status, WNOHANG);

            if(resultado == lista_de_job[i].pid){
                lista_de_job[i].job_finalizado = 1;
            }
        }

        if(lista_de_job[i].job_finalizado == 1){
            printf("Job foi Finalizado\n");
        } else {
            printf("Job ainda está em processo\n");
        }
    }
}

void funcao_esperar_job(char *argumentos[], int numero_argumentos, Job lista_de_job[], int job_totais){

    if(numero_argumentos < 2){
        printf("Poucos argumentos\n");
        return;
    }

    int procurar_id_job = atoi(argumentos[1]);

    int indice = -1;
    for(int i = 0; i < job_totais; i++){
        if(lista_de_job[i].id_do_job == procurar_id_job){
            indice = i;
            break;
        }
    }

    if(indice == -1){
        printf("nao existe\n");
        return;
    }

    if(lista_de_job[indice].job_finalizado == 1){
        printf("Foi job_finalizado.\n");
        return;
    }

    int status;
    waitpid(lista_de_job[indice].pid, &status, 0);
    lista_de_job[indice].job_finalizado = 1;

    printf("job_finalizado\n");
}

int ler_linha(char *linha, Tarefas cadastradas[], int *quantidade_de_tarefas, Job lista_de_job[], int *job_totais){

    char *argumentos[MAX_argumentos];
    int contador_de_argumentos = 0;

    char *argumento = strtok(linha, " ");

    while(argumento != NULL && contador_de_argumentos < MAX_argumentos){
        
        argumentos[contador_de_argumentos] = argumento;
        contador_de_argumentos++;
        argumento = strtok(NULL, " ");
    }

    if(contador_de_argumentos == 0){
        return 0; 
    }

    if(strcmp(argumentos[0], "exit") == 0){
        return 1; 
    }

    if(strcmp(argumentos[0], "task") == 0){
        cadastro_de_tarefa(argumentos, contador_de_argumentos, cadastradas, quantidade_de_tarefas);

    } else if(strcmp(argumentos[0], "run") == 0){
        
        if( contador_de_argumentos < 2){
            printf("quantidade de argumentos invalidos\n");
        
            } else if (strcmp(argumentos[1], "sequential") == 0){
                funcao_sequential(argumentos, contador_de_argumentos, cadastradas, *quantidade_de_tarefas);

            } else if (strcmp(argumentos[1], "parallel") == 0){
                funcao_parallel(argumentos, contador_de_argumentos, cadastradas, *quantidade_de_tarefas);

            } else if(strcmp(argumentos[1], "pipe") == 0){
                funcao_pipe(argumentos, contador_de_argumentos, cadastradas, *quantidade_de_tarefas);

            } else{
                funcao_run_normal(argumentos, contador_de_argumentos, cadastradas, *quantidade_de_tarefas);

            }
    
    } else if(strcmp(argumentos[0], "output") == 0){
        funcao_output(argumentos, contador_de_argumentos, cadastradas, *quantidade_de_tarefas);

    } else if(strcmp(argumentos[0], "append") == 0){
        funcao_append(argumentos, contador_de_argumentos, cadastradas, *quantidade_de_tarefas);

    } else if(strcmp(argumentos[0], "workdir") == 0){
        funcao_workdir(argumentos, contador_de_argumentos); 

    } else if(strcmp(argumentos[0], "input") == 0){
        funcao_input(argumentos, contador_de_argumentos, cadastradas, *quantidade_de_tarefas);

    }else if(strcmp(argumentos[0], "start") == 0){
        funcao_start(argumentos, contador_de_argumentos, cadastradas, *quantidade_de_tarefas, lista_de_job, job_totais);

    } else if(strcmp(argumentos[0], "jobs") == 0){
        funcao_do_job(lista_de_job, *job_totais);

    } else if(strcmp(argumentos[0], "wait") == 0){
        funcao_esperar_job(argumentos, contador_de_argumentos, lista_de_job, *job_totais);

    }else{
        printf("Comando desconhecido\n");
    }

    return 0; 
}

int main(int argc, char *argv[]){

    int quantidade_de_tarefas = 0;
    Tarefas cadastradas[100];

    Job lista_de_job[50];
    int job_totais = 0;

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
            int contador_de_argumentos = 0;

            char *argumento = strtok(comando_digitado, " ");

            while(argumento != NULL && contador_de_argumentos < MAX_argumentos){
                argumentos[contador_de_argumentos] = argumento;
                contador_de_argumentos++;
                argumento = strtok(NULL, " ");
            }

            if(contador_de_argumentos == 0){
                continue;
            }

            if ((strcmp(argumentos[0], "exit")) == 0){
                break;
            }

            if(strcmp(argumentos[0], "task") == 0){
                cadastro_de_tarefa(argumentos, contador_de_argumentos, cadastradas, &quantidade_de_tarefas);

            } else if(strcmp(argumentos[0], "run") == 0){
                
                if( contador_de_argumentos < 2){
                    printf("quantidade de argumentos invalida\n");
        
                } else if (strcmp(argumentos[1], "sequential") == 0){
                    funcao_sequential(argumentos, contador_de_argumentos, cadastradas, quantidade_de_tarefas);

                } else if (strcmp(argumentos[1], "parallel") == 0){
                    funcao_parallel(argumentos, contador_de_argumentos, cadastradas, quantidade_de_tarefas);

                } else if(strcmp(argumentos[1], "pipe") == 0){
                    funcao_pipe(argumentos, contador_de_argumentos, cadastradas, quantidade_de_tarefas);

                } else{
                    funcao_run_normal(argumentos, contador_de_argumentos, cadastradas, quantidade_de_tarefas);

                }

            } else if(strcmp(argumentos[0], "workdir") == 0){
                funcao_workdir(argumentos, contador_de_argumentos);

            }else if(strcmp(argumentos[0], "input") == 0){
                funcao_input(argumentos, contador_de_argumentos, cadastradas, quantidade_de_tarefas);

            } else if(strcmp(argumentos[0], "output") == 0){
                funcao_output(argumentos, contador_de_argumentos, cadastradas, quantidade_de_tarefas);

            }else if(strcmp(argumentos[0], "append") == 0){
                funcao_append(argumentos, contador_de_argumentos, cadastradas, quantidade_de_tarefas);

            } else if(strcmp(argumentos[0], "start") == 0){
                funcao_start(argumentos, contador_de_argumentos, cadastradas, quantidade_de_tarefas, lista_de_job, &job_totais);

            } else if(strcmp(argumentos[0], "jobs") == 0){
                funcao_do_job(lista_de_job, job_totais);

            } else if(strcmp(argumentos[0], "wait") == 0){
                funcao_esperar_job(argumentos, contador_de_argumentos, lista_de_job, job_totais);

            }else{
                printf("Tarefa desconhecida\n");
            }   
        }

    }else if (argc == 2){
        printf("Processo workflow\n");
        
        FILE *arquivo = fopen(argv[1], "r");
        if(arquivo == NULL){
            printf("Nao foi possível abrir o arquivo\n");
            return -1;
        }

        char linha[1000];
        
        while(fgets(linha, sizeof(linha), arquivo) != NULL){
            char *final_da_linha = strchr(linha, '\n');
            
            if(final_da_linha != NULL){
                *final_da_linha = '\0';
            }

            printf("%s\n", linha); 

            int acabou_a_linha = ler_linha(linha, cadastradas, &quantidade_de_tarefas, lista_de_job, &job_totais);

            if(acabou_a_linha == 1){
                break;
            }
        }

        fclose(arquivo);
        
    } else {
        return -1;
    }
    return 0;
}