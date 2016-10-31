#include <iostream>
#include <algorithm>
#include <cmath>

// g++ -std=c++14 -Wall -Werror -o test test.cpp
bool aLessB(const unsigned int& x, const unsigned int& y, unsigned int pow) {
  std::string s1 = std::to_string(x);
  std::string s2 = std::to_string(y);
  if (s1[pow] == s2[pow]) {
    return aLessB(x, y, pow + 1);
  } else {
    return s1[pow] < s2[pow];
  }
}

bool lecturers(const unsigned int& x, const unsigned int& y, unsigned int pow) {
  if (x == y) return false; // if the two numbers are the same then one is not less than the other
  unsigned int a = x;
  unsigned int b = y;
  // work out the digit we are currently comparing on.
  if (pow == 0) {
          while (a / 10 > 0) {
                  a = a / 10;
          }
          while (b / 10 > 0) {
                  b = b / 10;
          }
  } else {
  	while (a / 10 >= (unsigned int) std::round(std::pow(10,pow))) {
                  a = a / 10;
          }
          while (b / 10 >= (unsigned int) std::round(std::pow(10,pow))) {
                  b = b / 10;
          }
  }
  if (a == b) {
    return lecturers(x,y,pow + 1);  // recurse if this digit is the same
  } else {
    return a < b;
  }
}

int main() {
  std::cout << "aLessB(501, 52) = " << aLessB(501, 52, 0) << std::endl;
  std::cout << "lecturers(501, 52, 0) = " << lecturers(501, 52, 0) << std::endl;
  std::vector<unsigned int> numbers{501, 52, 5};
  std::sort(numbers.begin(), numbers.end(),
         [](const unsigned int& x, const unsigned int& y){
           return aLessB(x, y, 0);
         });
  for (auto& n : numbers) {
    std::cout << n << " ";
  }
  std::cout << std::endl;
}
