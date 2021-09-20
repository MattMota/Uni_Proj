#include <stdio.h>
#include <stdlib.h>

typedef struct no {
    struct no *esq;
    struct no *dir;
    int altura;
    int chave;
} no;

no* criaNo(int chave){
	no *n = (no*)malloc(sizeof(no));

	// Se não conseguir alocar o espaço pro nó,
	// retorna NULL
	if (n == NULL) {
		return n;
	}

	// Se conseguir alocar o espaço pro nó,
	// inicializa ele
	n -> esq = NULL;
	n -> dir = NULL;
	n -> altura = 1;
	n -> chave = chave;
	return n;
}

// Imprime as chaves de todos os nós em pré-ordem
void printPreOrdem(no *n) {
	// Imprime recursivamente as chaves dos nós até que
	// toda a árvore tenha sido percorrida
	if (n != NULL) {
		printf("%d(%d) ", n -> chave, n -> altura);
		printPreOrdem(n -> esq);
		printPreOrdem(n -> dir);
	}
}

// Apaga os nós da árvore em pré-ordem
void apagaArvore (no **raiz) {
	if (*raiz != NULL) {
		apagaArvore(&((*raiz) -> esq));
		apagaArvore(&((*raiz) -> dir));
		free(*raiz);
	}
}

// Calcula o fator de balanceamento do nó passado
int fatorBalanco(no *n) {
	// Calcula a diferença entre as alturas
	// das duas subárvores do nó n
	if ((n -> dir != NULL) && (n -> esq != NULL)) {
		return (n -> dir -> altura) - (n -> esq -> altura);	
	}
	else if (n -> dir != NULL) {
		return (n -> dir -> altura);
	}
	else if (n -> esq != NULL) {
		return -(n -> esq -> altura);	
	}
	else {
		return 0;	
	}
}

// Calcula a atura de um nó baseado em seus filhos
int calculaAltura(no *atual) {
	// Se tem filhos nos dois lados
	if ((atual -> dir != NULL) && (atual -> esq != NULL)) {
		if ((atual -> dir -> altura) > (atual -> esq -> altura)) {
			return (atual -> dir -> altura) + 1;
		}
		else {
			return (atual -> esq -> altura) + 1;
		}
	}
	// Se só tem filho na direita
	else if (atual -> dir != NULL) {
		return (atual -> dir -> altura) + 1;
	}
	// Se só tem filho na esquerda
	else if (atual -> esq != NULL) {
		return (atual -> esq -> altura) + 1;
	}
	// Se não tem filhos, o nó é folha e a altura é 1
	return 1;
}

// Faz a rotação simples à esquerda do nó passado
void rotacaoEsq(no **n) {
	// A raiz é guardada num auxiliar
	no *aux = *n;

	// O filho da direita vira a nova raiz
	*n = (*n) -> dir;

	// O filho à esquerda da nova raiz se torna filho à direita da antiga raiz
	aux -> dir = (*n) -> esq;

	// A antiga raiz vira filha da esquerda da nova raiz
	(*n) -> esq = aux;

	// Recalculando as alturas
	aux -> altura = calculaAltura(aux);
	(*n) -> altura = calculaAltura(*n);
}

// Faz a rotação simples à direita do nó passado
void rotacaoDir(no **n) {
	// A raiz é guardada num auxiliar
	no *aux = (*n);

	// O filho da esquerda vira a nova raiz
	(*n) = (*n) -> esq;

	// O filho à direita da nova raiz se torna filho à esquerda da antiga raiz
	aux -> esq = (*n) -> dir;

	// A antiga raiz vira filha da direita da nova raiz
	(*n) -> dir = aux;

	// Recalculando as alturas
	aux -> altura = calculaAltura(aux);
	(*n) -> altura = calculaAltura(*n);
}

// Faz a rotação dupla à esquerda do nó passado
void rotacaoDuplaEsq(no **n) {
	// Rotação à direita na sub-árvore da direita
	rotacaoDir(&((*n) -> dir));

	// Rotação à esquerda na árvore original
	rotacaoEsq(n);
}

// Faz a rotação dupla à direita	 do nó passado
void rotacaoDuplaDir(no **n) {
	// Rotação à esquerda na sub-árvore da esquerda
	rotacaoEsq(&((*n) -> esq));

	// Rotação à direita na árvore original
	rotacaoDir(n);
}

// Insere uma chave na árvore AVL navegando recursivamente
int insereNo(int chave, no **raiz) {
	// Se estivermos numa árvore vazia ou numa folha,
	// criamos o nó, que tomaremos como raiz
	if (*raiz == NULL) {
		*raiz = criaNo(chave);

		// Se a alocação falhou
		if (*raiz == NULL) {
			return 0;
		}

		// Se a alocação funcionou
		else {
			return 1;
		}
	}
	// Se a raiz já existe
	else {

		// Criamos uma variável para sabermos
		// se a inserção vai funcionar
		int res;

		// Se a chave inserida é menor que a chave da raiz
		if (chave < (*raiz) -> chave) {

			// Tenta inserir a chave na sub-árvore esquerda,
			// e guardamos se a inserção funcionou

			if ((res = insereNo(chave, &((*raiz) -> esq)))) {

				// Se funcionou, checa o fator de balanceamento

				// Se a árvore foi desbalanceada pela inserção
				// e agora a sub-árvore da esquerda é a maior
				if (fatorBalanco(*raiz) < -1) {

					// Checamos onde foi inserida a chave

					// Se foi inserida à esquerda
					if (chave < (*raiz) -> esq -> chave) {
						// Fazemos rotação simples à direita
						rotacaoDir(raiz);
					}

					// Se foi inserida à direita
					else {
						// Fazemos rotação dupla à direita
						rotacaoDuplaDir(raiz);
					}
				}
			}
		}

		// Caso contrário, se a chave inserida é maior
		// que a chave da raiz
		else if (chave > (*raiz) -> chave) {

			// Tenta inserir a chave na sub-árvore direita,
			// e guardamos se a inserção funcionou
			if ((res = insereNo(chave, &((*raiz) -> dir)))) {

				// Se funcionou, checa o fator de balanceamento

				// Se a árvore foi desbalanceada pela inserção
				// e agora a árvore da direita é a maior
				if (fatorBalanco(*raiz) > 1) {
					// Checamos onde foi inserida a chave

					// Se foi inserida à esquerda
					if (chave > (*raiz) -> dir -> chave) {
						// Fazemos rotação simples à esquerda
						rotacaoEsq(raiz);
					}

					// Se foi inserida à direita
					else {
						// Fazemos rotação dupla à esquerda
						rotacaoDuplaEsq(raiz);
					}
				}
			}
		}

		// Caso contrário, a chave já existe e
		// não precisamos inserir, vamos só avisar
		else{
			printf("Valor duplicado!\n");
			return 0;
		}

		
		(*raiz) -> altura = calculaAltura(*raiz);
		
		return res;
	}
}

int main(int argc, char const *argv[]) {
	int chave;
	char str[10];
	no *raiz = NULL;

	while (!feof(stdin)) {
		// Lê cada string até um espaço em branco qualquer
		scanf("%s", str);

		// Se o 1o caractere da linha for "p"
		if (str[0] == 'p') {
			// Faz a impressão das chaves da árvore AVL
			// em pré-ordem
			printPreOrdem(raiz);
			printf("\n");
		}
		//Se o 1o caractere da linha for "i"
		else if (str[0] == 'i') {
			// Lê a próxima chave
			scanf("%d", &chave);

			// Insere a chave na árvore AVL
			insereNo(chave, &raiz);
		}

		scanf(" ");
	}

	apagaArvore(&raiz);

	return 0;
}