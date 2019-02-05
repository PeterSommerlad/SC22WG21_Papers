dim3 block_dim;
dim3 grid_dim;
/* initialize block_dim, grid_dim*/
matrixMultiplicationKernel<<<grid_dim, block_dim>>>(d_A, d_B, d_C, N);