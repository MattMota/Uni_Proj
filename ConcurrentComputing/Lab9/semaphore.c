#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>
#include <semaphore.h>

#define NTHREADS 5

// Variaveis globais
int mensagens = 0;      //variavel compartilhada entre as threads
sem_t cond_meio, cond_fim;     //semaforos para sincronizar a ordem de execucao das threads

// Função executada pelas threads
void* threadFunc(void* tid) {
    int* thread_id = (int*) tid;
    switch (thread_id) {
        // Função executada pela thread 1
        case 1:
            sem_wait(&cond_fim); // Espera T2, T3, T4 e T5 executarem
            printf("Volte sempre!\n");
            pthread_exit(NULL);
            break;
        // Função executada pela thread 2
        case 2:
            sem_wait(&cond_fim); // Espera T5 executar
            printf("Fique a vontade.\n");
            sem_post(&cond_meio); // Permite que T3 e T4 executem
            sem_post(&cond_fim); // Permite que T1 execute se houve 3 posts desse
            pthread_exit(NULL);
            break;
        // Função executada pela thread 3
        case 3:
            sem_wait(&cond_meio); // Espera T5 executar
            printf("Sente-se por favor.\n");
            sem_post(&cond_meio); // Permite que T2 e T4 executem
            sem_post(&cond_fim); // Permite que T1 execute se houve 3 posts desse
            pthread_exit(NULL);
            break;
        // Função executada pela thread 4
        case 4:
            sem_wait(&cond_meio); // Espera T1 executar
            printf("“Aceita um copo d'agua?”\n");
            sem_post(&cond_meio); // Permite que T2 e T3 executem
            sem_post(&cond_fim); // Permite que T1 execute se houve 3 posts desse
            pthread_exit(NULL);
            break;
        // Função executada pela thread 5
        case 5:
            printf("Seja bem-vindo!\n");
            sem_post(&cond_meio); // Permite que T2, T3 e T4 executem
            pthread_exit(NULL);
            break;
        default:
            return -1;
    }
}

// Função principal
int main(int argc, char *argv[]) {
    pthread_t tid[NTHREADS];

    // Inicia os semaforos
    /*
    O semáforo dos intermediários inicia com 0
    pois só pode executar quando receber 1, depois que
    T5 for executada e fizer sem_post(&cond_meio)
    */
    sem_init(&cond_meio, 0, 0);
    /*
    O semáforo final inicia com -2
    pois só pode executar quando receber 1, depois que
    T2, T3 e T4 forem executadas e todas fizerem sem_post(&cond_fim)
    */
    sem_init(&cond_fim, 0, -2);

    // Criamos as threads individuais
    for (int i = 0; i < NTHREADS; i++) {
        if(pthread_create(&tid[i], NULL, threadFunc, (void*) i+1)){
            fprintf(stderr, "ERRO: pthread_create\n");
            return 1;
        } 
    }

    // Espera todas as threads terminarem
    for (t=0; t<NTHREADS; t++) {
        if (pthread_join(tid[t], NULL)) {
            printf("ERRO: pthread_join() \n"); exit(-1); 
        } 
        free(id[t]);
    }
}