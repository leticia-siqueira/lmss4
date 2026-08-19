#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]){
    if(argc == 1){
        printf("Processo interativo\n");
    }else if (argc == 2){
        printf("Processo workflow\n");
    }else{
        return -1;
    }
}