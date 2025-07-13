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

// Teste sequencial para speedup
double testeSequencial(int tamanho) {
    int *arr = (int*)malloc(tamanho * sizeof(int));
    for (int i = 0; i < tamanho; i++) {
        arr[i] = tamanho - i;
    }
    
    clock_t inicio = clock();
    mergeSort(arr, 0, tamanho - 1);
    clock_t fim = clock();
    
    double tempo = ((double)(fim - inicio)) / CLOCKS_PER_SEC;
    free(arr);
    return tempo;
}

int main(int argc, char *argv[]) {
    int rank, num_processos;
    int tamanho_total = 2097152;  // Tamanho otimizado
    
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
    
    // MEDIR TEMPO PARALELO
    double tempo_paralelo_inicio = MPI_Wtime();
    
    // PROCESSO 0: Criar array original
    if (rank == 0) {
        printf("\n=== MERGESORT PARALELO ===\n");
        printf("Numero de processos: %d\n", num_processos);
        printf("Tamanho total: %d elementos\n", tamanho_total);
        printf("Tamanho por processo: %d\n\n", tamanho_parte);
        
        array_completo = (int*)malloc(tamanho_total * sizeof(int));
        if (!array_completo) {
            printf("ERRO: Falha na alocacao do array completo\n");
            MPI_Abort(MPI_COMM_WORLD, 1);
        }
        
        // Preencher array sem mostrar
        for (int i = 0; i < tamanho_total; i++) {
            array_completo[i] = tamanho_total - i;
        }
    }
    
    // Alocar memória para a parte local
    int *minha_parte = (int*)malloc(tamanho_parte * sizeof(int));
    if (!minha_parte) {
        printf("ERRO: Processo %d falhou na alocacao de memoria\n", rank);
        MPI_Abort(MPI_COMM_WORLD, 1);
    }
    
    // Distribuir dados
    double tempo_scatter_inicio = MPI_Wtime();
    MPI_Scatter(array_completo, tamanho_parte, MPI_INT, 
                minha_parte, tamanho_parte, MPI_INT, 
                0, MPI_COMM_WORLD);
    double tempo_scatter = MPI_Wtime() - tempo_scatter_inicio;
    
    if (rank == 0) {
        printf("Iniciando ordenacao paralela...\n");
    }
    
    // Sincronizar
    MPI_Barrier(MPI_COMM_WORLD);
    
    // Cada processo ordena sua parte
    double tempo_ordenacao_inicio = MPI_Wtime();
    mergeSort(minha_parte, 0, tamanho_parte - 1);
    double tempo_ordenacao = MPI_Wtime() - tempo_ordenacao_inicio;
    
    if (rank == 0) {
        printf("Ordenacao local concluida...\n");
        printf("Coletando resultados...\n");
    }
    
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
    double tempo_gather_inicio = MPI_Wtime();
    MPI_Gather(minha_parte, tamanho_parte, MPI_INT,
               array_completo, tamanho_parte, MPI_INT,
               0, MPI_COMM_WORLD);
    double tempo_gather = MPI_Wtime() - tempo_gather_inicio;
    
    // PROCESSO 0: Merge final
    double tempo_merge_final = 0;
    if (rank == 0) {
        printf("Executando merge final...\n");
        
        double merge_inicio = MPI_Wtime();
        
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
        
        tempo_merge_final = MPI_Wtime() - merge_inicio;
        printf("Ordenacao completa!\n");
    }
    
    // CALCULAR TEMPO PARALELO TOTAL
    double tempo_paralelo_total = MPI_Wtime() - tempo_paralelo_inicio;
    
    // Coletar tempos máximos de todos os processos
    double max_scatter, max_ordenacao, max_gather;
    MPI_Reduce(&tempo_scatter, &max_scatter, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
    MPI_Reduce(&tempo_ordenacao, &max_ordenacao, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
    MPI_Reduce(&tempo_gather, &max_gather, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
    
    // SPEEDUP E GARGALOS (só processo 0)
    if (rank == 0) {
        printf("\n--- SPEEDUP ---\n");
        printf("Calculando tempo sequencial...\n");
        double tempo_sequencial = testeSequencial(tamanho_total);
        double speedup = tempo_sequencial / tempo_paralelo_total;
        
        printf("Tempo Sequencial: %.6f segundos\n", tempo_sequencial);
        printf("Tempo Paralelo:   %.6f segundos\n", tempo_paralelo_total);
        printf("Speedup:          %.2fx\n", speedup);
        
        if (speedup > 1.0) {
            printf("Resultado: Paralelo e %.1fx MAIS RAPIDO!\n", speedup);
        } else {
            printf("Resultado: Sequencial e mais rapido\n");
        }
        
        printf("\n--- ANALISE DE GARGALOS ---\n");
        printf("Scatter (distribuicao):   %.6f segundos (%.1f%%)\n", 
               max_scatter, (max_scatter/tempo_paralelo_total)*100);
        printf("Ordenacao (paralela):     %.6f segundos (%.1f%%)\n", 
               max_ordenacao, (max_ordenacao/tempo_paralelo_total)*100);
        printf("Gather (coleta):          %.6f segundos (%.1f%%)\n", 
               max_gather, (max_gather/tempo_paralelo_total)*100);
        printf("Merge final (sequencial): %.6f segundos (%.1f%%)\n", 
               tempo_merge_final, (tempo_merge_final/tempo_paralelo_total)*100);
        
        printf("\n--- CONCLUSOES ---\n");

        if (max_ordenacao > tempo_merge_final) {
            printf("+ Paralelismo funcionando bem\n");
        } else {
            printf("- Merge final e o maior gargalo\n");
        }
        
        if ((max_scatter + max_gather) > max_ordenacao) {
            printf("- Comunicacao MPI e gargalo significativo\n\n");
        } else {
            printf("+ Comunicacao MPI eficiente\n\n");
        }
        
        free(array_completo);
    }
    
    // Liberar memória local
    free(minha_parte);
    
    // Sincronização final
    MPI_Barrier(MPI_COMM_WORLD);
    
    MPI_Finalize();
    return 0;
}