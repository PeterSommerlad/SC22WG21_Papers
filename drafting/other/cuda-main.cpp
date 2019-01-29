int main() {
   size_t N = 16;
   /* Matrix dimension */
   size_t SIZE = N * N;

   /* Declare device memory pointers */
   float *d_A;
   float *d_B;
   float *d_C;

   /* Allocate CUDA memory */
   cudaMallocManaged(&d_A, SIZE * sizeof(float));
   cudaMallocManaged(&d_B, SIZE * sizeof(float));
   cudaMallocManaged(&d_C, SIZE * sizeof(float));

   /* Fill values into A and B */
   for (int i { 0 }; i < N; i++) {
      for (int j { 0 }; j < N; j++) {
         d_B[N * i + j] = cos(j);
         d_A[j + N * i] = sin(i);
      }
   }

   /* Define grid and block dimensions */
   dim3 block_dim;
   dim3 grid_dim;

   if (N * N > 512) {
      block_dim = {512, 512};
      grid_dim = {(N + 512 - 1) / 512, (N + 512 - 1) / 512};
   } else {
      block_dim = {N, N};
      grid_dim = {1, 1};
   }

   /* Invoke kernel */
   matrixMultiplicationKernel<<<grid_dim, block_dim>>>(d_A, d_B, d_C, N);

   /* Synchronize device and host memory */
   cudaDeviceSynchronize();

   float *cpu_C;
   cpu_C = new float[SIZE];

   /* Run matrix multiplication on the CPU for reference */
   float sum;
   for (int row { 0 }; row < N; row++) {
      for (int col { 0 }; col < N; col++) {
         sum = 0.f;
         for (int n { 0 }; n < N; n++) {
            sum += d_A[row * N + n] * d_B[n * N + col];
         }
         cpu_C[row * N + col] = sum;
      }
   }

   double err { 0 };
   /* Compare the results */
   for (int ROW { 0 }; ROW < N; ROW++) {
      for (int COL { 0 }; COL < N; COL++) {
         err += cpu_C[ROW * N + COL] - d_C[ROW * N + COL];
      }
   }

   std::cout << "Error: " << err << std::endl;

   /* Free the CUDA memory */
   cudaFree(d_A);
   cudaFree(d_B);
   cudaFree(d_C);

   return 0;
}
