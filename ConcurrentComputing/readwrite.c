#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define L 4 // Número de threads leitoras
#define E 1 // Número de threads escritoras

// Variáveis do problema
int lendo = 0; // Contador de threads lendo
int escrevendo = 0; // Contador de threads escrevendo
int esperando = 0; // COntador de escritores em espera

// Variáveis para sincronização
pthread_mutex_t mutex;
pthread_cond_t cond_leitura, cond_escrita;

/*
Na definição básica do problema dos leitores/escritores,
sempre que um leitor quiser ler e não houver escritor
escrevendo (pode haver escritor esperando), ele tem
acesso à estrutura de dados. Nesta solução, um escritor
pode ter de esperar por muito tempo (inanição),
caso novos leitores cheguem com frequência.

Uma alternativa para minimizar longas esperas
do lado dos escritores e a seguinte: quando um escritor
desejar escrever, nenhum leitor pode iniciar uma leitura
enquanto o escritor não for atendido. Nesta solução,
um leitor pode ter de esperar por muito tempo (inanição),
caso novos escritores cheguem com frequência.
*/

// Início da leitura
void inicio_leitura (int id) {
    // Thread "pega" o mutex, só ela executando
    pthread_mutex_lock(&mutex);
    printf("L[%d] quer ler\n", id);
    // Enquanto houver escritores trabalhando,
    // espera todos acabarem, mesmo recebendo signal
    while (escrevendo || esperando) {
        // Espera terminar a(s) escritas(s)
        printf("L[%d] bloqueou\n", id);
        pthread_cond_wait(&cond_leitura, &mutex);
        printf("L[%d] desbloqueou\n", id);
    }
    // Anota que vai começar a ler
    lendo++;
    // "Libera" o mutex para outras threads usarem
    pthread_mutex_unlock(&mutex);
}

// Início da escrita
void inicio_escrita (int id) {
    // Thread "pega" o mutex, só ela executando
    pthread_mutex_lock(&mutex);
    printf("E[%d] quer escrever\n", id);
    // Enquanto houver leitores ou escritores trabalhando,
    // espera todos acabarem, mesmo recebendo signal
    while(lendo || escrevendo) {
        printf("E[%d] bloqueou\n", id);
        esperando++;
        pthread_cond_wait(&cond_escrita, &mutex);
        esperando--;
        printf("E[%d] desbloqueou\n", id);
    }
    // Anota que vai começar a ler
    escrevendo++;
    // "Libera" o mutex para outras threads usarem
    pthread_mutex_unlock(&mutex);
}

// Fim da leitura
void fim_leitura (int id) {
    // Thread "pega" o mutex, só ela executando
    pthread_mutex_lock(&mutex);
    // Anota que já acabou de ler
    printf("L[%d] terminou de ler\n", id);
    lendo--;
    // Se não houver ninguém lendo
    if (!lendo)
        // Avisa os escritores que um deles pode escrever
        pthread_cond_signal(&cond_escrita);
    // "Libera" o mutex para outras threads usarem
    pthread_mutex_unlock(&mutex);
}

// Fim da escrita
void fim_escrita (int id) {
    // Thread "pega" o mutex, só ela executando
    pthread_mutex_lock(&mutex);
    // Anota que já acabou de escrever
    printf("E[%d] terminou de escrever\n", id);
    escrevendo--;
    // Se não houver nenhum leitor esperando
    if (!esperando)
        // Avisa que todos os leitores podem ler
        pthread_cond_broadcast(&cond_leitura);
    // Avisa que SÓ UM escritor pode escrever
    pthread_cond_signal(&cond_escrita);
    // "Libera" o mutex para outras threads usarem
    pthread_mutex_unlock(&mutex);
}

// Processamento da thread leitora
void* leitor (void* arg) {
    int* id = (int*) arg;
    while (1) {
        inicio_leitura(*id);
        printf("Leitora %d esta lendo\n", *id);
        fim_leitura(*id);
        sleep(1);
    } 
    free(arg);
    pthread_exit(NULL);
}

// Processamento da thread escritora
void* escritor (void* arg) {
    int *id = (int*) arg;
    while (1) {
        inicio_escrita(*id);
        printf("Escritora %d esta escrevendo\n", *id);
        fim_escrita(*id);
        sleep(1);
    } 
    free(arg);
    pthread_exit(NULL);
}

// Função principal
int main(void) {
    // Identificadores das threads
    pthread_t thread_id[L+E];
    int id[L+E];

    // Inicializa as variáveis de sincronização
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond_leitura, NULL);
    pthread_cond_init(&cond_escrita, NULL);

    // Cria as threads leitoras
    for (int i = 0; i < L; i++) {
        id[i] = i+1;
        if (pthread_create(&thread_id[i], NULL, leitor, (void*) &id[i]))
            exit(-1);
    } 

    // Cria as threads escritoras
    for (int i = 0; i < E; i++) {
        id[i+L] = i+1;
        if (pthread_create(&thread_id[i+L], NULL, escritor, (void *) &id[i+L]))
            exit(-1);
    } 

    pthread_exit(NULL);
    return 0;
}
