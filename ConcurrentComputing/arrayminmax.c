// Calcula o mínimo e o máximo de um vetor de float de muitas posições
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
void * tarefa(void *arg) {

    long long int id = (long long int) arg;
    //variáveis locais do menor e maior elemento do bloco
    minmax *min_max = (minmax*) malloc(sizeof(minmax));
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
    for (long long int i = ini; i < fim; i++) {
        if (i == ini) {
            min_max->min = vetor[i];
            min_max->max = vetor[i];
        }
        else {
            if (vetor[i] < min_max->min){
                min_max->min = vetor[i];
            }
            if (vetor[i] > min_max->max) {
                min_max->max = vetor[i];
            }
        }
    }

    //retorna o resultado da soma local
    pthread_exit((void *) min_max);
}

//fluxo principal
int main(int argc, char *argv[]) {
    float ini, fim, t_seq, t_conc; //tomada de tempo
    float min_conc; //minimo global
    float max_conc; //maximo global
    pthread_t *tid; //identificadores das threads no sistema
    minmax *retorno_seq, *retorno_conc; //valores de retorno

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

    //aloca a struct de retorno sequencial
    retorno_seq = malloc(sizeof(minmax));
    if (retorno_seq == NULL) {
        fprintf(stderr, "ERRO--malloc\n");
        return 2;
    }

    //aloca a struct de retorno concorrente
    retorno_conc = malloc(sizeof(minmax));
    if (retorno_conc == NULL) {
        fprintf(stderr, "ERRO--malloc\n");
        return 2;
    }

    //preenche o vetor de entrada
    srand(time(NULL));
    for (long long int i = 0; i < dim; i++) {
        vetor[i] = rand();
    }

    //min_max sequencial dos elementos
    GET_TIME(ini);
    for(long long int i=0; i<dim; i++) {
        if (i == 0) {
            retorno_seq->min = vetor[i];
            retorno_seq->max = vetor[i];
        }
        else {
            if (vetor[i] < retorno_seq->min){
                retorno_seq->min = vetor[i];
            }
            if (vetor[i] > retorno_seq->max) {
                retorno_seq->max = vetor[i];
            }
        }
    }
    GET_TIME(fim);
    t_seq = fim - ini;

    //criando o vetor de threads
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

    
    //aguardar o termino das threads
    for(long long int i = 0; i<nthreads; i++) {
        if(pthread_join(*(tid+i), (void**) &retorno_conc)){
            fprintf(stderr, "ERRO--pthread_create\n");
            return 3;
        }

        //análise de min-max global
        if (i == 0) {
            min_conc = retorno_conc->min;
            max_conc = retorno_conc->max;
        }
        else {
            if (retorno_conc->min < min_conc){
                min_conc = retorno_conc->min;
            }
            if (retorno_conc->max > max_conc) {
                max_conc = retorno_conc->max;
            }
        }
        free(retorno_conc);
    }
    GET_TIME(fim);
    t_conc = fim - ini;

    //exibir os resultados
    if ((min_conc == retorno_seq->min) && (max_conc == retorno_seq->max)) {
        printf("Valores iguais!\nMinimo:  %.12lf\nMaximo: %.12lf\n\n", min_conc, max_conc);
    }
    else {
        fprintf(stderr, "Valores diferentes!\n");
        return 4; 
    }
    printf("%lf/%lf = %lf\n", t_seq, t_conc, t_seq/t_conc);

    //libera as areas de memoria alocadas
    free(vetor);
    free(tid);
    free(retorno_seq);

    return 0;
}
