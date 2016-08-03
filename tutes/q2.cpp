#include <cstdlib>  // optional example
#include <iostream> // io operations
#include <fstream> // file operations

namespace cs6771 {
	void order3(unsigned int& a, unsigned int& b, unsigned int& c) {
		if (a > c) {
			int t = a;
			a = c;
			c = t;
		}
		if (b > c) {
			int t = b;
			b = c;
			c = t;
		}
		if (a > b) {
			int t = a;
			a = b;
			b = t;
		}
	}
}

int main(int argc, char* argv[]) {
  std::ifstream fin("numbers.txt");
  // Take in 3 numbers
  unsigned int x, y, z;
  fin >> x >> y >> z;
  // Sort into descending order
  cs6771::order3(x, y, z);
  // Print to command line
  std::cout << x << " " << y << " " << z << std::endl;
  fin.close();
  // Note std::cout << "\n" << std::flush
  // Print to a file called 'numbers.txt'
  std::ofstream fout("blah.txt");
  fout << x << " " << y << " " << z << std::endl;
  fout.close();
  return EXIT_SUCCESS;
}
