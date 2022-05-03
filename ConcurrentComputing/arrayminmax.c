//Soma todos os elementos de um vetor de inteiro
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "timer.h"

long long int dim; //dimensao do vetor de entrada
int nthreads; //numero de threads
float *vetor; //vetor de entrada com dimensao dim 

typedef struct minmax {
    float min;
    float max;
} minmax;

//fluxo das threads
void * tarefa(void * arg) {

    long long int id = (long long int) arg; //identificador da thread
    minmax min_max; //variáveis locais do menor e maior elemento do bloco

    minmax min_max = (minmax*) malloc(sizeof(minmax));
    if (min_max == NULL) {
        exit(1);
    }

    long long int tamBloco = dim/nthreads;  //tamanho do bloco de cada thread 
    long long int ini = id * tamBloco; //elemento inicial do bloco da thread
    long long int fim; //elemento final (nao processado) do bloco da thread
    if (id == nthreads-1) {
        fim = dim;
    }
    else {
        fim = ini + tamBloco;
    } // Trata o resto se houver

    // Acha o menor e o maior elemento do bloco da thread
    for (long int i = ini; i < fim; i++) {
        if (i == ini) {
            min_max->min = vetor[i];
            min_max->max = vetor[i];
        }
        else {
            if (vetor[i] < min){
                min_max->min = vetor[i];
            }
            if (vetor[i] > max) {
                min_max->max = vetor[i];
            }
        }
    }

    //retorna o resultado da soma local
    pthread_exit((void *) min_max);
}

//fluxo principal
int main(int argc, char *argv[]) {
    float somaSeq = 0; //soma sequencial
    float somaConc = 0; //soma concorrente
    float ini, fim; //tomada de tempo
    pthread_t *tid; //identificadores das threads no sistema
    float *retorno; //valor de retorno das threads

    //recebe e valida os parametros de entrada (dimensao do vetor, numero de threads)
    if(argc < 3) {
        fprintf(stderr, "Digite: %s <dimensao do vetor> <numero threads>\n", argv[0]);
        return 1; 
    }
    dim = atoll(argv[1]);
    nthreads = atoi(argv[2]);

    //aloca o vetor de entrada
    vetor = (float*) malloc(sizeof(float)*dim);
    if (vetor == NULL) {
        fprintf(stderr, "ERRO--malloc\n");
        return 2;
    }

    //preenche o vetor de entrada
    srand(time(NULL));
    for (long long int i = 0; i < dim; i++) {
        vetor[i] = rand();
    }

    //soma sequencial dos elementos
    GET_TIME(ini);
    for(long long int i=0; i<dim; i++) {
        somaSeq += vetor[i];
    }
    GET_TIME(fim);
    printf("Tempo sequencial:  %lf\n", fim - ini);

    //soma concorrente dos elementos
    GET_TIME(ini);
    tid = (pthread_t *) malloc(sizeof(pthread_t) * nthreads);
    if (tid == NULL) {
        fprintf(stderr, "ERRO--malloc\n");
        return 2;
    }

    //criar as threads
    for(long long int i = 0; i<nthreads; i++) {
        if(pthread_create(tid+i, NULL, tarefa, (void*) i)){
            fprintf(stderr, "ERRO--pthread_create\n");
            return 3;
        }
    }

    long long int min;
    long long int max;
    //aguardar o termino das threads
    for(long long int i = 0; i<nthreads; i++) {
        if(pthread_join(*(tid+i), (void**) &retorno)){
            fprintf(stderr, "ERRO--pthread_create\n");
            return 3;
        }
        //análise de min-max global
        if (i == 0) {
            min = retorno->min;
            max = retorno->max;
        }
        else {
            if (vetor[i] < min){
                min = retorno->min;
            }
            if (vetor[i] > max) {
                max = retorno->max;
            }
        }
        free(retorno);
    }
    GET_TIME(fim);
    printf("Tempo concorrente:  %lf\n", fim-ini);

    //exibir os resultados
    printf("Minimo:  %.12lf\n", min);
    printf("Maximo: %.12lf\n", max);

    //libera as areas de memoria alocadas
    free(vetor);
    free(tid);
    min

    return 0;
}
