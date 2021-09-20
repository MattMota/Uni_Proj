#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define TAM 512

int *cria_vet(int *size){
    int cont = 0;
    int num = 0;
    int tam = TAM * sizeof(int);
    int* vet = (int*)malloc(tam);
    while(scanf("%d", &num) != EOF){
        vet[cont++] = num;
        if(cont % TAM == 0){
            tam *= 2;
            vet = (int*)realloc(vet, tam);
            if (vet == NULL){
                break;
            }
        }
    }
    *size = cont;

    return vet;
}

void merge(int vet[], int inicio, int meio, int fim) {
    int in = inicio;
    int me = meio + 1;
    int *aux = (int*)malloc((fim - inicio + 1) * sizeof(int));
    int pos_aux = 0;
    while(in <= meio && me <= fim){
        if(vet[in] < vet[me]){
            aux[pos_aux] = vet[in];
            in++;
        }
        else{
            aux[pos_aux] = vet[me];
            me++;
        }
        pos_aux++;
    }

    while(in <= meio){
        aux[pos_aux] = vet[in];
        in++;
        pos_aux++;
    }

    while(me <= fim){
        aux[pos_aux] = vet[me];
        me++;
        pos_aux++;
    }

    for(pos_aux = inicio; pos_aux <= fim; pos_aux++) {
        vet[pos_aux] = aux[pos_aux - inicio];
    }

    free(aux);
}


void merge_sort(int vet[], int inicio, int fim){
    if (inicio < fim) {
        int meio = (inicio + fim)/2;
        merge_sort(vet, inicio, meio);
        merge_sort(vet, meio + 1, fim);
        merge(vet, inicio, meio, fim);
    }
}

int main(){
    int size = 0;
    int* vet = cria_vet(&size);

    if (vet != NULL)
    {

        merge_sort(vet, 0, size-1);

        for(int i = 0; i < size; i++){
            printf("%d\n", vet[i]);
        }
    }
    else{
        printf("Erro! Nao ha memoria disponivel.\n");
    }
    
}
