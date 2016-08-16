#include <iostream>
#include<iterator>


int main() {
  int ia[3][4] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 };
  std::cout << "std:begin(ia) = " << std::begin(ia) << std::endl;
  // for (auto p = std::begin(ia); p != std::end(ia); ++p) {
  //   for (auto q = std::begin(*p); q != std::end(*p); ++q) {
  //     std::cout << *q << ' ';
  //   }
  //   std::cout << std::endl;
  // }
}
