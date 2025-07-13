#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h> // Biblioteca para programação paralela com MPI
#include <string.h>

// ============================================================================
// FUNÇÃO JUNTAR - Mescla duas partes ordenadas de um array
// Parâmetros: arr[] - array a ser ordenado
//            inicio - índice inicial da primeira parte
//            meio - índice final da primeira parte
//            fim - índice final da segunda parte
// ============================================================================
void juntar(int arr[], int inicio, int meio, int fim)
{

    // Verificação de segurança: validar índices
    if (inicio >= fim || meio < inicio || meio >= fim)
        return;

    int i, j, k;
    // Calcular tamanhos das duas partes a serem mescladas
    int tamanho1 = meio - inicio + 1; // Tamanho da parte esquerda
    int tamanho2 = fim - meio;        // Tamanho da parte direita

    // Verificar se os tamanhos são válidos
    if (tamanho1 <= 0 || tamanho2 <= 0)
        return;

    // Alocar arrays temporários para as duas partes
    int *esquerda = (int *)malloc(tamanho1 * sizeof(int)); // Array temporário esquerdo
    int *direita = (int *)malloc(tamanho2 * sizeof(int));  // Array temporário direito

    // Verificar se a alocação foi bem-sucedida
    if (!esquerda || !direita)
    {

        printf("Erro: falha na alocacao de memoria\n");

        if (esquerda)
            free(esquerda);
        if (direita)
            free(direita);

        return;
    }

    // Copiar dados para o array temporário esquerdo
    for (i = 0; i < tamanho1; i++)
        esquerda[i] = arr[inicio + i];

    // Copiar dados para o array temporário direito
    for (j = 0; j < tamanho2; j++)
        direita[j] = arr[meio + 1 + j];

    // Inicializar índices para merge
    i = 0;      // Índice para array esquerdo
    j = 0;      // Índice para array direito
    k = inicio; // Índice para array original

    // Mesclar as duas partes em ordem crescente
    while (i < tamanho1 && j < tamanho2)
    {

        if (esquerda[i] <= direita[j])
        {
            arr[k] = esquerda[i]; // Elemento da esquerda é menor ou igual
            i++;
        }
        else
        {
            arr[k] = direita[j]; // Elemento da direita é menor
            j++;
        }

        k++;
    }

    // Copiar elementos restantes da parte esquerda (se houver)
    while (i < tamanho1)
    {

        arr[k] = esquerda[i];
        i++;
        k++;
    }

    // Copiar elementos restantes da parte direita (se houver)
    while (j < tamanho2)
    {

        arr[k] = direita[j];
        j++;
        k++;
    }

    // Liberar memória dos arrays temporários
    free(esquerda);
    free(direita);
}

// ============================================================================
// FUNÇÃO MERGESORT - Implementa o algoritmo MergeSort recursivamente
// Estratégia: Dividir para conquistar
// Parâmetros: arr[] - array a ser ordenado
//            inicio - índice inicial da parte a ordenar
//            fim - índice final da parte a ordenar
// ============================================================================
void mergeSort(int arr[], int inicio, int fim)
{

    // Condição de parada: se há mais de um elemento para ordenar
    if (inicio < fim && arr != NULL)
    {

        // Calcular ponto médio (evita overflow)
        int meio = inicio + (fim - inicio) / 2;

        // DIVIDIR: Ordenar recursivamente a primeira metade
        mergeSort(arr, inicio, meio);

        // DIVIDIR: Ordenar recursivamente a segunda metade
        mergeSort(arr, meio + 1, fim);

        // CONQUISTAR: Mesclar as duas metades ordenadas
        juntar(arr, inicio, meio, fim);
    }
}

// ============================================================================
// TESTE SEQUENCIAL - Executa MergeSort sequencial para comparação de speedup
// Parâmetro: tamanho - número de elementos do array
// Retorna: tempo de execução em segundos
// ============================================================================
double testeSequencial(int tamanho)
{

    // Alocar array para teste sequencial
    int *arr = (int *)malloc(tamanho * sizeof(int));

    // Preencher array com valores decrescentes (pior caso para ordenação)
    for (int i = 0; i < tamanho; i++)
    {
        arr[i] = tamanho - i;
    }

    // Medir tempo de execução do algoritmo sequencial
    clock_t inicio = clock();
    mergeSort(arr, 0, tamanho - 1); // Executar MergeSort sequencial
    clock_t fim = clock();

    // Converter tempo para segundos
    double tempo = ((double)(fim - inicio)) / CLOCKS_PER_SEC;

    // Liberar memória e retornar tempo
    free(arr);
    return tempo;
}

// ============================================================================
// FUNÇÃO PRINCIPAL - Implementa MergeSort Paralelo com MPI
// ============================================================================
int main(int argc, char *argv[])
{

    int rank, num_processos;
    int tamanho_total = 2097152; // 2^21 elementos (aproximadamente 2 milhões)

    // ========================================================================
    // INICIALIZAÇÃO MPI
    // ========================================================================
    MPI_Init(&argc, &argv);                        // Inicializar ambiente MPI
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);          // Obter ID do processo atual
    MPI_Comm_size(MPI_COMM_WORLD, &num_processos); // Obter número total de processos

    // ========================================================================
    // VALIDAÇÃO DE ENTRADA
    // ========================================================================
    // Verificar se o array pode ser dividido igualmente entre os processos
    if (tamanho_total % num_processos != 0)
    {

        if (rank == 0)
        {
            printf("ERRO: Tamanho do array (%d) deve ser divisivel pelo numero de processos (%d)\n",
                   tamanho_total, num_processos);
        }

        MPI_Finalize();

        return 1;
    }

    // ========================================================================
    // INICIALIZAÇÃO DE VARIÁVEIS
    // ========================================================================
    int *array_completo = NULL;                        // Array completo (só processo 0)
    int tamanho_parte = tamanho_total / num_processos; // Tamanho da parte de cada processo

    // Iniciar medição do tempo total de execução paralela
    double tempo_paralelo_inicio = MPI_Wtime();

    // ========================================================================
    // PROCESSO 0: CRIAÇÃO E INICIALIZAÇÃO DO ARRAY ORIGINAL
    // ========================================================================
    if (rank == 0)
    {

        // Exibir informações sobre a execução
        printf("\n=== MERGESORT PARALELO ===\n");
        printf("Numero de processos: %d\n", num_processos);
        printf("Tamanho total: %d elementos\n", tamanho_total);
        printf("Tamanho por processo: %d\n\n", tamanho_parte);

        // Alocar memória para o array completo
        array_completo = (int *)malloc(tamanho_total * sizeof(int));

        if (!array_completo)
        {
            printf("ERRO: Falha na alocacao do array completo\n");
            MPI_Abort(MPI_COMM_WORLD, 1);
        }

        // Preencher array com valores decrescentes (pior caso)
        for (int i = 0; i < tamanho_total; i++)
        {
            array_completo[i] = tamanho_total - i;
        }
    }

    // ========================================================================
    // TODOS OS PROCESSOS: ALOCAÇÃO DE MEMÓRIA LOCAL
    // ========================================================================
    // Cada processo aloca memória para sua parte do array
    int *minha_parte = (int *)malloc(tamanho_parte * sizeof(int));

    if (!minha_parte)
    {
        printf("ERRO: Processo %d falhou na alocacao de memoria\n", rank);
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    // ========================================================================
    // GARGALO 1: DISTRIBUIÇÃO DOS DADOS (MPI_SCATTER)
    // ========================================================================
    // Medir tempo de distribuição dos dados
    double tempo_scatter_inicio = MPI_Wtime();

    // Distribuir partes do array para todos os processos
    MPI_Scatter(array_completo, tamanho_parte, MPI_INT, // Fonte: array completo
                minha_parte, tamanho_parte, MPI_INT,    // Destino: parte local
                0, MPI_COMM_WORLD);                     // Processo raiz: 0

    double tempo_scatter = MPI_Wtime() - tempo_scatter_inicio;

    if (rank == 0)
    {
        printf("Iniciando ordenacao paralela...\n");
    }

    // Sincronizar todos os processos antes da ordenação
    MPI_Barrier(MPI_COMM_WORLD);

    // ========================================================================
    // PARALELISMO REAL: ORDENAÇÃO LOCAL EM CADA PROCESSO
    // ========================================================================
    // Medir tempo de ordenação local
    double tempo_ordenacao_inicio = MPI_Wtime();

    // Cada processo ordena sua parte independentemente
    mergeSort(minha_parte, 0, tamanho_parte - 1);

    double tempo_ordenacao = MPI_Wtime() - tempo_ordenacao_inicio;

    if (rank == 0)
    {
        printf("Ordenacao local concluida...\n");
        printf("Coletando resultados...\n");
    }

    // Sincronizar todos os processos antes da coleta
    MPI_Barrier(MPI_COMM_WORLD);

    // ========================================================================
    // PROCESSO 0: PREPARAÇÃO PARA COLETA DE RESULTADOS
    // ========================================================================
    // Realocar memória se necessário (precaução)
    if (rank == 0 && !array_completo)
    {

        array_completo = (int *)malloc(tamanho_total * sizeof(int));

        if (!array_completo)
        {
            printf("ERRO: Falha na alocacao para gather\n");
            MPI_Abort(MPI_COMM_WORLD, 1);
        }
    }

    // ========================================================================
    // GARGALO 2: COLETA DOS RESULTADOS (MPI_GATHER)
    // ========================================================================
    // Medir tempo de coleta dos dados ordenados
    double tempo_gather_inicio = MPI_Wtime();

    // Coletar todas as partes ordenadas no processo 0
    MPI_Gather(minha_parte, tamanho_parte, MPI_INT,    // Fonte: parte local
               array_completo, tamanho_parte, MPI_INT, // Destino: array completo
               0, MPI_COMM_WORLD);                     // Processo destino: 0

    double tempo_gather = MPI_Wtime() - tempo_gather_inicio;

    // ========================================================================
    // GARGALO 3: MERGE FINAL SEQUENCIAL (APENAS PROCESSO 0)
    // ========================================================================
    double tempo_merge_final = 0;

    if (rank == 0)
    {

        printf("Executando merge final...\n");

        // Medir tempo do merge final
        double merge_inicio = MPI_Wtime();

        // Implementar merge bottom-up das partes ordenadas
        int tamanho_merge = tamanho_parte; // Começar com tamanho de cada parte

        // Mesclar partes progressivamente (2, 4, 8, ... partes por vez)
        while (tamanho_merge < tamanho_total)
        {
            // Para cada grupo de partes
            for (int i = 0; i < tamanho_total; i += 2 * tamanho_merge)
            {
                int inicio = i;
                int meio = i + tamanho_merge - 1;
                int fim = i + 2 * tamanho_merge - 1;

                // Ajustar limites para não ultrapassar o array
                if (meio >= tamanho_total)
                    meio = tamanho_total - 1;
                if (fim >= tamanho_total)
                    fim = tamanho_total - 1;

                // Mesclar apenas se há duas partes válidas
                if (inicio < meio && meio < fim)
                {
                    juntar(array_completo, inicio, meio, fim);
                }
            }
            tamanho_merge *= 2; // Dobrar tamanho para próxima iteração
        }

        tempo_merge_final = MPI_Wtime() - merge_inicio;
        printf("Ordenacao completa!\n");
    }

    // ========================================================================
    // MEDIÇÃO DO TEMPO TOTAL E COLETA DE MÉTRICAS
    // ========================================================================
    // Calcular tempo total de execução paralela
    double tempo_paralelo_total = MPI_Wtime() - tempo_paralelo_inicio;

    // Coletar tempos máximos de cada fase (pior caso entre todos os processos)
    double max_scatter, max_ordenacao, max_gather;
    MPI_Reduce(&tempo_scatter, &max_scatter, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
    MPI_Reduce(&tempo_ordenacao, &max_ordenacao, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
    MPI_Reduce(&tempo_gather, &max_gather, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

    // ========================================================================
    // ANÁLISE DE PERFORMANCE E GARGALOS (APENAS PROCESSO 0)
    // ========================================================================
    if (rank == 0)
    {

        // Calcular speedup comparando com versão sequencial
        printf("\n--- SPEEDUP ---\n");
        double tempo_sequencial = testeSequencial(tamanho_total);
        double speedup = tempo_sequencial / tempo_paralelo_total;

        // Exibir métricas de speedup
        printf("Tempo Sequencial: %.6f segundos\n", tempo_sequencial);
        printf("Tempo Paralelo:   %.6f segundos\n", tempo_paralelo_total);
        printf("Speedup:          %.2fx\n", speedup);

        // Interpretar resultado do speedup
        if (speedup > 1.0)
        {
            printf("Resultado:        Paralelo e %.1fx MAIS RAPIDO!\n", speedup);
        }
        else
        {
            printf("Resultado:        Sequencial e mais rapido\n");
        }

        // Análise detalhada dos gargalos
        printf("\n--- GARGALOS ---\n");
        printf("Scatter (distribuicao):   %.6f segundos (%.1f%%)\n",
               max_scatter, (max_scatter / tempo_paralelo_total) * 100);
        printf("Ordenacao (paralela):     %.6f segundos (%.1f%%)\n",
               max_ordenacao, (max_ordenacao / tempo_paralelo_total) * 100);
        printf("Gather (coleta):          %.6f segundos (%.1f%%)\n",
               max_gather, (max_gather / tempo_paralelo_total) * 100);
        printf("Merge final (sequencial): %.6f segundos (%.1f%%)\n",
               tempo_merge_final, (tempo_merge_final / tempo_paralelo_total) * 100);

        // Conclusões automáticas baseadas nas métricas
        printf("\n--- CONCLUSOES ---\n");

        // Avaliar eficácia do paralelismo
        if (max_ordenacao > tempo_merge_final)
        {
            printf("Paralelismo eficaz: trabalho util supera gargalo sequencial\n");
        }
        else
        {
            printf("Paralelismo limitado: merge final domina o tempo total\n");
        }

        // Avaliar overhead de comunicação MPI
        if ((max_scatter + max_gather) > max_ordenacao)
        {
            printf("Comunicacao MPI ineficiente: overhead supera trabalho paralelo\n\n");
        }
        else
        {
            printf("Comunicacao MPI otimizada: baixo overhead de transferencia\n\n");
        }

        // Liberar memória do array completo
        free(array_completo);
    }

    // ========================================================================
    // FINALIZAÇÃO E LIMPEZA
    // ========================================================================
    // Liberar memória local de cada processo
    free(minha_parte);

    // Sincronizar todos os processos antes da finalização
    MPI_Barrier(MPI_COMM_WORLD);

    // Finalizar ambiente MPI
    MPI_Finalize();

    return 0;
}