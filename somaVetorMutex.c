#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <omp.h>

// Antônio Costa Satiro de Souza 10723636

#define TAMANHO (long long)round(pow(2, 28))

int main(int argc, char *argv[]){

    int nThreads = atoi(argv[1]);
    int *v = malloc(TAMANHO * sizeof(int));

    for(long i = 0; i < TAMANHO; i++){
        v[i] = i;
    }

    long long total = 0; // variável compartilhada
    struct timespec inicio, fim;
    clock_gettime(CLOCK_MONOTONIC, &inicio);

    #pragma omp parallel num_threads(nThreads)
    {
        int my_rank = omp_get_thread_num();
        int thread_count = omp_get_num_threads();

        long bloco = TAMANHO / thread_count;
        long start = my_rank * bloco;
        long end;
        if (my_rank == thread_count - 1){
            end = TAMANHO;
        }else{
            end = start + bloco;
        }

        long long resultadoParte = 0;
        for(long i = start; i < end; i++){
            resultadoParte += v[i];
        }

        #pragma omp critical
        total += resultadoParte;
    }

    clock_gettime(CLOCK_MONOTONIC, &fim);
    free(v);

    double tempo = (fim.tv_sec - inicio.tv_sec) + (fim.tv_nsec - inicio.tv_nsec) / 1e9;
    printf("[Mutex] N threads: %.2i | Soma: %lld | Tempo: %.4f s\n", nThreads, total, tempo);

    return 0;
}