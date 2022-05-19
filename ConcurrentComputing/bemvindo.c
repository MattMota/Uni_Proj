#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NTHREADS 5

int mensagens = 0;
pthread_mutex_t mutex;
pthread_cond_t cond;


void* mensagem(void* tid) {
    long int thread_id = (long int) tid;

    // printf("Começando thread %d\n", thread_id);

    // Toma controle
    pthread_mutex_lock(&mutex);

    // Se nenhuma mensagem foi impressa
    // e essa não é a thread 5, aguarda
    if ((!mensagens) && (thread_id != 5)) {
        pthread_cond_wait(&cond, &mutex);
    }

    // Checa o id da thread atual
    switch (thread_id) {
        case 1:
            // Se esta é a thread 1, enquanto não terminar
            // de imprimir todas as outras mensagens, aguarda
            while ((mensagens < 4) && (thread_id == 1)) {
                pthread_cond_wait(&cond, &mutex);
            }
            
            printf("Volte sempre!\n");
            break;
        case 2:
            printf("Fique a vontade.\n");
            break;
        case 3:
            printf("Sente-se por favor.\n");
            break;
        case 4:
            printf("Aceita um copo d'agua?\n");
            break;
        case 5:
            printf("Seja bem-vindo!\n");

            // Se esta é a thread 5, ela imprime
            // a mensagem e destrava as outras
            pthread_cond_broadcast(&cond);
            break;
    }

    // Incrementa o contador de mensagens
    mensagens++;

    // Devolve o controle
    pthread_mutex_unlock(&mutex);

    // Se já imprimimos as demais mensagens
    if (mensagens == 4)
        // Destravamos a thread 1
        pthread_cond_signal(&cond);

    // Encerra a função
     pthread_exit(NULL);
}


int main() {
    // Declaramos e inicializamos
    // o vetor de threads
    pthread_t *threads;
    threads = (pthread_t *)malloc(sizeof(pthread_t) * NTHREADS);

    // Declaramos a variável de controle
    // e as condicionais
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond, NULL);

    // Criamos as threads individuais
    for (long int i = 0; i < NTHREADS; i++) {
        if(pthread_create(threads+i, NULL, mensagem, (void*) i+1)){
            fprintf(stderr, "ERRO: pthread_create\n");
            return 1;
        } 
    }

    // Esperamos todas as threads terminarem
    for (long int i = 0; i < NTHREADS; i++) {
        if(pthread_join(*(threads + i), NULL)){
            fprintf(stderr, "ERRO: pthread_create\n");
            return 2;
        } 
    }

    // Destruímos a variável de controle
    // e as condicionais
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);

    // Desalocamos as threads
    free(threads);

    // Fim
    return 0;
}