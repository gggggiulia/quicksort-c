#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_NUM 5000
#define INPUT "arq_teste.txt"
#define OUTPUT "tempos_quicksort.txt"

int vet_base[MAX_NUM];

int ler_arq(int vetor[], const char *nome_arquivo) {
    FILE *arq = fopen(nome_arquivo, "rt");
    if (arq == NULL) {
        fprintf(stderr, "Erro na abertura do arquivo de entrada: %s\n", nome_arquivo);
        return 0;
    }

    int i = 0;
    while (i < MAX_NUM && fscanf(arq, "%d", &vetor[i]) == 1) {
        i++;
        fscanf(arq, ",");
    }

    fclose(arq);
    return i;
}

int partition(int v[], int left, int right) {
    int pivot = v[left];
    int l = left + 1;
    int r = right;

    while (l < r) {
        while (l < right && v[l] < pivot) l++;
        while (r > left && v[r] >= pivot) r--;
        if (l < r) {
            int aux = v[l];
            v[l] = v[r];
            v[r] = aux;
        }
    }
    v[left] = v[r];
    v[r] = pivot;
    return r;
}

void quicksort(int v[], int left, int right) {
    if (left < right) {
        int p = partition(v, left, right);
        quicksort(v, left, p - 1);
        quicksort(v, p + 1, right);
    }
}

int main() {
    int i, N, R = 1000;
    int elementos_lidos = ler_arq(vet_base, INPUT);
    
    if (elementos_lidos == 0){
        return 1;
    }

    FILE *output = fopen(OUTPUT, "wt");
    if (output == NULL) {
        fprintf(stderr, "Erro na criação do arquivo de saída: %s\n", OUTPUT);
        return 1;
    }

    for (N = 500; N <= 5000; N += 500) {
        if (elementos_lidos < N) {
            fprintf(stderr, "Número de elementos lidos: %i. Inválido.\n", elementos_lidos);
            break;
        }

        int *vet_aux = (int*)malloc(N * sizeof(int));
        if (vet_aux == NULL) {
            fprintf(stderr, "Erro na alocação para %i números\n", N);
            fclose(output);
            return 1;
        }

        fprintf(output, "----------------------- n = %d ----------------------- \n", N);

        double tempo_total = 0.0;

        for (i = 0; i < R; i++) {
            for (int j = 0; j < N; j++) {
                vet_aux[j] = vet_base[j];
            }

            clock_t begin = clock();
            quicksort(vet_aux, 0, N - 1);
            clock_t end = clock();

            double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
            tempo_total += time_spent;

            fprintf(output, "%.6f\n", time_spent);
        }

        double tempo_medio = tempo_total / R;
        printf("%d dados ordenados com um tempo médio de %.6f segundos\n", N, tempo_medio);

        free(vet_aux);
    }

    fclose(output);
    printf("\nExecucoes concluidas! Os tempos foram salvos em %s\n", OUTPUT);

    return 0;
}
