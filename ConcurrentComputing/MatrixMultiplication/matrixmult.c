// Multiplicação de matrizes (considerando matrizes quadradas)


// Bibliotecas necessárias
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include "timer.h"


// Declaração das variáveis globais:

// Matrizes de entrada no. 1 e no. 2, e de saída
float *matriz_1, *matriz_2, *saida;

// Quantidade de threads
int num_threads;


// Definimos a estrutura a ser passada para a função concorrente
typedef struct{
    int id; // Identificador do elemento que a thread irá processar
    int dim; // Dimensões das matrizes quadradas
} tArgs;


// Função que as threads executarão independentemente
void * tarefa(void *arg) {
    tArgs *args = (tArgs*) arg;

    for (int i = (args->id); i < (args->dim); i += num_threads) {
      for (int j = 0; j < (args->dim); j++) {
         for (int k = 0; k < (args->dim); k++) {
            saida[i*(args->dim) + j] += matriz_1[i*(args->dim) + k] * matriz_2[k*(args->dim) + j];
         }
      }
    }

    pthread_exit(NULL);
}


// Fluxo principal
int main(int argc, char* argv[]) {

    // Declaração das variáveis:

    int dim; // Dimensões das matrizes quadradas
    int erro = 0; // Flag de erro da comparação de matrizes (começa desativado)
    pthread_t *thread_id; // Idents. das threads no sistema
    tArgs *args; // Idents. locais das threads e dimensões

    // Variáveis de medida de tempo
    double inicio_pt1, fim_pt1, tempo_pt1;
    double inicio_pt2, fim_pt2, tempo_pt2;
    double inicio_seq, fim_seq, tempo_seq;
    double inicio_conc, fim_conc, tempo_conc;
    double delta;


    // Guarda o início da 1a parte do programa
    GET_TIME(inicio_pt1);


    /*
    Leitura e avaliação dos parâmetros de entrada:
    [nome programa] [dimensão 1] [dimensão 2] [no. threads]

    Caso não estejam todos preenchidos, retorna -1 e joga erro
    */
    if (argc < 3) {
        fprintf(stderr, "Passe os seguintes parâmetros: %s [dimensões das matrizes] [no. threads]\n", argv[0]);
        return -1;
    }
    // Caso esteja tudo certo, programa continua


    // Convertendo os parâmetros de string para int
    dim = atoi(argv[1]);
    num_threads = atoi(argv[2]);


    /*
    Nesta abordagem, não há necessidade de ter
    mais threads que linhas ou colunas. Portanto,
    se for maior, limitamos à quantidade destes.
    */
    if (num_threads > dim) {
        num_threads = dim;
    }


    // Alocação de memória para as estruturas de dados:

    // Identificadores das threads
    thread_id = (pthread_t*) malloc(sizeof(pthread_t)*num_threads);
    if (thread_id == NULL) {
        fprintf(stderr, "ERRO--malloc");
        return -2;
    }

    // Argumentos de cada thread
    args = (tArgs*) malloc(sizeof(tArgs)*num_threads);
    if (args == NULL) {
        fprintf(stderr, "ERRO--malloc");
        return -2;
    }

    // Matriz 1
    matriz_1 = (float *) malloc(sizeof(float) * dim * dim);
    if (matriz_1 == NULL) {
        fprintf(stderr, "ERRO--malloc\n");
        return -2;
    }

    // Matriz 2
    matriz_2 = (float *) malloc(sizeof(float) * dim * dim);
    if (matriz_2 == NULL) {
        fprintf(stderr, "ERRO--malloc\n");
        return -2;
    }

    // Matriz resultante (de saída)
    saida = (float *) malloc(sizeof(float) * dim * dim);
    if (saida == NULL) {
        fprintf(stderr, "ERRO--malloc\n");
        return -2;
    }


    // Inicialização das estruturas de dados

    // Cria uma seed pra randomizar valores das matrizes
    srand(time(NULL));

    /*
    Preenche as matrizes de entrada
    com valores aleatórios e a de saída com 0s
    */
    for (int i = 0; i < dim; i++) {
        for (int j = 0; j < dim; j++) {
            matriz_1[i + j*dim] = rand(); // matriz_1[i][j] = rand();
            matriz_2[i + j*dim] = rand(); // matriz_2[i][j] = rand();
            saida[i + j*dim] = 0; // saida[i][j] = 0;
        }
    }


    // Guarda o fim da 1a parte do programa
    GET_TIME(fim_pt1);
    

    // Guarda o início do processamento concorrente
    GET_TIME(inicio_conc);


    // Criação das threads (também usaremos a thread principal):

    /*
    Para cada thread (começa em 1 porque
    a thread principal também trabalha)
    */
    for (int i = 1; i < num_threads; i++) {
        /*
        Preenchemos seus argumentos,
        endereçando sequencialmente
        */
        (args + i)->id = i;
        (args + i)->dim = dim;

        // Criamos a thread passando os argumentos
        if (pthread_create(thread_id+i, NULL, tarefa, (void*) (args + i))) {
            fprintf(stderr, "ERRO--pthread_create");
            return -3;
        }
    }
    

    // Processamento da thread principal
    for (int i = 0; i < dim; i += num_threads) {
      for (int j = 0; j < dim; j++) {
         for (int k = 0; k < dim; k++) {
            saida[i*dim + j] += matriz_1[i*dim + k] * matriz_2[k*dim + j];
         }
      }
    }


    // Espera pela sincronização das threads
    for(int i = 1; i < num_threads; i++) {
        pthread_join(*(thread_id+i), NULL);
    }


    // Guarda o fim do processamento concorrente
    GET_TIME(fim_conc);


    // Guarda o início do processamento sequencial
    GET_TIME(inicio_seq);

    double soma = 0.0;
    for (int i = 0; i < dim; i++) {
      for (int j = 0; j < dim; j++) {
         for (int k = 0; k < dim; k++) {
            soma += matriz_1[i + k*dim] * matriz_2[k + j*dim];
         }
         if (saida[i + j*dim] != soma){
             erro = 1;
         }
         printf("matriz seq[%d][%d]: %lf \n", i,j, soma);
         printf("matriz conc[%d][%d]: %lf \n", i,j, saida[i + j*dim]);
         soma = 0.0;
      }
    } 

    // Guarda o fim do processamento sequencial
    GET_TIME(fim_seq);


    // Guarda o início da 2a parte do programa
    GET_TIME(inicio_pt2);


    // Liberação da memória
    free(matriz_1);
    free(matriz_2);
    free(saida);
    free(thread_id);
    free(args);


    // Avisa caso o cálculo sequencial e concorrente sejam diferentes
    if (erro) {
        fprintf(stderr, "ERRO--calculo de matriz\n");
        return -4;
    }


    // Guarda o fim da 2a parte do programa
    GET_TIME(fim_pt2);


    // Cálculo do tempo sequencial vs. tempo concorrente
    tempo_pt1 = (fim_pt1 - inicio_pt1);
    tempo_pt2 = (fim_pt2 - inicio_pt2);
    tempo_seq = (fim_seq - inicio_seq);
    tempo_conc = (fim_conc - inicio_conc);

    delta = (tempo_pt1 + tempo_seq + tempo_pt2)/(tempo_pt1 + tempo_conc + tempo_pt2);

    printf("Tempo em computação sequencial: %lf \nTempo em computação concorrente: %lf \nAceleração: %lf \n", tempo_seq, tempo_conc, delta);

    return 0;
}
