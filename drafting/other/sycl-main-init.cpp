/* Fill values into A and B */
{
    auto acc_d_B = d_B.get_access<cl::sycl::access::mode::read_write>();
    auto acc_d_A = d_A.get_access<cl::sycl::access::mode::read_write>();
    /* Fill values into A and B */
    for (int i { 0 }; i < N; i++) {
        for (int j { 0 }; j < N; j++) {
            acc_d_B[N * i + j] = cos(j);
            acc_d_A[j + N * i] = sin(i);
        }
    }
}
