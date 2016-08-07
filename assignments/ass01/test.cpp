#include "genlib.h"
#include "lexicon.h"
#include <string>
#include <vector>
#include <iostream>


int main() {
  // vector<int> blah{1, 2, 3};
  // for (int num : blah) {
  //   std::cout << num << std::endl;
  // }
  // blah = {};
  // std::cout << "blah is now " << blah.size() << " size" << std::endl;
  // blah.push_back(3);
  // blah.push_back(6);
  // for (int num : blah) {
  //   std::cout << num << std::endl;
  // }
  Lexicon english("SmallDictionary.txt");
  foreach (std::string word in english) {
    std::cout << word << std::endl;
  }
  return 0;
}
