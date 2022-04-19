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
	int* vetor;
	int tam_vetor;
	int index_inicial;
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
	for (int i = t_args.index_inicial; i < t_args.tam_vetor; i += NUM_THREADS + 1) {
		t_args.vetor[i] *= t_args.vetor[i];
	}

	// Libera o espaço de memória dos argumentos
	free(args);
	
	pthread_exit(NULL);
}

// Thread principal
int main (void) {
	int vetor_quadrado[TAM_VETOR];
	
	// Preenchendo o vetor_quadrado
	for (int i = 0; i < (TAM_VETOR); i++) {
		// vetor[0] = 1, vetor[1] = 2, etc.
		vetor_quadrado[i] =         //int copia_vetor[tam_vetor];
78
        
79
        // Preenchemos a cópia do vetor
80
        for (int i = 0; i < (tam_vetor); i++) {
81
                copia_vetor[i] = vetor_quadrado[i];
82
        }
83
        
84
        // Criamos os argumentos necessários para cada thread
85
        thread_args vetor_args[NUM_THREADS] = {{0, tam_vetor/2, vetor_quadrado}, {tam_vetor/2, tam_vetor, vetor_quadrado}};
86
        
87
​
88
        // Criação das threads propriamente ditas
89
        for (int i = 0; i < NUM_THREADS; i++) {
90
        
91
                // Se não conseguiu criar thread, joga erro
92
                if (pthread_create(&thread_id[i], NULL, square, (void *)&vetor_args[i])) {
93
                        printf("Erro de thread");
94
                }
95
        }
96
        
97
        // Sincronização das threads após execução
98
        for (int i = 0; i < NUM_THREADS; i++) {
99
        
100
                // Se não conseguiu encerrar thread, joga erro
101
                if (pthread_join(thread_id[i], NULL)) {
102
                        printf("Erro de thread");
103
                }
104
        }
105
        
106
        // Checamos se a operação está correta
107
        int correto = 1;
108
        for (int i = 0; i < (tam_vetor); i++) {
109
                if (vetor_quadrado[i] != (copia_vetor[i] * copia_vetor[i])) {
110
                printf("%d != %d*%d \n", vetor_quadrado[i], copia_vetor[i], copia_vetor[i]);
111
                        correto = 0;
112
                }
113
        }
114
        
115
        if (correto == 1) {
116
                printf("Operação correta!\n");
117
        }
118
        else {
119
                printf("Operação incorreta!\n");
120
        }
121
        
122
        // Fim do programa
123
        return 0;
124        //int copia_vetor[tam_vetor];
78
        
79
        // Preenchemos a cópia do vetor
80
        for (int i = 0; i < (tam_vetor); i++) {
81
                copia_vetor[i] = vetor_quadrado[i];
82
        }
83
        
84
        // Criamos os argumentos necessários para cada thread
85
        thread_args vetor_args[NUM_THREADS] = {{0, tam_vetor/2, vetor_quadrado}, {tam_vetor/2, tam_vetor, vetor_quadrado}};
86
        
87
​
88
        // Criação das threads propriamente ditas
89
        for (int i = 0; i < NUM_THREADS; i++) {
90
        
91
                // Se não conseguiu criar thread, joga erro
92
                if (pthread_create(&thread_id[i], NULL, square, (void *)&vetor_args[i])) {
93
                        printf("Erro de thread");
94
                }
95
        }
96
        
97
        // Sincronização das threads após execução
98
        for (int i = 0; i < NUM_THREADS; i++) {
99
        
100
                // Se não conseguiu encerrar thread, joga erro
101
                if (pthread_join(thread_id[i], NULL)) {
102
                        printf("Erro de thread");
103
                }
104
        }
105
        
106
        // Checamos se a operação está correta
107
        int correto = 1;
108
        for (int i = 0; i < (tam_vetor); i++) {
109
                if (vetor_quadrado[i] != (copia_vetor[i] * copia_vetor[i])) {
110
                printf("%d != %d*%d \n", vetor_quadrado[i], copia_vetor[i], copia_vetor[i]);
111
                        correto = 0;
112
                }
113
        }
114
        
115
        if (correto == 1) {
116
                printf("Operação correta!\n");
117
        }
118
        else {
119
                printf("Operação incorreta!\n");
120
        }
121
        
122
        // Fim do programa
123
        return 0;
124
}
125

}
125
i+1;
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
	thread_args vetor_args[NUM_THREADS] = {{vetor_quadrado, tam_vetor, 1}, {vetor_quadrado, tam_vetor, 2}};
	

	// Criação das threads propriamente ditas
	for (int i = 0; i < NUM_THREADS; i++) {
	
		// Se não conseguiu criar thread, joga erro
		if (pthread_create(&thread_id[i], NULL, square, (void *)&vetor_args[i])) {
			printf("Erro de thread");
		}
	}

	// Execução da thread principal
	for (int i = 0; i < tam_vetor; i += NUM_THREADS + 1) {
		vetor_quadrado[i] *= vetor_quadrado[i];
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
