#include <hip/hip_runtime.h>
#include <iostream>

#define N 1024

__global__ void relu_kernel(const float* input, float* output, int n) {
    int idx = blockIdx.x * blockDim.x + threadIdx.x;

    if (idx < n) {
        float x = input[idx];
        output[idx] = x > 0.0f ? x : 0.0f;
    }
}

int main() {
    float h_input[N];
    float h_output[N];

    // Initialize input data
    for (int i = 0; i < N; i++) {
        h_input[i] = static_cast<float>(i - 512);
    }

    float* d_input;
    float* d_output;

    hipMalloc(&d_input, N * sizeof(float));
    hipMalloc(&d_output, N * sizeof(float));

    hipMemcpy(d_input, h_input, N * sizeof(float), hipMemcpyHostToDevice);

    int threadsPerBlock = 256;
    int blocks = (N + threadsPerBlock - 1) / threadsPerBlock;

    relu_kernel<<<blocks, threadsPerBlock>>>(d_input, d_output, N);

    hipDeviceSynchronize();

    hipMemcpy(h_output, d_output, N * sizeof(float), hipMemcpyDeviceToHost);

    // Print first few results
    for (int i = 508; i < 516; i++) {
        std::cout << "input[" << i << "] = " << h_input[i]
                  << ", output[" << i << "] = " << h_output[i]
                  << std::endl;
    }

    hipFree(d_input);
    hipFree(d_output);

    return 0;
}