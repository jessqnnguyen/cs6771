
#include <iostream>
#include <vector> // vectors
#include <algorithm> // sort
#include <array>   // arrays
#include <list>    // list
#include <map>     // list


// Tute 2 STL containers and basic operations
int main(int argc, char* argv[]) {

  // Write a C++ program that does the following:
  // Create a std::vector of std::string containing the strings "one", "two" and "three"
  std::cout << "Creating some strings.." << std::endl;
  std::string one = "one";
  std::string two("two");
  std::string three_tmp = "three";
  std::string three(three_tmp);
  std::vector<std::string> v2;
  v2.push_back(one);
  v2.push_back(two);
  v2.push_back(three);

  std::vector<std::string> v3 = {"one", "two", "three"};
  std::vector<std::string> v4{"one", "two", "three"};

  // Create a std::array of int containing the numbers 3, 2, and 1.
  std::cout << "Creating some arrays..." << std::endl;
  // Initialise am array pf size 3
  std::array<int, 3> arr1 = {3, 2, 1};

  // How is a std::array different from a std::vector?
  // Sort the vector and the array using the STL algorithm std::sort().
  std::sort(arr1.begin(), arr1.end());
  std::sort(v3.begin(), v3.end());

  // Why/How does std::sort() work on both std::vector and std::array?
  // Create a std::list of int
  // Create a back insert iterator for the list of int. e.g.:
  // std::back_insert_iterator<std::list<int>>
  std::list<int> list_1;
  std::back_insert_iterator<std::list<int>> back_it(list_1);

  // Use iterators and a for loop to go over the int array, and use
  // the back insert iterator to insert the values into the int list.
  // How is a back insert iterator different from an input iterator?
  for (auto it = arr1.begin(); it != arr1.end(); ++it) {
    *back_it = *it;
  }
  for (auto elt : list_1) {
    std::cout << elt << std::endl;
  }

  // Why can you not sort a list using std::sort()?
  // std::sort(list_1.begin(), list_1.end());

  // Sort the list using it's .sort() member function (rather than std::sort).
  list_1.sort();

  // Create a std::map where the keys are std::string and the values are int
  // Use a for loop to go through std::string vector, create a std::pair of
  // each string in the vector and its position index, insert each std::pair
  // into the map. Use a range-based (for each) loop over the map to print
  // out the key and value for each pair in the map.
  std::map<std::string, int> map_1;
  std::pair<std::string, int> p;
  auto p1 = std::make_pair(one , 1);

  int index = 0;
  for (auto it = v3.cbegin(); it != v3.cend(); ++it) {
    map_1.insert(std::make_pair(*it, ++index));
  }
  int i = 0;
  for (auto elt : map_1) {
    std::cout << elt.first << ", " << elt.second << std::endl;
  }

  // How to deal with invalidated iterators when you modify the list
  std::vector<int> v = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
  auto remove_it = std::remove(v.begin(), v.end(), 5); // fixes the iterator and keeps it valid


  return EXIT_SUCCESS;
}
