#include <algorithm>
#include <iostream>

// find the least value in the arguments
template <typename T, typename... Ts>
T least(T first, Ts... rest)
{
  std::sort(&first,
    &first + 1 + sizeof...(rest));
  return first;
}

int main()
{
  std::cout << "least(1): "
    << least(1) << std::endl;
  std::cout << "least(1,2,3,4): "
    << least(1,2,3,4) << std::endl;
  std::cout << "least(10,9,8,7,6,5,4,3,2,1): "
    << least(10,9,8,7,6,5,4,3,2,1)
    << std::endl;
}
