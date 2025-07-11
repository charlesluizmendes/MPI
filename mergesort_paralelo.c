#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>
#include <string.h>

// Função juntar - mais segura
void juntar(int arr[], int inicio, int meio, int fim) {
    if (inicio >= fim || meio < inicio || meio >= fim) return;
    
    int i, j, k;
    int tamanho1 = meio - inicio + 1;
    int tamanho2 = fim - meio;
    
    // Verificar se os tamanhos são válidos
    if (tamanho1 <= 0 || tamanho2 <= 0) return;
    
    int *esquerda = (int*)malloc(tamanho1 * sizeof(int));
    int *direita = (int*)malloc(tamanho2 * sizeof(int));
    
    if (!esquerda || !direita) {
        printf("Erro: falha na alocacao de memoria\n");
        if (esquerda) free(esquerda);
        if (direita) free(direita);
        return;
    }
    
    for (i = 0; i < tamanho1; i++)
        esquerda[i] = arr[inicio + i];
    
    for (j = 0; j < tamanho2; j++)
        direita[j] = arr[meio + 1 + j];
    
    i = 0; j = 0; k = inicio;
    
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
    
    while (i < tamanho1) {
        arr[k] = esquerda[i];
        i++;
        k++;
    }
    
    while (j < tamanho2) {
        arr[k] = direita[j];
        j++;
        k++;
    }
    
    free(esquerda);
    free(direita);
}

// Função mergeSort - mais segura
void mergeSort(int arr[], int inicio, int fim) {
    if (inicio < fim && arr != NULL) {
        int meio = inicio + (fim - inicio) / 2;
        mergeSort(arr, inicio, meio);
        mergeSort(arr, meio + 1, fim);
        juntar(arr, inicio, meio, fim);
    }
}

int main(int argc, char *argv[]) {
    int rank, num_processos;
    int tamanho_total = 16;
    
    // Inicializar MPI
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_processos);
    
    // Verificar se o número de processos é válido
    if (tamanho_total % num_processos != 0) {
        if (rank == 0) {
            printf("ERRO: Tamanho do array (%d) deve ser divisivel pelo numero de processos (%d)\n", 
                   tamanho_total, num_processos);
        }
        MPI_Finalize();
        return 1;
    }
    
    int *array_completo = NULL;
    int tamanho_parte = tamanho_total / num_processos;
    
    // PROCESSO 0: Criar array original
    if (rank == 0) {
        printf("=== MERGESORT PARALELO ===\n");
        printf("Numero de processos: %d\n", num_processos);
        printf("Tamanho por processo: %d\n\n", tamanho_parte);
        
        array_completo = (int*)malloc(tamanho_total * sizeof(int));
        if (!array_completo) {
            printf("ERRO: Falha na alocacao do array completo\n");
            MPI_Abort(MPI_COMM_WORLD, 1);
        }
        
        printf("Array original: ");
        for (int i = 0; i < tamanho_total; i++) {
            array_completo[i] = tamanho_total - i;
            printf("%d ", array_completo[i]);
        }
        printf("\n\n");
    }
    
    // Alocar memória para a parte local
    int *minha_parte = (int*)malloc(tamanho_parte * sizeof(int));
    if (!minha_parte) {
        printf("ERRO: Processo %d falhou na alocacao de memoria\n", rank);
        MPI_Abort(MPI_COMM_WORLD, 1);
    }
    
    // Distribuir dados
    MPI_Scatter(array_completo, tamanho_parte, MPI_INT, 
                minha_parte, tamanho_parte, MPI_INT, 
                0, MPI_COMM_WORLD);
    
    // Cada processo mostra o que recebeu
    printf("Processo %d recebeu: ", rank);
    for (int i = 0; i < tamanho_parte; i++) {
        printf("%d ", minha_parte[i]);
    }
    printf("\n");
    fflush(stdout);
    
    // Sincronizar
    MPI_Barrier(MPI_COMM_WORLD);
    
    if (rank == 0) {
        printf("\n--- ORDENANDO LOCALMENTE ---\n");
        fflush(stdout);
    }
    MPI_Barrier(MPI_COMM_WORLD);
    
    // Cada processo ordena sua parte
    mergeSort(minha_parte, 0, tamanho_parte - 1);
    
    // Cada processo mostra o que ordenou
    printf("Processo %d ordenou: ", rank);
    for (int i = 0; i < tamanho_parte; i++) {
        printf("%d ", minha_parte[i]);
    }
    printf("\n");
    fflush(stdout);
    
    // Sincronizar antes de juntar
    MPI_Barrier(MPI_COMM_WORLD);
    
    // Alocar memória para receber resultados (só processo 0)
    if (rank == 0 && !array_completo) {
        array_completo = (int*)malloc(tamanho_total * sizeof(int));
        if (!array_completo) {
            printf("ERRO: Falha na alocacao para gather\n");
            MPI_Abort(MPI_COMM_WORLD, 1);
        }
    }
    
    // Juntar resultados
    MPI_Gather(minha_parte, tamanho_parte, MPI_INT,
               array_completo, tamanho_parte, MPI_INT,
               0, MPI_COMM_WORLD);
    
    // PROCESSO 0: Merge final
    if (rank == 0) {
        printf("\n--- MERGE FINAL ---\n");
        printf("Antes do merge: ");
        for (int i = 0; i < tamanho_total; i++) {
            printf("%d ", array_completo[i]);
        }
        printf("\n");
        
        // Merge das partes ordenadas
        int tamanho_merge = tamanho_parte;
        while (tamanho_merge < tamanho_total) {
            for (int i = 0; i < tamanho_total; i += 2 * tamanho_merge) {
                int inicio = i;
                int meio = i + tamanho_merge - 1;
                int fim = i + 2 * tamanho_merge - 1;
                
                if (meio >= tamanho_total) meio = tamanho_total - 1;
                if (fim >= tamanho_total) fim = tamanho_total - 1;
                
                if (inicio < meio && meio < fim) {
                    juntar(array_completo, inicio, meio, fim);
                }
            }
            tamanho_merge *= 2;
        }
        
        printf("Array final: ");
        for (int i = 0; i < tamanho_total; i++) {
            printf("%d ", array_completo[i]);
        }
        printf("\n\n=== FIM ===\n");
        
        free(array_completo);
    }
    
    // Liberar memória local
    free(minha_parte);
    
    // Sincronização final
    MPI_Barrier(MPI_COMM_WORLD);
    
    MPI_Finalize();
    return 0;
}