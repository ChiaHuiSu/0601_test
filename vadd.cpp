#include <stdio.h>
#include <math.h>
#include "hip/hip_runtime.h"

// GPU kernel for vector addition
__global__ void vec_add(double *A, double *B, double *C, int n) {
    // [TODO] Add kernel code here
}

int main(int argc, char *argv[]) {
    const int N = 1024 * 1024;
    const size_t bytes = N * sizeof(double);

    // Host-side memory allocation
    double *h_A = (double*)malloc(bytes);
    double *h_B = (double*)malloc(bytes);
    double *h_C = (double*)malloc(bytes);

    // Initialize host data
    for (int i = 0; i < N; i++) {
        h_A[i] = sin(i) * sin(i);
        h_B[i] = cos(i) * cos(i);
        h_C[i] = 0.0;
    }

    // Device pointers
    // [TODO] hipMalloc
    double *d_A, *d_B, *d_C;


    // Copy data from host to device
    // [TODO] hipMemcpy


    // Kernel launch configuration
    const int threads_per_block = 256;
    const int blocks_in_grid = (N + threads_per_block - 1) / threads_per_block;

    // Launch the kernel
    vec_add<<<blocks_in_grid, threads_per_block>>>(d_A, d_B, d_C, N);

    // Copy result back to host
    // [TODO] hipMemcpy

    // Print Something
    for (int i = 0; i < 10; i ++) {
        printf("%f + %f = %f\n", h_A[i], h_B[i], h_C[i]);
    }

    // Validate results
    double sum = 0.0;
    double tolerance = 1.0e-14;
    for (int i = 0; i < N; i++)
        sum += h_C[i];

    if (fabs((sum / N) - 1.0) > tolerance) {
        printf("Error: Sum/N = %.2f instead of ~1.0\n", sum / N);
        exit(1);
    }

    // Cleanup
    free(h_A);
    free(h_B);
    free(h_C);
    hipFree(d_A);
    hipFree(d_B);
    hipFree(d_C);

    printf("__SUCCESS__\n");
    return 0;
}