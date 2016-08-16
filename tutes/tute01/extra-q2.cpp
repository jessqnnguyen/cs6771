#include <iostream>

int i;

int f() { return i++; }
int g() { return i == 1 ? i + 3 : i - 1; }
int h() { return i == 2 ? i - 3 : i + 2; }

int main() {
  std::cout << f() + g() * h() << std::endl;
}
