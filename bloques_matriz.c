#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX 2000   
#define BS 64     


void init_matrix(double A[MAX][MAX], int n) {
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            A[i][j] = (double)(rand() % 10);
}

void multiply_blocks(double A[MAX][MAX], double B[MAX][MAX], double C[MAX][MAX], int n) {
    for (int ii = 0; ii < n; ii += BS) {
        for (int jj = 0; jj < n; jj += BS) {
            for (int kk = 0; kk < n; kk += BS) {
   
                for (int i = ii; i < ii + BS && i < n; i++) {
                    for (int j = jj; j < jj + BS && j < n; j++) {
                        double sum = C[i][j];
                        for (int k = kk; k < kk + BS && k < n; k++) {
                            sum += A[i][k] * B[k][j];
                        }
                        C[i][j] = sum;
                    }
                }
            }
        }
    }
}

int main() {
    static double A[MAX][MAX], B[MAX][MAX], C[MAX][MAX];
    srand(time(NULL));

    int sizes[] = {200, 500, 800, 1000};  
    int num_sizes = sizeof(sizes) / sizeof(sizes[0]);

    for (int s = 0; s < num_sizes; s++) {
        int n = sizes[s];
        printf("\n--- Multiplicación de matrices por bloques %dx%d ---\n", n, n);

        init_matrix(A, n);
        init_matrix(B, n);

        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                C[i][j] = 0.0;

        clock_t start = clock();
        multiply_blocks(A, B, C, n);
        clock_t end = clock();

        double time_taken = ((double)(end - start)) / CLOCKS_PER_SEC;
        printf("Tiempo: %.4f segundos (BS=%d)\n", time_taken, BS);
    }

    return 0;
}