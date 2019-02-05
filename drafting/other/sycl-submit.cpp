{
  gpu_selector selector { };
  device selectedDevice { selector };
  queue compute_queue { selectedDevice };
  compute_queue.submit(
    [&](handler& cgh) {
      auto acc_d_A = d_A.get_access<read_write>(cgh);
      auto acc_d_B = d_B.get_access<read_write>(cgh);
      auto acc_d_C = d_C.get_access<read_write>(cgh);
      cgh.parallel_for<class matrixMul_f0>( 
        nd_range<2>(grid_dim * block_dim, block_dim), 
        [=](nd_item<> item) {
          matrixMultiplicationKernel(
            item, acc_d_A, acc_d_B, acc_d_C,
            N);
        }
    );
  });
}