> mpiexec -n 2 ./mergesort_paralelo.exe

=== MERGESORT PARALELO ===
Numero de processos: 2
Tamanho total: 2097152 elementos
Tamanho por processo: 1048576

Iniciando ordenacao paralela...
Ordenacao local concluida...
Coletando resultados...
Executando merge final...
Ordenacao completa!

--- SPEEDUP ---
Calculando tempo sequencial...
Tempo Sequencial: 0.261000 segundos
Tempo Paralelo:   0.146119 segundos
Speedup:          1.79x
Resultado: Paralelo e 1.8x MAIS RAPIDO!

--- ANALISE DE GARGALOS ---
Scatter (distribuicao):   0.005034 segundos (3.4%)
Ordenacao (paralela):     0.132914 segundos (91.0%)
Gather (coleta):          0.000822 segundos (0.6%)
Merge final (sequencial): 0.007327 segundos (5.0%)

--- CONCLUSOES ---
+ Paralelismo funcionando bem
+ Comunicacao MPI eficiente

> mpiexec -n 4 ./mergesort_paralelo.exe

=== MERGESORT PARALELO ===
Numero de processos: 4
Tamanho total: 2097152 elementos
Tamanho por processo: 524288

Iniciando ordenacao paralela...
Ordenacao local concluida...
Coletando resultados...
Executando merge final...
Ordenacao completa!

--- SPEEDUP ---
Calculando tempo sequencial...
Tempo Sequencial: 0.265000 segundos
Tempo Paralelo:   0.089064 segundos
Speedup:          2.98x
Resultado: Paralelo e 3.0x MAIS RAPIDO!

--- ANALISE DE GARGALOS ---
Scatter (distribuicao):   0.006049 segundos (6.8%)
Ordenacao (paralela):     0.066819 segundos (75.0%)
Gather (coleta):          0.001712 segundos (1.9%)
Merge final (sequencial): 0.014422 segundos (16.2%)

--- CONCLUSOES ---
+ Paralelismo funcionando bem
+ Comunicacao MPI eficiente

> mpiexec -n 8 ./mergesort_paralelo.exe

=== MERGESORT PARALELO ===
Numero de processos: 8
Tamanho total: 2097152 elementos
Tamanho por processo: 262144

Iniciando ordenacao paralela...
Ordenacao local concluida...
Coletando resultados...
Executando merge final...
Ordenacao completa!

--- SPEEDUP ---
Calculando tempo sequencial...
Tempo Sequencial: 0.264000 segundos
Tempo Paralelo:   0.079612 segundos
Speedup:          3.32x
Resultado: Paralelo e 3.3x MAIS RAPIDO!

--- ANALISE DE GARGALOS ---
Scatter (distribuicao):   0.006567 segundos (8.2%)
Ordenacao (paralela):     0.048978 segundos (61.5%)
Gather (coleta):          0.002046 segundos (2.6%)
Merge final (sequencial): 0.021923 segundos (27.5%)

--- CONCLUSOES ---
+ Paralelismo funcionando bem
+ Comunicacao MPI eficiente

> mpiexec -n 16 ./mergesort_paralelo.exe

=== MERGESORT PARALELO ===
Numero de processos: 16
Tamanho total: 2097152 elementos
Tamanho por processo: 131072

Iniciando ordenacao paralela...
Ordenacao local concluida...
Coletando resultados...
Executando merge final...
Ordenacao completa!

--- SPEEDUP ---
Calculando tempo sequencial...
Tempo Sequencial: 0.282000 segundos
Tempo Paralelo:   0.071985 segundos
Speedup:          3.92x
Resultado: Paralelo e 3.9x MAIS RAPIDO!

--- ANALISE DE GARGALOS ---
Scatter (distribuicao):   0.007630 segundos (10.6%)
Ordenacao (paralela):     0.033841 segundos (47.0%)
Gather (coleta):          0.002454 segundos (3.4%)
Merge final (sequencial): 0.027903 segundos (38.8%)

--- CONCLUSOES ---
+ Paralelismo funcionando bem
+ Comunicacao MPI eficiente

> mpiexec -n 32 ./mergesort_paralelo.exe

=== MERGESORT PARALELO ===
Numero de processos: 32
Tamanho total: 2097152 elementos
Tamanho por processo: 65536

Iniciando ordenacao paralela...
Ordenacao local concluida...
Coletando resultados...
Executando merge final...
Ordenacao completa!

--- SPEEDUP ---
Calculando tempo sequencial...
Tempo Sequencial: 0.280000 segundos
Tempo Paralelo:   0.082824 segundos
Speedup:          3.38x
Resultado: Paralelo e 3.4x MAIS RAPIDO!

--- ANALISE DE GARGALOS ---
Scatter (distribuicao):   0.009288 segundos (11.2%)
Ordenacao (paralela):     0.031289 segundos (37.8%)
Gather (coleta):          0.003210 segundos (3.9%)
Merge final (sequencial): 0.034924 segundos (42.2%)

--- CONCLUSOES ---
- Merge final e o maior gargalo
+ Comunicacao MPI eficiente

> mpiexec -n 64 ./mergesort_paralelo.exe

=== MERGESORT PARALELO ===
Numero de processos: 64
Tamanho total: 2097152 elementos
Tamanho por processo: 32768

Iniciando ordenacao paralela...
Ordenacao local concluida...
Coletando resultados...
Executando merge final...
Ordenacao completa!

--- SPEEDUP ---
Calculando tempo sequencial...
Tempo Sequencial: 0.288000 segundos
Tempo Paralelo:   0.109202 segundos
Speedup:          2.64x
Resultado: Paralelo e 2.6x MAIS RAPIDO!

--- ANALISE DE GARGALOS ---
Scatter (distribuicao):   0.015187 segundos (13.9%)
Ordenacao (paralela):     0.017613 segundos (16.1%)
Gather (coleta):          0.007698 segundos (7.0%)
Merge final (sequencial): 0.044702 segundos (40.9%)

--- CONCLUSOES ---
- Merge final e o maior gargalo
- Comunicacao MPI e gargalo significativo

> mpiexec -n 128 ./mergesort_paralelo.exe

=== MERGESORT PARALELO ===
Numero de processos: 128
Tamanho total: 2097152 elementos
Tamanho por processo: 16384

Iniciando ordenacao paralela...
Ordenacao local concluida...
Coletando resultados...
Executando merge final...
Ordenacao completa!

--- SPEEDUP ---
Calculando tempo sequencial...
Tempo Sequencial: 0.328000 segundos
Tempo Paralelo:   0.171393 segundos
Speedup:          1.91x
Resultado: Paralelo e 1.9x MAIS RAPIDO!

--- ANALISE DE GARGALOS ---
Scatter (distribuicao):   0.026327 segundos (15.4%)
Ordenacao (paralela):     0.005302 segundos (3.1%)
Gather (coleta):          0.037260 segundos (21.7%)
Merge final (sequencial): 0.055572 segundos (32.4%)

--- CONCLUSOES ---
- Merge final e o maior gargalo
- Comunicacao MPI e gargalo significativo

> mpiexec -n 256 ./mergesort_paralelo.exe

=== MERGESORT PARALELO ===
Numero de processos: 256
Tamanho total: 2097152 elementos
Tamanho por processo: 8192

Iniciando ordenacao paralela...
Ordenacao local concluida...
Coletando resultados...
Executando merge final...
Ordenacao completa!

--- SPEEDUP ---
Calculando tempo sequencial...
Tempo Sequencial: 0.387000 segundos
Tempo Paralelo:   0.425320 segundos
Speedup:          0.91x
Resultado: Sequencial e mais rapido

--- ANALISE DE GARGALOS ---
Scatter (distribuicao):   0.064353 segundos (15.1%)
Ordenacao (paralela):     0.007820 segundos (1.8%)
Gather (coleta):          0.149303 segundos (35.1%)
Merge final (sequencial): 0.093283 segundos (21.9%)

--- CONCLUSOES ---
- Merge final e o maior gargalo
- Comunicacao MPI e gargalo significativo

> mpiexec -n 512 ./mergesort_paralelo.exe

=== MERGESORT PARALELO ===
Numero de processos: 512
Tamanho total: 2097152 elementos
Tamanho por processo: 4096

Iniciando ordenacao paralela...
Ordenacao local concluida...
Coletando resultados...
Executando merge final...
Ordenacao completa!

--- SPEEDUP ---
Calculando tempo sequencial...
Tempo Sequencial: 0.396000 segundos
Tempo Paralelo:   0.775242 segundos
Speedup:          0.51x
Resultado: Sequencial e mais rapido

--- ANALISE DE GARGALOS ---
Scatter (distribuicao):   0.174547 segundos (22.5%)
Ordenacao (paralela):     0.001282 segundos (0.2%)
Gather (coleta):          0.330780 segundos (42.7%)
Merge final (sequencial): 0.098924 segundos (12.8%)

--- CONCLUSOES ---
- Merge final e o maior gargalo
- Comunicacao MPI e gargalo significativo

> mpiexec -n 1024 ./mergesort_paralelo.exe

=== MERGESORT PARALELO ===
Numero de processos: 1024
Tamanho total: 2097152 elementos
Tamanho por processo: 2048

Iniciando ordenacao paralela...
Ordenacao local concluida...
Coletando resultados...
Executando merge final...
Ordenacao completa!

--- SPEEDUP ---
Calculando tempo sequencial...
Tempo Sequencial: 0.363000 segundos
Tempo Paralelo:   1.862147 segundos
Speedup:          0.19x
Resultado: Sequencial e mais rapido

--- ANALISE DE GARGALOS ---
Scatter (distribuicao):   0.904713 segundos (48.6%)
Ordenacao (paralela):     0.001804 segundos (0.1%)
Gather (coleta):          0.563758 segundos (30.3%)
Merge final (sequencial): 0.125418 segundos (6.7%)

--- CONCLUSOES ---
- Merge final e o maior gargalo
- Comunicacao MPI e gargalo significativo

> mpiexec -n 2048 ./mergesort_paralelo.exe

=== MERGESORT PARALELO ===
Numero de processos: 2048
Tamanho total: 2097152 elementos
Tamanho por processo: 1024

Iniciando ordenacao paralela...
Ordenacao local concluida...
Coletando resultados...
Executando merge final...
Ordenacao completa!

--- SPEEDUP ---
Calculando tempo sequencial...
Tempo Sequencial: 0.410000 segundos
Tempo Paralelo:   7.766859 segundos
Speedup:          0.05x
Resultado: Sequencial e mais rapido

--- ANALISE DE GARGALOS ---
Scatter (distribuicao):   5.918291 segundos (76.2%)
Ordenacao (paralela):     0.000702 segundos (0.0%)
Gather (coleta):          1.198156 segundos (15.4%)
Merge final (sequencial): 0.134102 segundos (1.7%)

--- CONCLUSOES ---
- Merge final e o maior gargalo
- Comunicacao MPI e gargalo significativo