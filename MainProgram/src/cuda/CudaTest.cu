// ============================================================================
// File: src/cuda/CudaTest.cu
// ============================================================================

#include <iostream>
#include <cuda_runtime.h>

__global__ void vectorAddKernel(int* a, int* b, int* c, int n) {
    int i = blockIdx.x * blockDim.x + threadIdx.x;
    if (i < n) {
        c[i] = a[i] + b[i];
    }
}

extern "C" void runCudaTest() {
    const int N = 16;
    const int size = N * sizeof(int);

    int h_a[N], h_b[N], h_c[N];

    // Initialize host data
    for (int i = 0; i < N; ++i) {
        h_a[i] = i;
        h_b[i] = i * 2;
    }

    int *d_a, *d_b, *d_c;

    cudaMalloc((void**)&d_a, size);
    cudaMalloc((void**)&d_b, size);
    cudaMalloc((void**)&d_c, size);

    cudaMemcpy(d_a, h_a, size, cudaMemcpyHostToDevice);
    cudaMemcpy(d_b, h_b, size, cudaMemcpyHostToDevice);

    vectorAddKernel<<<1, N>>>(d_a, d_b, d_c, N);

    cudaDeviceSynchronize();

    cudaMemcpy(h_c, d_c, size, cudaMemcpyDeviceToHost);

    std::cout << "CUDA test results:\n";
    for (int i = 0; i < N; ++i) {
        std::cout << h_a[i] << " + " << h_b[i] << " = " << h_c[i] << "\n";
    }

    cudaFree(d_a);
    cudaFree(d_b);
    cudaFree(d_c);

    std::cout << "CUDA test completed successfully.\n";
}