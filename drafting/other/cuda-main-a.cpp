/* Declare device memory pointers */
float *d_A;
/* Allocate CUDA memory */
cudaMallocManaged(&d_A, SIZE*sizeof(float));
/* Synchronize device and host memory */
cudaDeviceSynchronize();
/* Free the CUDA memory */
cudaFree(d_A);