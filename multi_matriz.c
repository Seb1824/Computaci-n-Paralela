#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX 2000 

void init_matrix(double A[MAX][MAX], int n) {
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            A[i][j] = (double)(rand() % 10);
}

void multiply(double A[MAX][MAX], double B[MAX][MAX], double C[MAX][MAX], int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            C[i][j] = 0.0;
            for (int k = 0; k < n; k++) {
                C[i][j] += A[i][k] * B[k][j];
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
        printf("\n--- Multiplicación de matrices %dx%d ---\n", n, n);

        init_matrix(A, n);
        init_matrix(B, n);

        clock_t start = clock();
        multiply(A, B, C, n);
        clock_t end = clock();

        double time_taken = ((double)(end - start)) / CLOCKS_PER_SEC;
        printf("Tiempo: %.4f segundos\n", time_taken);
    }

    return 0;
}
