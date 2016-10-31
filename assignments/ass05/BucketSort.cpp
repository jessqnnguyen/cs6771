#include "BucketSort.h"

#include <algorithm>
#include <cmath>
#include <thread>
#include <iostream>

bool aLessB(const unsigned int& x, const unsigned int& y, unsigned int pow) {
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
    return aLessB(x,y,pow + 1);  // recurse if this digit is the same
  } else {
    return a < b;
  }
}

template <typename F>
void BucketSort::locked_scope(std::mutex& m, F f) {
  std::lock_guard<std::mutex> lg{m};
  f();
}

// TODO: replace this with a parallel version.
void BucketSort::sort(unsigned int numCores) {
  // Store a vector of vectors where each digit is represented.
  // E.g. The 0th vector stores all numbers with an msd of 0.
  std::vector<std::vector<unsigned int>> radixBuckets(10);

  unsigned int numPartitions = numCores - 1;
  // Store size of partitions and number of partitions.
  unsigned int m = numbersToSort.size()/numPartitions;

  // Create a lock guard to safely lock access of data between threads.
  std::mutex mLock;

  // auto get_digits = [&](unsigned int num) {
  //   std::vector<unsigned int> digits;
  //   while (num) {
  //     digits.push_back(num % 10);
  //     num /= 10;
  //   }
  //   std::reverse(digits.begin(), digits.end());
  //   return digits.size();
  // }

  auto get_digit = [&](unsigned int num, unsigned int digit) {
    std::vector<unsigned int> digits;
    while (num) {
      digits.push_back(num % 10);
      num /= 10;
    }
    std::reverse(digits.begin(), digits.end());
    return digits.at(digit);
  };

  // Given a partition number, returns the index of the starting
  // index of the current partition.
  // Returns -1 otherwise. This is an unsuccessful result.
  auto get_partition_pos = [&](unsigned int partitionNum) {
    unsigned int pos = -1;
    // Check partition number is within the bounds of allowed partition number.
    if (partitionNum <= numPartitions) {
      // First partition case.
      if (partitionNum == 0) {
        pos = 0;
      } else {
        pos =  partitionNum * m;
      }
    }
    return pos;
  };

  auto push_to_radix_buckets = [&](unsigned int partitionNum) {
    try {
      unsigned int startPos = get_partition_pos(partitionNum);
      unsigned int endPos;
      if (partitionNum == numPartitions) {
        endPos = numbersToSort.size() - 1;
      } else {
        endPos = get_partition_pos(partitionNum + 1) - 1;
      }
      unsigned int currNum, currMSD;
      // unsigned int currHighestBase = 0, currBase;
      // Find base of most significant digit.
      // for (unsigned int j = startPos; j <= endPos; ++j) {
      //   currNum = numbersToSort.at(j);
      //   currBase = getDigits(currNum);
      //   if (currBase > currHighestBase) {
      //     currHighestBase = currBase;
      //   }
      // }
      // currHighestBase = pow(10, currHighestBase - 1);
      // locked_scope(mLock, [&] {
        for (unsigned int i = startPos; i <= endPos; ++i) {
          currNum = numbersToSort.at(i);
          // currMSD = floor(currNum/currHighestBase);
          currMSD = get_digit(currNum, 0);
          locked_scope(mLock, [&] {
            radixBuckets[currMSD].push_back(currNum);
          });
        }
      //  });
    } catch (std::runtime_error &e) {
      std::cout << "Exception caught: " << e.what() << std::endl;
    }
  };
  // // DEBUG PRINT AREA
  // for (unsigned int i = 0; i < numPartitions; ++i) {
  //     unsigned int startPos = getPartitionPos(i);
  //     std::cout << "partition " << i << " start pos= " << startPos << std::endl;
  // }

  // Create threads.
  std::vector<std::thread> threads;

  // Create threads to sort each partition into radix buckets.
  for (unsigned int i = 0; i < numPartitions; ++i) {
    threads.push_back(std::thread(push_to_radix_buckets, i));
  }

  // Wait for threads to finish before closing them.
  for (auto& t : threads) t.join();

  // Update m partition size to sort by partition each of the radix buckets.
  m = radixBuckets.size()/numPartitions;

  auto sort_each_radix_bucket = [&](unsigned int radixPartitionNum) {
    try {
      unsigned int startPos, endPos;
      startPos = get_partition_pos(radixPartitionNum);
      if (radixPartitionNum == numPartitions) {
       endPos = radixBuckets.size() - 1;
      } else {
       endPos = get_partition_pos(radixPartitionNum + 1) - 1;
      }
      for (unsigned int i = startPos; i <= endPos; ++i) {
        std::vector<unsigned int> currBucket = radixBuckets.at(i);
        std::sort(currBucket.begin(), currBucket.end(), std::less<int>());
                  // [](const unsigned int& x, const unsigned int& y) {
                  //     return std::less(x,y);
                  // });
        locked_scope(mLock, [&] {
          radixBuckets[i] = currBucket;
        });
      }
    } catch (std::runtime_error &e) {
      std::cout << "Exception caught: " << e.what() << std::endl;
    }
  };

  // Create threads.
  std::vector<std::thread> sortingThreads;
  // Create threads to sort each radix bucket in partitions.
  for (unsigned int i = 0; i < numPartitions; ++i) {
    sortingThreads.push_back(std::thread(sort_each_radix_bucket, i));
  }

  // Wait for threads to finish before closing them.
  for (auto& t : sortingThreads) t.join();

  int i = 0;
  for (auto& bucket : radixBuckets) {
    std::cout << "printing bucket at radix " << i << std::endl;
    for (auto& num : bucket) {
      std::cout << num << " ";
    }
    std::cout << std::endl;
    i++;
  }

  // Merge buckets
  numbersToSort.clear();
  for (auto& bucket : radixBuckets) {
    for (auto& num : bucket) {
      numbersToSort.push_back(num);
    }
  }
  std::cout << "final sorted list ";
  for (auto& num : numbersToSort) {
    std::cout << num << " ";
  }
  // std::cout << std::endl;
  // std::sort(numbersToSort.begin(),numbersToSort.end(), [](const unsigned int& x, const unsigned int& y){
  //         return aLessB(x,y,0);
  // } );
}
