#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef unsigned long long ull;
#define LINE_ELEMS 8     
#define CACHE_LINES 4096  

typedef struct {
    ull tag;
    char valid;
} CacheSlot;

CacheSlot cache_slots[CACHE_LINES];
ull cache_hits = 0, cache_misses = 0, cache_accesses = 0;

void cache_reset() {
    for (int i = 0; i < CACHE_LINES; i++) {
        cache_slots[i].valid = 0;
        cache_slots[i].tag = 0;
    }
    cache_hits = cache_misses = cache_accesses = 0;
}

void cache_access(ull address) {
    cache_accesses++;
    ull line = address / LINE_ELEMS;
    ull slot = line % CACHE_LINES;
    if (cache_slots[slot].valid && cache_slots[slot].tag == line) {
        cache_hits++;
    } else {
        cache_misses++;
        cache_slots[slot].valid = 1;
        cache_slots[slot].tag = line;
    }
}

#define MAX 1024  
#define BLOCK 64  

double A[MAX][MAX], B[MAX][MAX], C[MAX][MAX];

void init_matrices(int n) {
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++) {
            A[i][j] = (double)((i + j) % 10);
            B[i][j] = (double)((i - j + 10) % 10);
            C[i][j] = 0.0;
        }
}

double multi_clasica(int n) {
    clock_t start = clock();
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            double sum = 0.0;
            for (int k = 0; k < n; k++) {
                cache_access((ull)&A[i][k]);
                cache_access((ull)&B[k][j]);
                sum += A[i][k] * B[k][j];
            }
            cache_access((ull)&C[i][j]);
            C[i][j] = sum;
        }
    }
    clock_t end = clock();
    return (double)(end - start) / CLOCKS_PER_SEC;
}

double multiply_blocked(int n) {
    clock_t start = clock();
    for (int ii = 0; ii < n; ii += BLOCK) {
        for (int jj = 0; jj < n; jj += BLOCK) {
            for (int kk = 0; kk < n; kk += BLOCK) {
                int i_max = (ii + BLOCK < n) ? ii + BLOCK : n;
                int j_max = (jj + BLOCK < n) ? jj + BLOCK : n;
                int k_max = (kk + BLOCK < n) ? kk + BLOCK : n;
                for (int i = ii; i < i_max; i++) {
                    for (int j = jj; j < j_max; j++) {
                        double sum = C[i][j];
                        for (int k = kk; k < k_max; k++) {
                            cache_access((ull)&A[i][k]);
                            cache_access((ull)&B[k][j]);
                            sum += A[i][k] * B[k][j];
                        }
                        cache_access((ull)&C[i][j]);
                        C[i][j] = sum;
                    }
                }
            }
        }
    }
    clock_t end = clock();
    return (double)(end - start) / CLOCKS_PER_SEC;
}

int main() {
    int sizes[] = {128, 256, 512};
    int num_sizes = sizeof(sizes) / sizeof(sizes[0]);

    printf("Comparación de multiplicación de matrices (clásica vs bloques)\n");
    printf("Cache: %d líneas, %d elementos por línea\n\n", CACHE_LINES, LINE_ELEMS);

    for (int s = 0; s < num_sizes; s++) {
        int n = sizes[s];
        printf("=== Tamaño n = %d ===\n", n);

        init_matrices(n);

        cache_reset();
        double t_classic = multi_clasica
    (n);
        printf("Clásica: %.4fs | Accesos=%llu, Hits=%llu, Misses=%llu, Miss rate=%.4f\n",
               t_classic, cache_accesses, cache_hits, cache_misses,
               (double)cache_misses / cache_accesses);

        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                C[i][j] = 0.0;

        cache_reset();
        double t_blocked = multiply_blocked(n);
        printf("Bloques (BS=%d): %.4fs | Accesos=%llu, Hits=%llu, Misses=%llu, Miss rate=%.4f\n\n",
               BLOCK, t_blocked, cache_accesses, cache_hits, cache_misses,
               (double)cache_misses / cache_accesses);
    }

    return 0;
}
