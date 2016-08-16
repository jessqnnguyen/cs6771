#include <cstdlib>  // optional example
#include <iostream> // io operations
#include <fstream> // file operations

namespace cs6771 {
	void order3(int *a, int *b, int *c) {
		if (*a < *c) {
			int t = *a;
			*a = *c;
			*c = t;
		}
		if (*a < *b) {
			int t = *a;
			*a = *b;
			*b = t;
		}
		if (*b < *c) {
			int t = *b;
			*b = *c;
			*c = t;
		}
	}
}

int main(int argc, char* argv[]) {
  // Take in 3 numbers
  int x, y, z;
  std::cin >> x >> y >> z;
  // Sort into descending order
  cs6771::order3(&x, &y, &z);
  // Print to command line
  std::cout << x << " " << y << " " << z << std::endl;
  // Note std::cout << "\n" << std::flush
  // Print to a file called 'numbers.txt'
  std::ofstream fout("numbers.txt");
  fout << x << " " << y << " " << z << std::endl;
  return EXIT_SUCCESS;
}
