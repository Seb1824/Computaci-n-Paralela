#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX 2000
#define NUM_RUNS 5

double A[MAX][MAX], x[MAX], y[MAX];

void init_data() {
    for (int i = 0; i < MAX; i++) {
        x[i] = 1.0;
        y[i] = 0.0;
        for (int j = 0; j < MAX; j++) {
            A[i][j] = (double)(i + j);
        }
    }
}

int main() {
    clock_t start, end;
    double cpu_time;
    double total_time_rows = 0.0;
    double total_time_cols = 0.0;

    for (int run = 0; run < NUM_RUNS; run++) {
        printf("Ejecución %d de %d:\n", run + 1, NUM_RUNS);
        
        init_data();

        start = clock();
        for (int i = 0; i < MAX; i++) {
            for (int j = 0; j < MAX; j++) {
                y[i] += A[i][j] * x[j];
            }
        }
        end = clock();
        cpu_time = ((double)(end - start)) / CLOCKS_PER_SEC;
        printf("  Tiempo acceso por filas: %f segundos\n", cpu_time);
        total_time_rows += cpu_time;

        for (int i = 0; i < MAX; i++) y[i] = 0.0;

        start = clock();
        for (int j = 0; j < MAX; j++) {
            for (int i = 0; i < MAX; i++) {
                y[i] += A[i][j] * x[j];
            }
        }
        end = clock();
        cpu_time = ((double)(end - start)) / CLOCKS_PER_SEC;
        printf("  Tiempo acceso por columnas: %f segundos\n", cpu_time);
        total_time_cols += cpu_time;
    }

    printf("\nRESULTADOS FINALES:\n");
    printf("Tiempo promedio acceso por filas: %f segundos\n", total_time_rows/NUM_RUNS);
    printf("Tiempo promedio acceso por columnas: %f segundos\n", total_time_cols/NUM_RUNS);

    return 0;
}