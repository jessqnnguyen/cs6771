#include <iostream>
#include <vector>

/**
 * An example of trying to access an object after deleting it error.
 * Compile with flag -fsanitize=address to get information.
 *
 */
int main() {

  // Bad example number 1.
  // int *i = new int{10};
  // delete i;
  // int j = *i + 1;
  // std::cout << j << std::endl;

  // Bad example number 2: deleting a vector in dynamic memory.
  // std::vector<int> *v = new std::vector<int>();
  // delete v;
  // v->push_back(1);

  // Bad example number 3: address santizer won't help in this case.
  std::vector<int> *v = new std::vector<int>();
  v = nullptr;
}
