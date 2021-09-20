#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct vertice {
    struct vertice *adjacentes;
    int indice;
} vertice;

int main(){

    char *linha = NULL;
    size_t tamLinha = 0;
    int vertices, arestas;
    scanf("%d %d\n", &vertices, &arestas);

    vertice **grafo = (vertice**)malloc(vertices * sizeof(vertice*));
    int *cont_entradas = (int*)malloc(vertices * sizeof(int));

    // Cria a estrutura do grafo

    for (int i = 0; i < vertices; i++) {
        grafo[i] = NULL;
        cont_entradas[i] = 0;
    }

    for (int i = 0; i < vertices; i++) {
        getline(&linha,&tamLinha,stdin);
        int pos = 0;
        int numLido = 0;
        int numChars;

        while (sscanf(&linha[pos], " %d %n", &numLido, &numChars) > 0) {
            pos += numChars;
            if (strlen(linha) != 0){
                vertice *ponteiro_lista;
                
                //Insere na lista dos adjacentes
                ponteiro_lista = grafo[i];
                grafo[i] = (vertice*)malloc(sizeof(vertice));
                grafo[i]->indice = numLido;
                grafo[i]->adjacentes = ponteiro_lista;
                // Adiciona 1 no contador de entradas do número lido
                cont_entradas[numLido-1]++;
            }
        }
    }

    int *ordem = (int*)malloc(vertices * sizeof(int));
    int pos_ordem = 0;

    for (int i = 0; i < vertices; i++) {
        if (cont_entradas[i] == 0) {
            ordem[pos_ordem++] = i + 1;
        }
    }

    for (int i = 0; i < pos_ordem; i++) {
        vertice *aux = grafo[ordem[i]-1];
        while (aux != NULL) {
            cont_entradas[aux->indice-1]--;
            if (cont_entradas[aux->indice-1] == 0) {
                ordem[pos_ordem++] = aux->indice;
            }
            vertice *lixo = aux;
            aux = aux->adjacentes;
            free(lixo);
        }
        grafo[ordem[i]-1] = NULL;
    }

    if (pos_ordem == vertices) {
        for (int i = 0; i < vertices-1; i++) {
            printf("%d ", ordem[i]);
        }
        printf("%d\n", ordem[vertices-1]);
    }
    else {
        for (int i = 0; i < vertices; i++) {
            if (grafo[i] != NULL) {
                vertice *aux = grafo[i];
                while (aux->adjacentes != NULL) {
                    vertice *lixo = aux;
                    aux = aux->adjacentes;
                    free(lixo);
                }
            }
        }
    }
    free(grafo);
    free(ordem);
    free(linha);
    free(cont_entradas);
}