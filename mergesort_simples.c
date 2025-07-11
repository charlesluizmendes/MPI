#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Função para juntar duas partes já ordenadas do array
void juntar(int arr[], int inicio, int meio, int fim) {
    int i, j, k;
    
    // Tamanhos das duas partes
    int tamanho1 = meio - inicio + 1;
    int tamanho2 = fim - meio;
    
    // Arrays temporários para as duas partes
    int *esquerda = (int*)malloc(tamanho1 * sizeof(int));
    int *direita = (int*)malloc(tamanho2 * sizeof(int));
    
    // Copiar dados para os arrays temporários
    for (i = 0; i < tamanho1; i++)
        esquerda[i] = arr[inicio + i];
    
    for (j = 0; j < tamanho2; j++)
        direita[j] = arr[meio + 1 + j];
    
    // Juntar os arrays temporários de volta no array original
    i = 0; // Índice do array esquerda
    j = 0; // Índice do array direita  
    k = inicio; // Índice do array original
    
    while (i < tamanho1 && j < tamanho2) {
        if (esquerda[i] <= direita[j]) {
            arr[k] = esquerda[i];
            i++;
        } else {
            arr[k] = direita[j];
            j++;
        }
        k++;
    }
    
    // Copiar elementos restantes da esquerda, se houver
    while (i < tamanho1) {
        arr[k] = esquerda[i];
        i++;
        k++;
    }
    
    // Copiar elementos restantes da direita, se houver
    while (j < tamanho2) {
        arr[k] = direita[j];
        j++;
        k++;
    }
    
    // Liberar memória dos arrays temporários
    free(esquerda);
    free(direita);
}

// Função principal do MergeSort
void mergeSort(int arr[], int inicio, int fim) {
    if (inicio < fim) {
        // Encontrar o ponto do meio
        int meio = inicio + (fim - inicio) / 2;
        
        // Ordenar primeira metade
        mergeSort(arr, inicio, meio);
        
        // Ordenar segunda metade
        mergeSort(arr, meio + 1, fim);
        
        // Juntar as duas metades ordenadas
        juntar(arr, inicio, meio, fim);
    }
}

// Função para imprimir o array
void imprimirArray(int arr[], int tamanho) {
    for (int i = 0; i < tamanho; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

// Função para preencher array com números aleatórios
void preencherArrayAleatorio(int arr[], int tamanho) {
    for (int i = 0; i < tamanho; i++) {
        arr[i] = rand() % 100; // Números de 0 a 99
    }
}

int main() {
    printf("=== MERGESORT SIMPLES ===\n\n");
    
    // Tamanho do array para teste
    int tamanho = 10;
    
    // Criar array
    int *arr = (int*)malloc(tamanho * sizeof(int));
    
    // Preencher com números aleatórios
    srand(time(NULL));
    preencherArrayAleatorio(arr, tamanho);
    
    printf("Array antes da ordenação:\n");
    imprimirArray(arr, tamanho);
    
    // Medir tempo de execução
    clock_t inicio = clock();
    mergeSort(arr, 0, tamanho - 1);
    clock_t fim = clock();
    
    printf("\nArray depois da ordenação:\n");
    imprimirArray(arr, tamanho);
    
    // Calcular e mostrar tempo
    double tempo = ((double)(fim - inicio)) / CLOCKS_PER_SEC;
    printf("\nTempo de execução: %.6f segundos\n", tempo);
    
    // Liberar memória
    free(arr);
    
    return 0;
}