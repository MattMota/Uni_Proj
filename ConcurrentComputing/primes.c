#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>
#include <math.h>
#include "timer.h"

// variável compartilhada entre as threads
// e dimensão do vetor
long long int dim, global_iter = 0;
// vetor de entrada
int *vetor_entrada;
// variável de lock para exclusão mutua
pthread_mutex_t mutex; 

int ehPrimo(long long int n) {
    if (n <= 1)
        return 0;
    if (n == 2)
        return 1;
    for (int i = 3; i < sqrt(n)+1; i++) {
        if (!(n%i))
            return 0;
    }
    return 1;
}

// função executada pelas threads
void* ExecutaTarefa(void* vetor_conc) {
    double* vetor = (double*) vetor_conc;
    int i = (global_iter < dim)? 0 : (dim + 1);
    
    while (global_iter < dim) {

        //entrada na seção concorrente
        pthread_mutex_lock(&mutex);
        
        // seção concorrente
        i = global_iter++; //incrementa o contador de tarefas realizadas

        //--saida da SC
        pthread_mutex_unlock(&mutex);

        vetor[i] = (ehPrimo(vetor_entrada[i])? sqrt(vetor_entrada[i]) : vetor_entrada[i]);
    }

    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    // medidas de tempo
    double inicio, fim, tempo_seq, tempo_conc;
    // identificador e quantidade de threads
    pthread_t *thread_id;
    int nthreads;
    // vetores de saída
    double *vetor_seq, *vetor_conc;

    // recebe e valida os parametros de entrada (dimensao do vetor, numero de threads)
    if (argc < 3) {
        fprintf(stderr, "Digite: %s <dimensao do vetor> <numero de threads>\n", argv[0]);
        return 1;
    }

    // converte os argumentos recebidos em variáveis
    dim = atoll(argv[1]);
    nthreads = atoi(argv[2]);

    // aloca o vetor de entrada, e os de saída
    vetor_entrada = (int *)malloc(sizeof(int) * dim);
    if (vetor_entrada == NULL) {
        fprintf(stderr, "ERRO--malloc\n");
        return 2;
    }
    vetor_seq = (double *)malloc(sizeof(double) * dim);
    if (vetor_seq == NULL) {
        fprintf(stderr, "ERRO--malloc\n");
        return 2;
    }
    vetor_conc = (double *)malloc(sizeof(double) * dim);
    if (vetor_conc == NULL) {
        fprintf(stderr, "ERRO--malloc\n");
        return 2;
    }

    // preenche o vetor de entrada
    srand(time(NULL));
    for (long long int i = 0; i < dim; i++)
        vetor_entrada[i] = rand() % dim;

    // processamento sequencial dos elementos
    GET_TIME(inicio);
    
    for (long long int i = 0; i < dim; i++) {
        vetor_seq[i] = (ehPrimo(vetor_entrada[i])? sqrt(vetor_entrada[i]) : vetor_entrada[i]);
    }

    GET_TIME(fim);
    
    tempo_seq = fim - inicio;
    printf("Tempo sequencial:  %lf\n", tempo_seq);

    // processamento concorrente dos elementos
    GET_TIME(inicio);

    // aloca as threads
    thread_id = (pthread_t *)malloc(sizeof(pthread_t) * nthreads);
    if (thread_id == NULL) {
        fprintf(stderr, "ERRO--malloc\n");
        return 2;
    }

    // cria as threads
    pthread_mutex_init(&mutex, NULL);
    for (long int i = 0; i < nthreads; i++) {
        if (pthread_create(thread_id + i, NULL, ExecutaTarefa, (void*) vetor_conc)) {
            fprintf(stderr, "ERRO--pthread_create\n");
            return 3;
        }
    }

    // aguarda o término das threads
    for (long int i = 0; i < nthreads; i++) {
        if (pthread_join(*(thread_id + i), NULL)) {
            fprintf(stderr, "ERRO--pthread_create\n");
            return 3;
        }
    }
    pthread_mutex_destroy(&mutex);
    
    GET_TIME(fim);
    tempo_conc = fim - inicio;
    printf("Tempo concorrente:  %lf\n", tempo_conc);

    // exibe os resultados
    printf("Aceleração: %lf\n", tempo_seq/tempo_conc);

    int igual = 1;
    for (int i = 0; i < dim; i++) {
        if (vetor_seq[i] != vetor_conc[i]) {
            igual = 0;
        }
    }

    if (igual)
        printf("Sequencial = Concorrente\n");
    else
        printf("Sequencial != Concorrente\n");


    // libera as áreas de memória alocadas
    free(vetor_entrada);
    free(vetor_seq);
    free(vetor_conc);
    free(thread_id);

    return 0;
}