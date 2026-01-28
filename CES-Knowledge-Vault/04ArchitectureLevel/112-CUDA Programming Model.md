---
Theme: CUDA Programming Model
Date created: 2026-01-28 12:00
tags: [ArchitectureLayer, GPU, Programming]
---

## 📚 Idea/Concept

CUDA (Compute Unified Device Architecture) is NVIDIA's parallel computing platform and programming model that enables developers to use GPUs for general-purpose computing through extensions to C/C++.

### CUDA Programming Basics

Host: CPU and its memory
Device: GPU and its memory

Program structure:
1. Allocate memory on device
2. Copy data from host to device
3. Launch kernel (GPU function)
4. Copy results from device to host
5. Free device memory

### Kernel Definition

```cuda
__global__ void vectorAdd(float *A, float *B, float *C, int n) {
    int i = blockIdx.x * blockDim.x + threadIdx.x;
    if (i < n) {
        C[i] = A[i] + B[i];
    }
}
```

Function qualifiers:
- `__global__`: Kernel, called from host, runs on device
- `__device__`: Device function, called from device only
- `__host__`: Host function (default)

### Thread Indexing

Built-in variables:
- `threadIdx.x/y/z`: Thread index within block
- `blockIdx.x/y/z`: Block index within grid
- `blockDim.x/y/z`: Block dimensions
- `gridDim.x/y/z`: Grid dimensions

Global thread ID (1D):
```cuda
int globalIdx = blockIdx.x * blockDim.x + threadIdx.x;
```

2D indexing:
```cuda
int row = blockIdx.y * blockDim.y + threadIdx.y;
int col = blockIdx.x * blockDim.x + threadIdx.x;
```

### Kernel Launch

```cuda
// Configure: <<<gridDim, blockDim>>>
int threadsPerBlock = 256;
int blocksPerGrid = (n + threadsPerBlock - 1) / threadsPerBlock;

vectorAdd<<<blocksPerGrid, threadsPerBlock>>>(d_A, d_B, d_C, n);
```

Launch configuration:
- `blocksPerGrid`: Number of blocks
- `threadsPerBlock`: Threads per block (max 1024)

### Memory Management

```cuda
float *d_A, *d_B, *d_C;

// Allocate device memory
cudaMalloc(&d_A, size);
cudaMalloc(&d_B, size);
cudaMalloc(&d_C, size);

// Copy host to device
cudaMemcpy(d_A, h_A, size, cudaMemcpyHostToDevice);
cudaMemcpy(d_B, h_B, size, cudaMemcpyHostToDevice);

// Launch kernel
vectorAdd<<<blocks, threads>>>(d_A, d_B, d_C, n);

// Copy device to host
cudaMemcpy(h_C, d_C, size, cudaMemcpyDeviceToHost);

// Free device memory
cudaFree(d_A);
cudaFree(d_B);
cudaFree(d_C);
```

### Memory Types in CUDA

| Memory | Declaration | Scope | Lifetime | Speed |
|--------|-------------|-------|----------|-------|
| Register | Automatic | Thread | Thread | Fastest |
| Local | Automatic (spill) | Thread | Thread | Slow |
| Shared | `__shared__` | Block | Block | Fast |
| Global | `__device__` | Grid | Application | Slow |
| Constant | `__constant__` | Grid | Application | Fast (cached) |
| Texture | texture ref | Grid | Application | Fast (cached) |

### Shared Memory Example

```cuda
__global__ void matMul(float *A, float *B, float *C, int N) {
    __shared__ float sA[TILE_SIZE][TILE_SIZE];
    __shared__ float sB[TILE_SIZE][TILE_SIZE];
    
    int row = blockIdx.y * TILE_SIZE + threadIdx.y;
    int col = blockIdx.x * TILE_SIZE + threadIdx.x;
    float sum = 0.0f;
    
    for (int t = 0; t < N/TILE_SIZE; t++) {
        // Load tiles into shared memory
        sA[threadIdx.y][threadIdx.x] = A[row * N + t*TILE_SIZE + threadIdx.x];
        sB[threadIdx.y][threadIdx.x] = B[(t*TILE_SIZE + threadIdx.y) * N + col];
        
        __syncthreads();  // Barrier synchronization
        
        // Compute partial sum
        for (int k = 0; k < TILE_SIZE; k++) {
            sum += sA[threadIdx.y][k] * sB[k][threadIdx.x];
        }
        
        __syncthreads();
    }
    
    C[row * N + col] = sum;
}
```

### Synchronization

Within block:
```cuda
__syncthreads();  // Barrier for all threads in block
```

Atomic operations:
```cuda
atomicAdd(&counter, 1);  // Thread-safe increment
atomicMax(&maxVal, val);
atomicCAS(&addr, compare, val);  // Compare-and-swap
```

### Error Handling

```cuda
cudaError_t err = cudaMalloc(&d_A, size);
if (err != cudaSuccess) {
    printf("CUDA error: %s\n", cudaGetErrorString(err));
}

// Check for kernel errors
cudaDeviceSynchronize();
err = cudaGetLastError();
```

### Streams and Concurrency

```cuda
cudaStream_t stream1, stream2;
cudaStreamCreate(&stream1);
cudaStreamCreate(&stream2);

// Overlapping operations
cudaMemcpyAsync(d_A, h_A, size, cudaMemcpyHostToDevice, stream1);
kernel<<<blocks, threads, 0, stream2>>>(d_B, d_C);

cudaStreamSynchronize(stream1);
cudaStreamDestroy(stream1);
```

### Optimization Guidelines

1. Maximize occupancy (active warps)
2. Minimize warp divergence
3. Coalesce global memory accesses
4. Use shared memory for data reuse
5. Avoid bank conflicts in shared memory
6. Balance register usage vs occupancy

### Memory Coalescing

Good (coalesced):
```cuda
// Thread i accesses element i
data[threadIdx.x]  // Sequential, 1 transaction
```

Bad (strided):
```cuda
// Thread i accesses element i*stride
data[threadIdx.x * 32]  // 32 transactions!
```

## 📌 Key Points (Optional)
- CUDA exposes GPU parallelism through hierarchical thread organization.
- Memory management and access patterns are critical for GPU performance.

## 🖼️ Recommended Image
- Diagram showing CUDA thread hierarchy: Grid → Blocks → Warps → Threads.

## 🔗 Connections
- [[110-GPU Architecture and SIMT Model]]
- [[107-SIMD and Vector Architecture Fundamentals]]
- [[097-Thread Level Parallelism TLP Fundamentals]]
- [[086-Memory Hierarchy Principles and Locality]]