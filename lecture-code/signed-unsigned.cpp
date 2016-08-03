#include <iostream>

int main() {
  signed int a = -1;
  unsigned int b = 1;
  std::cout << a * b << std::endl;
  unsigned u = 10;
  int i = -42;
  unsigned int x = -42;
  std::cout << x << std::endl;
  std::cout << i + i << std::endl; // prints -84
  std::cout << u + i << std::endl; // if 32-bit ints, prints 4294967264
}
