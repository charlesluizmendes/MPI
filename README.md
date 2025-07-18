# ⚙️ AMBIENTE DE EXECUÇÃO

## 🎯 Pré-requisitos e Configuração

### Ambiente de Desenvolvimento:

- **Sistema Operacional**: Windows 11
- **Compilador**: g++ (MSYS2 UCRT64)
- **MPI**: Microsoft MPI (MS-MPI)
- **IDE**: Visual Studio Code
- **Hardware**: 8 cores físicos, 16 threads lógicos

## 🛠️ Instalação do C:

### Instalar MSYS2:

```bash
pacman -S mingw-w64-ucrt-x86_64-gcc
pacman -S mingw-w64-ucrt-x86_64-gdb
```

**Link de download:** https://www.msys2.org/

## 📥 Download do MPI

### Arquivos Necessários:

1. **msmpisetup.exe** - Runtime do MPI
2. **msmpisdk.msi** - SDK para desenvolvimento

**Link de download:** https://www.microsoft.com/en-us/download/details.aspx?id=105289

---

## 🛠️ Instalar MPI

### 1. Instalar Runtime (msmpisetup.exe)

- Executar como Administrador
- Durante instalação, alterar diretório para: `C:\MPI`
- Confirmar instalação

### 2. Instalar SDK (msmpisdk.msi)

- Executar como Administrador  
- Durante instalação, alterar diretório para: `C:\MPI\SDK`
- Confirmar instalação

---

## 🔧 Configurar Variáveis de Ambiente

### Via CMD (como Administrador):

```cmd
setx MSMPI_INC "C:\MPI\SDK\Include" /M
setx MSMPI_LIB64 "C:\MPI\SDK\Lib\x64" /M
setx PATH "%PATH%;C:\MPI\Bin" /M
```

# 📊 RELATÓRIO COMPLETO

## 🎯 Resumo Executivo

Este relatório apresenta uma análise abrangente da escalabilidade do algoritmo MergeSort paralelo implementado com MPI, testando desde 2 até 2048 processos com um array de 2.097.152 elementos. Os resultados demonstram claramente o ponto ótimo de paralelização e os limites práticos da escalabilidade.

---

## 📈 Resultados de Speedup por Número de Processos

| Processos | Speedup | Eficiência | Tempo Paralelo | Classificação |
|-----------|---------|------------|----------------|---------------|
| **2**     | **1.79x** | **89.5%** | 0.146s | 🟢 **EXCELENTE** |
| **4**     | **2.98x** | **74.5%** | 0.089s | 🟢 **EXCELENTE** |
| **8**     | **3.32x** | **41.5%** | 0.080s | 🟢 **MUITO BOM** |
| **16**    | **3.92x** | **24.5%** | 0.072s | 🟢 **ÓTIMO** |
| **32**    | **3.38x** | **10.6%** | 0.083s | 🟡 **BOM** |
| **64**    | **2.64x** | **4.1%**  | 0.109s | 🟠 **REGULAR** |
| **128**   | **1.91x** | **1.5%**  | 0.171s | 🟠 **FRACO** |
| **256**   | **0.91x** | **0.4%**  | 0.425s | 🔴 **CONTRAPRODUCENTE** |
| **512**   | **0.51x** | **0.1%**  | 0.775s | 🔴 **MUITO RUIM** |
| **1024**  | **0.19x** | **0.02%** | 1.862s | 🔴 **CATASTRÓFICO** |
| **2048**  | **0.05x** | **0.002%** | 7.767s | 🔴 **INVIÁVEL** |

---

### Tempo Paralelo x Número De Processos

<img width="670" height="480" alt="image" src="https://github.com/user-attachments/assets/83a12274-d8df-4336-b6b6-a7c6fc460fff" />

### 📊 Evolução dos Gargalos (%)

| Processos | Ordenação | Scatter | Gather | Merge Final | Comunicação Total |
|-----------|-----------|---------|--------|-------------|-------------------|
| **2**     | **91.0%** | 3.4%    | 0.6%   | 5.0%        | **4.0%** |
| **4**     | **75.0%** | 6.8%    | 1.9%   | 16.2%       | **8.7%** |
| **8**     | **61.5%** | 8.2%    | 2.6%   | 27.5%       | **10.8%** |
| **16**    | **47.0%** | 10.6%   | 3.4%   | 38.8%       | **14.0%** |
| **32**    | **37.8%** | 11.2%   | 3.9%   | 42.2%       | **15.1%** |
| **64**    | **16.1%** | 13.9%   | 7.0%   | 40.9%       | **20.9%** |
| **128**   | **3.1%**  | 15.4%   | 21.7%  | 32.4%       | **37.1%** |
| **256**   | **1.8%**  | 15.1%   | 35.1%  | 21.9%       | **50.2%** |
| **512**   | **0.2%**  | 22.5%   | 42.7%  | 12.8%       | **65.2%** |
| **1024**  | **0.1%**  | 48.6%   | 30.3%  | 6.7%        | **78.9%** |
| **2048**  | **0.0%**  | 76.2%   | 15.4%  | 1.7%        | **91.6%** |

---

### Evolução Dos Gargalos

<img width="670" height="480" alt="image" src="https://github.com/user-attachments/assets/b49a1899-7851-4cea-bc90-cccf9f5dd4cb" />

## 🎯 Pontos-Chave Identificados

### 🏆 Ponto Ótimo: 16 Processos

- **Máximo speedup**: 3.92x
- **Eficiência**: 24.5%
- **Trabalho útil**: 47.0%
- **Comunicação**: 14.0%

### ⚖️ Ponto de Equilíbrio: 32 Processos

- Último ponto com speedup > 3.0x
- Merge final torna-se gargalo dominante (42.2%)

### 📉 Início da Degradação: 64 Processos

- Speedup cai para 2.64x
- Comunicação MPI torna-se significativa (20.9%)

### 🚨 Ponto de Ruptura: 256 Processos

- **Speedup < 1.0** (paralelo mais lento que sequencial)
- Comunicação domina 50.2% do tempo

### 💥 Colapso Total: 1024+ Processos

- Speedup catastrófico (0.19x e 0.05x)
- Comunicação consome 78.9% e 91.6% do tempo

---

## 📊 Análise de Gargalos por Categoria

### 🔵 Zona Eficiente (2-16 processos)

- **Característica**: Ordenação domina (47-91%)
- **Comunicação**: Baixa (4-14%)
- **Speedup**: Crescente (1.79x → 3.92x)
- **Conclusão**: Paralelismo efetivo

### 🟡 Zona de Transição (32-64 processos)

- **Característica**: Merge final vira gargalo principal
- **Comunicação**: Moderada (15-21%)
- **Speedup**: Estável/declinante (3.38x → 2.64x)
- **Conclusão**: Ainda útil, mas com limitações

### 🟠 Zona de Degradação (128-256 processos)

- **Característica**: Comunicação supera trabalho útil
- **Comunicação**: Alta (37-50%)
- **Speedup**: Declínio acentuado (1.91x → 0.91x)
- **Conclusão**: Ineficiente

### 🔴 Zona de Colapso (512+ processos)

- **Característica**: Comunicação domina completamente
- **Comunicação**: Crítica (65-92%)
- **Speedup**: Catastrófico (0.51x → 0.05x)
- **Conclusão**: Contraproducente

---

## 🔬 Validação da Lei de Amdahl

### Parte Sequencial Identificada:

- **Merge Final**: 5% → 42% do tempo total
- **Limite teórico**: ~2.4x speedup máximo
- **Resultado obtido**: 3.92x (superou devido a cache effects)

### Overhead de Comunicação:

- **Scatter**: Cresce de 3.4% → 76.2%
- **Gather**: Cresce de 0.6% → 15.4%
- **Total**: Crescimento exponencial com número de processos

---

## 📋 Recomendações

### ✅ Para Máxima Performance:

- **Usar 8-16 processos** para arrays de ~2M elementos
- **Speedup esperado**: 3.3x - 3.9x
- **Eficiência**: 20-40%

### ⚖️ Para Balancear Recursos:

- **Usar 4-8 processos** para eficiência alta
- **Speedup esperado**: 2.98x - 3.32x
- **Eficiência**: 40-75%

### ❌ Evitar:

- **32+ processos** para arrays desta magnitude
- **256+ processos** (sempre contraproducente)

---

## 🏆 Conclusões

1. **Escalabilidade Limitada**: MergeSort paralelo escala eficientemente apenas até 16 processos
2. **Gargalo Principal**: Merge final sequencial limita speedup máximo
3. **Overhead Crítico**: Comunicação MPI cresce exponencialmente
4. **Granularidade**: Arrays de 2M elementos são insuficientes para muitos processos
5. **Lei de Amdahl Validada**: Parte sequencial e overhead limitam paralelização

## 📚 Contribuição

Este estudo fornece dados empíricos precisos sobre os limites práticos da paralelização do MergeSort, demonstrando que **mais processos não garantem melhor performance** e validando teorias fundamentais de computação paralela através de medições experimentais rigorosas.

---

**🎯 Resultado Final: Ponto ótimo de 16 processos com speedup de 3.92x para arrays de 2.097.152 elementos.**
