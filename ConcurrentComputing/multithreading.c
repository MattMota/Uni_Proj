/*
Atividade 5
Matheus Mota S. Silva
*/

/*
	Implementação "burra" em batch. O ideal seria alocar
	individualmente cada tarefa (quadrado e return) a uma
	determinada thread baseado em sua posição na "queue".
	Isto encurtaria o processamento para aprox. n/2 ciclos.
	
	Ex.: vetor[1] -> thread 1, vetor[2] -> thread 2,
	     vetor[3] -> thread 1, vetor[4] -> thread 2, etc.
*/

#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>

#define NUM_THREADS 2
#define TAM_VETOR 10000

int copia_vetor[TAM_VETOR];

/*
Definimos os parâmetros de cada thread:
um range e o vetor em que ela vai atuar
*/
typedef struct thread_args {
	int inicio;
	int fim;
	int* vetor;
} thread_args;

/*
Definimos a função de quadrado: para o range da thread atual,
ela pega o subvetor correspondente e calcula o quadrado para
cada um de seus elementos.
*/
void *square (void* args) {
	
	// Capturamos os argumentos passados
	thread_args t_args = * (thread_args*) args;
	
	// Montamos o subvetor e realizamos o quadrado em seus elementos
	for (int i = (t_args.inicio); i < (t_args.fim); i++) {
		t_args.vetor[i] *= t_args.vetor[i];
	}
	
	pthread_exit(NULL);
}

// Thread principal
int main (void) {
	int vetor_quadrado[TAM_VETOR];
	
	// Preenchendo o vetor_quadrado
	for (int i = 0; i < (TAM_VETOR); i++) {
		// vetor[0] = 1, vetor[1] = 2, etc.
		vetor_quadrado[i] = i+1;
		//printf("%d \n", (vetor_quadrado[i]));
		
	}
	
	/*
	A partir daqui, supomos que não conhecemos o vetor
	mas sabemos que ele é par
	*/
	
	/*
	Inicializamos as variáveis necessárias:
	identificadores das threads, tamanho do vetor (por conveniência),
	cópia do vetor para checar se a operação foi feita corretamente.
	*/ 
	pthread_t thread_id[NUM_THREADS];
	int tam_vetor = sizeof(vetor_quadrado)/sizeof(int);
	//int copia_vetor[tam_vetor];
	
	// Preenchemos a cópia do vetor
	for (int i = 0; i < (tam_vetor); i++) {
		copia_vetor[i] = vetor_quadrado[i];
	}
	
	// Criamos os argumentos necessários para cada thread
	thread_args vetor_args[NUM_THREADS] = {{0, tam_vetor/2, vetor_quadrado}, {tam_vetor/2, tam_vetor, vetor_quadrado}};
	

	// Criação das threads propriamente ditas
	for (int i = 0; i < NUM_THREADS; i++) {
	
		// Se não conseguiu criar thread, joga erro
		if (pthread_create(&thread_id[i], NULL, square, (void *)&vetor_args[i])) {
			printf("Erro de thread");
		}
	}
	
	// Sincronização das threads após execução
	for (int i = 0; i < NUM_THREADS; i++) {
	
		// Se não conseguiu encerrar thread, joga erro
		if (pthread_join(thread_id[i], NULL)) {
			printf("Erro de thread");
		}
	}
	
	// Checamos se a operação está correta
	int correto = 1;
	for (int i = 0; i < (tam_vetor); i++) {
		if (vetor_quadrado[i] != (copia_vetor[i] * copia_vetor[i])) {
		printf("%d != %d*%d \n", vetor_quadrado[i], copia_vetor[i], copia_vetor[i]);
			correto = 0;
		}
	}
	
	if (correto == 1) {
		printf("Operação correta!\n");
	}
	else {
		printf("Operação incorreta!\n");
	}
	
	// Fim do programa
	return 0;
}
