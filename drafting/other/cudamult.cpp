__global__ void matrixMultiplicationKernel(float *A, float *B, float *C, int N) {
    int ROW = blockIdx.y * blockDim.y + threadIdx.y;
    int COL = blockIdx.x * blockDim.x + threadIdx.x;
   
    float tmpSum = 0;
    if (ROW < N && COL < N) {
        for (int i = 0; i < N; i++) {
            tmpSum += A[ROW * N + i] * 
                      B[i * N + COL];
        }
    }
    C[ROW * N + COL] = tmpSum;
}
