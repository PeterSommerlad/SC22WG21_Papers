//template aliases provided automatically
template<cl::sycl::access::mode mode, 
	int dim>
using Accessor = cl::sycl::accessor<
	float, dim, mode,
	cl::sycl::access::target::global_buffer>;
template<int dim>
using ReadAccessor = Accessor<
	cl::sycl::access::mode::read,dim>;
template<int dim>
using WriteAccessor = Accessor<
	cl::sycl::access::mode::write, dim>;

template<int dim>
void matrixMultiplicationKernel(
	nd_item<dim> item, 
	ReadAccessor<dim> A, 
	ReadAccessor<dim> B, 
	WriteAccessor<dim> C, 
	int N); 
