//============================================================================
// Name        : size_type_signed.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================
#include <array>
#include <vector>
#include <iostream>

using meg=std::array<char,1024*1024>;
using big=std::vector<meg>;
using namespace std;

#include <cstdint>
#include <limits>

struct A
{
  std::int32_t si;
   std::uint32_t ui;
   std::int32_t sib : 2;
   std::uint32_t uib : 2;
   std::uint32_t uib31 : 31;
};

int main()
{
   A a = {};

  a.si = std::numeric_limits<decltype(a.si)>::max();
  std::cout << "a.si = "<< a.si<<endl;
  ++a.si; // #1
  std::cout << "a.si = "<< a.si<<endl;

  a.ui = std::numeric_limits<decltype(a.ui)>::max();
  std::cout << "a.ui = "<< a.ui<<endl;
  ++a.ui; // #2
  std::cout << "a.ui = "<< a.ui<<endl;

  a.sib = 1;
  ++a.sib; // #3
  std::cout << "a.sib after = "<< a.sib<<endl;

  a.uib = 3;
  ++a.uib; // #4
  std::cout << "a.uib after = "<< a.uib<<endl;

  a.uib31 = std::numeric_limits<decltype(a.uib31)>::max() / 2;
  std::cout << "a.uib31 = "<< a.uib31<<endl;
  ++a.uib31; // #5
  std::cout << "a.uib31 = "<< a.uib31<<endl;
}



