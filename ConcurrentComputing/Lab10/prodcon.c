#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>

#define NTHREADS 10

sem_t em_e, em_l, escr, leit;
long int e = 0, l = 0;

// Função executada pelas threads leitoras
void* threadLeit(void* thread_id) {
    long int leit_id = (long int) thread_id;

    while (1) {

        // Leitor ocupa o semáforo de leitores
        printf("Leitor %ld quer comecar\n", leit_id);
        sem_wait(&leit);
        printf("Leitor %ld comecou\n", leit_id);

        // Leitor inicia 1a parte que requer
        // exclusão mútua das threads leitoras
        sem_wait(&em_l);

        // Aumenta contagem de leitores
        l++;
        printf("Aumentando contagem de leitores para %ld\n", l);

        // Se há leitor lendo, bloqueia
        // o semáforo de escritores
        if (l == 1) {
            printf("Bloqueando escritores de comecar; há leitores trabalhando\n");
            sem_wait(&escr);
        }
        
        // Leitor encerra 1a parte
        // de exclusão mútua%d dos leitores
        sem_post(&em_l);

        // Leitor libera o semáforo dos leitores
        sem_post(&leit);

        printf("Leitor %ld lendo...\n", leit_id);

        // -------- Faz leitura --------

        printf("Leitor %ld encerrou leitura\n", leit_id);

        // Leitor inicia 2a parte que requer
        // exclusão mútua das threads leitoras
        sem_wait(&em_l);

        // Diminui contagem de leitores
        printf("Diminuindo contagem de leitores para %ld\n", l-1);
        l--;

        // Se não há leitor lendo, libera
        // o semáforo de escritores
        if (l == 0) {
            printf("Liberando escritores para comecarem; não há leitores trabalhando\n");
            sem_post(&escr);
        }
        
        // Leitor encerra 2a parte
        // de exclusão mútua dos leitores
        printf("Leitor %ld encerrou\n", leit_id);
        sem_post(&em_l);

    }

}

// Função executada pelas threads escritoras
void* threadEscr(void* thread_id) {
    long int escr_id = (long int) thread_id;

    while (1) {

        // Escritor inicia 1a parte que requer
        // exclusão mútua das threads escritoras
        printf("Escritor %ld quer comecar\n", escr_id);
        sem_wait(&em_e);
        printf("Escritor %ld comecou\n", escr_id);

        // Aumenta contagem de escritores
        e++;
        printf("Aumentando contagem de escritores para %ld\n", e);

        // Se há escritor escrevendo, bloqueia o
        // semáforo de escritores
        if (e == 1) {
            printf("Bloqueando leitores de comecar; há escritores trabalhando\n");
            sem_wait(&leit);
        }

        // Escritor encerra 1a parte que requer
        // exclusão mútua das threads escritoras
        sem_post(&em_e);

        // Escritor ocupa semáforo de escritores
        printf("Escritor %ld escrevendo...\n", escr_id);
        sem_wait(&escr);

        // -------- Faz escrita --------

        // Escritor libera semáforo de escritores
        printf("Escritor %ld encerrou escrita\n", escr_id);
        sem_post(&escr);

        // Escritor inicia 2a parte que requer
        // exclusão mútua das threads escritoras
        sem_wait(&em_e);

        // Diminui contagem de escritores
        printf("Diminuindo contagem de escritores para %ld\n", e-1);
        e--;

        // Se não há escritor escrevendo,
        // bloqueia o semáforo de escritores
        if (e == 0) {
            printf("Liberando leitores para comecarem; não há escritores trabalhando\n");
            sem_post(&leit);
        }

        // Escritor encerra 2a parte que requer
        // exclusão mútua das threads escritoras
        printf("Escritor %ld encerrou\n", escr_id);
        sem_post(&em_e);

    }

}

// Função principal
int main(int argc, char *argv[]) {
    pthread_t tid[NTHREADS];

    sem_init(&escr, 0, 1);
    sem_init(&leit, 0, 1);
    sem_init(&em_e, 0, 1);
    sem_init(&em_l, 0, 0);

    srand(time(0));
    int upper = NTHREADS-2, lower = 2;


    int leit_count = (rand() % (upper - lower + 1)) + lower;
    int escr_count = NTHREADS - leit_count;

    printf("Threads leitoras: %d; threads escritoras: %d\n", leit_count, escr_count);

    // Criamos as threads escritoras
    for (long int i = 0; i < escr_count; i++) {
        if(pthread_create(tid + i, NULL, threadEscr, (void*) i+1)){
            fprintf(stderr, "ERRO: pthread_create\n");
            exit(-1);
        } 
    }

    // Criamos as threads leitoras
    for (long int i = 0; i < leit_count; i++) {
        if(pthread_create(tid + i, NULL, threadLeit, (void*) i+1)){
            fprintf(stderr, "ERRO: pthread_create\n");
            exit(-1);
        } 
    }

    // Espera todas as threads terminarem
    for (int i = 0; i<NTHREADS; i++) {
        if (pthread_join(*(tid + i), NULL)) {
            printf("ERRO: pthread_join() \n");
            exit(-1); 
        } 
    }

    sem_destroy(&escr);
    sem_destroy(&leit);
    sem_destroy(&em_e);
    sem_destroy(&em_l);
}
