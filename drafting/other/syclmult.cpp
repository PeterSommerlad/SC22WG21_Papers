template<int dim>
void matrixMultiplicationKernel(
	nd_item<dim> item, 
	global_ptr<float> A, 
	global_ptr<float> B, 
	global_ptr<float> C, 
	global_ptr<int> N) 
{
    int ROW = item.get_group(1) * 
              item.get_local_range(1) 
              + item.get_local_id(1);
    int COL = item.get_group(0) * 
              item.get_local_range(0) 
              + item.get_local_id(0);
   
    float tmpSum = 0;
    if (ROW < N && COL < N) {
        for (int i = 0; i < N; i++) {
            tmpSum += *(A + ROW * N + i) * 
                      *(B + i * N + COL);
        }
    }   
    *(C + ROW * N + COL) = tmpSum;
}