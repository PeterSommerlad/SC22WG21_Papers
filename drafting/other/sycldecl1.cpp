using namespace cl::sycl;

template<int dimensions> 
void matrixMultiplicationKernel(
nd_item<dimensions> item, 
global_ptr<float> A, 
global_ptr<float> B, 
global_ptr<float> C, 
global_ptr<int> N);

