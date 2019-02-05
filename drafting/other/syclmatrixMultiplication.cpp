#include <iostream>
#include <CL/sycl.hpp>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <math.h>

template<int dimensions> void matrixMultiplicationKernel(cl::sycl::nd_item<dimensions> item, cl::sycl::global_ptr<float> A,
      cl::sycl::global_ptr<float> B, cl::sycl::global_ptr<float> C, int N);

template<int dimensions>
void matrixMultiplicationKernel(cl::sycl::nd_item<dimensions> item, cl::sycl::global_ptr<float> A, cl::sycl::global_ptr<float> B,
      cl::sycl::global_ptr<float> C, int N) {
   int ROW = item.get_group(1) * item.get_local_range(1) + item.get_local_id(1);
   int COL = item.get_group(0) * item.get_local_range(0) + item.get_local_id(0);
   float tmpSum = 0;
   if (ROW < N && COL < N) {
      /* Each thread computes a single element of the block */
      for (int i = 0; i < N; i++) {
         tmpSum += *(A + ROW * N + i) * *(B + i * N + COL);
      }
   }
   *(C + ROW * N + COL) = tmpSum;
}

int main() {
   size_t N = 16;
   /* Matrix dimension */
   size_t SIZE = N * N;

   /* Declare device memory pointers */
   /* Allocate CUDA memory */
   cl::sycl::buffer<float> d_A(SIZE);
   cl::sycl::buffer<float> d_B(SIZE);
   cl::sycl::buffer<float> d_C(SIZE);

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

   /* Define grid and block dimensions */
   cl::sycl::range < 3 > block_dim;
   cl::sycl::range < 3 > grid_dim;

   if (N * N > 512) {
      block_dim = cl::sycl::range<3> { 512, 512, 1 };
      grid_dim = cl::sycl::range<3> { (N + 512 - 1) / 512, (N + 512 - 1) / 512, 1 };
   } else {
      block_dim = cl::sycl::range<3> { N, N, 1 };
      grid_dim = cl::sycl::range<3> { 1, 1, 1 };
   }

   /* Invoke kernel */
   {
      cl::sycl::gpu_selector selector { };
      cl::sycl::device selectedDevice { selector };
      cl::sycl::queue compute_queue { selectedDevice };
      compute_queue.submit(
            [&](cl::sycl::handler& cgh) {
               auto acc_d_A = d_A.get_access<cl::sycl::access::mode::read_write>(cgh);
               auto acc_d_B = d_B.get_access<cl::sycl::access::mode::read_write>(cgh);
               auto acc_d_C = d_C.get_access<cl::sycl::access::mode::read_write>(cgh);
               cgh.parallel_for<class matrixMultiplicationKernel_functor0>(cl::sycl::nd_range<3>(grid_dim * block_dim, block_dim), [=](cl::sycl::nd_item<> item) {
                        matrixMultiplicationKernel(item, acc_d_A, acc_d_B, acc_d_C, N);
                     }
               );
            });
   };
   float *cpu_C;
   cpu_C = new float[SIZE];

   /* Run matrix multiplication on the CPU for reference */
   float sum;
   {
      auto acc_d_A = d_A.get_access<cl::sycl::access::mode::read_write>();
      auto acc_d_B = d_B.get_access<cl::sycl::access::mode::read_write>();
      for (int row { 0 }; row < N; row++) {
         for (int col { 0 }; col < N; col++) {
            sum = 0.f;
            for (int n { 0 }; n < N; n++) {
               sum += acc_d_A[row * N + n] * acc_d_B[n * N + col];
            }
            cpu_C[row * N + col] = sum;
         }
      }
   }

   double err { 0 };
   /* Compare the results */
   {
      auto acc_d_C = d_C.get_access<cl::sycl::access::mode::read_write>();
      /* Compare the results */
      for (int ROW { 0 }; ROW < N; ROW++) {
         for (int COL { 0 }; COL < N; COL++) {
            err += cpu_C[ROW * N + COL] - acc_d_C[ROW * N + COL];
         }
      }
   }

   std::cout << "Error: " << err << std::endl;
   return 0;
}
