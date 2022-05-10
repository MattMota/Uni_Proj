/* Disciplina: Computacao Concorrente */
/* Prof.: Silvana Rossetto */
/* Codigo: Comunicação entre threads usando variável compartilhada e exclusao mutua com bloqueio */

#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>
#include "timer.h"

int global_iter = 0; //variavel compartilhada entre as threads
int dim; //dimensao do vetor
pthread_mutex_t mutex; //variavel de lock para exclusao mutua

typedef struct vetores {
    int vetorEntrada[];
    float vetorSaida[];
} vetores;

int ehPrimo(long long int n) {
  if (n <= 1)
    return 0;
  if (n == 2)
    return 1;
  for (int i = 3; i < sqrt(n)+1; i++) {
    if (n%i == 0)
      return 0;
  }
  return 1;
}

void processaPrimos(int *vetorEntrada, float *vetorSaida) {
  for (int i = 0; i < dim, i++) {
    if (ehPrimo(vetorEntrada[i])
      vetorSaida[i] = sqrt(vetorEntrada[i]);
    else
      vetorSaida[i] = vetorEntrada[i];
  }
}

//funcao executada pelas threads
void *ExecutaTarefa(void *args) {
    int *vetorEntrada = ((vetores*) args)->vetorEntrada;
    float *vetorSaida = ((vetores*) args)->vetorSaida;
    int i;
    while (global_iter < dim) {
        //--entrada na SC
        pthread_mutex_lock(&mutex);
        //--SC
        i = global_iter++; //incrementa o contador de tarefas realizadas 
        //--saida da SC
        pthread_mutex_unlock(&mutex);

        if (ehPrimo(vetorEntrada[i])
            vetorSaida[i] = sqrt(vetorEntrada[i]);
        else
            vetorSaida[i] = vetorEntrada[i];
    }
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
  double inicio, fim;
    pthread_t *thread_id;
    float *vetor_seq, *vetor_conc;

    // recebe e valida os parametros de entrada (dimensao do vetor, numero de threads)
    if (argc < 3) {
        fprintf(stderr, "Digite: %s <dimensao do vetor> <numero de threads>\n", argv[0]);
        return 1;
    }
    size = atoll(argv[1]);
    nthreads = atoi(argv[2]);
    // aloca o vetor de entrada
    vetor = (int *)malloc(sizeof(int) * size);
    if (vetor == NULL)
    {
        fprintf(stderr, "ERRO--malloc\n");
        return 2;
    }
    vetor_seq = (float *)malloc(sizeof(float) *  size);
    vetor_conc = (float *)malloc(sizeof(float) *  size);
    srand(time(NULL));
    // preenche o vetor de entrada
    for (long int i = 0; i < size; i++)
        vetor[i] = rand() % size;

    // analise sequencial dos elementos
    GET_TIME(inicio);
    for (long long int i = 0; i < size; i++){
        //fazer
        vetor_seq[i] = ehPrimo(vetor[i]) ? sqrt(vetor[i]) : vetor[i];
    }

    GET_TIME(fim);
    tempo_seq = fim - inicio;
    printf("Tempo sequencial:  %lf\n", tempo_seq);

    // analise concorrente dos elementos
    GET_TIME(inicio);
    thread_id = (pthread_t *)malloc(sizeof(pthread_t) * nthreads);
    if (thread_id == NULL)
    {
        fprintf(stderr, "ERRO--malloc\n");
        return 2;
    }
    // criar as threads
    pthread_mutex_iniciot(&lock, NULL);
    for (long int i = 0; i < nthreads; i++)
    {
        if (pthread_create(thread_id + i, NULL, tarefa, (void *)vetor_conc))
        {
            fprintf(stderr, "ERRO--pthread_create\n");
            return 3;
        }
    }
    // aguardar o termino das threads
    for (long int i = 0; i < nthreads; i++)
    {
        if (pthread_join(*(thread_id + i), NULL))
        {
            fprintf(stderr, "ERRO--pthread_create\n");
            return 3;
        }
    }
    pthread_mutex_destroy(&lock);
    GET_TIME(fim);
    tempo_conc = fim - inicio;
    printf("Tempo concorrente:  %lf\n", tempo_conc);

    // exibir os resultados
    printf("Aceleração: %lf\n", tempo_seq/tempo_conc);

    // libera as areas de memoria alocadas
    free(vetor);
    free(thread_id);

    return 0;
}
