#include "BucketSort.h"

#include <algorithm>
#include <cmath>
#include <thread>
#include <iostream>

bool aLessB(const unsigned int& x, const unsigned int& y, unsigned int pow) {
  std::string s1 = std::to_string(x);
  std::string s2 = std::to_string(y);
  if (s1[pow] == s2[pow]) {
    return aLessB(x, y, pow + 1);
  } else {
    return s1[pow] < s2[pow];
  }
}

// Helper function for mutex access to a resource by using std::lock_guard.
template <typename F>
void BucketSort::locked_scope(std::mutex& m, F f) {
  std::lock_guard<std::mutex> lockGuard{m};
  f();
}

void BucketSort::sort(unsigned int numCores) {
  // Store radix buckets into a vector of vectors where
  // radixBucket[1] stores all numbers which begin with 1.
  std::vector<std::vector<unsigned int>> radixBuckets(10);

  // Store number of partitions which is always one less than the number of
  // given due to the main thread using one processsor.
  unsigned int numPartitions = numCores - 1;

  // Store size of partitions and number of partitions.
  unsigned int m = numbersToSort.size()/numPartitions;

  // Create a lock guard to safely lock access of radix buckets between threads.
  std::mutex radixBucketsLock;

  // ---------------------------------------------------------------
  // Helper lambda functions.
  // ---------------------------------------------------------------

  // Given a number and a digit, returns the digit at that number
  // starting at the 0th index.
  // For example: get_digit(10, 0) = 1. Since 1 is the 0th digit.
  auto get_digit = [&](unsigned int num, unsigned int digit) {
    std::vector<unsigned int> digits;
    while (num) {
      digits.push_back(num % 10);
      num /= 10;
    }
    std::reverse(digits.begin(), digits.end());
    return digits.at(digit);
  };

  // Given a partition number (starting at 0), returns the index of
  // the starting index of the current partition.
  // For example: get_partition_pos(0) is always 0 since the first partition's
  // starting index should always be zero.
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

  // This function takes a partition number, works out the start and end
  // points of the partition in the unsorted array and proceeds to process only
  // this partition by pushing it into the correct radix buckets in the vector
  // radixBuckets.
  // Relies on the previously calculated values of m (partition size) and
  // numPartitions.
  // Makes use of helper lambda functions get_partition_pos() and get_digit().
  // This function is passed into each new thread created such that each
  // thread available works on a different partition of the unsorted numbers
  // list.
  auto push_to_radix_buckets = [&](unsigned int partitionNum) {
    try {
      // Retrieve start and end points of the current partition number.
      unsigned int startPos, endPos;
      startPos = get_partition_pos(partitionNum);
      // End case: if this is the last partition, the end point is always the
      // last index of the unsorted numbers list.
      // This deals with the case where the unsorted numbers list cannot be
      // divided evenly.
      if (partitionNum == numPartitions) {
        endPos = numbersToSort.size() - 1;
      } else {
        endPos = get_partition_pos(partitionNum + 1) - 1;
      }

      // Keep track of the current number being iterated and that number's most
      // significant digit (i.e. the 0th digit).
      unsigned int currNum, currMSD;
      for (unsigned int i = startPos; i <= endPos; ++i) {
        currNum = numbersToSort.at(i);
        currMSD = get_digit(currNum, 0);

        // Write operation to shared resource - radixBuckets vector. Lock until
        // write process is complete.
        locked_scope(radixBucketsLock, [&] {
          radixBuckets[currMSD].push_back(currNum);
        });
      }
    } catch (std::runtime_error &e) {
        std::cout << "Exception caught: " << e.what() << std::endl;
    }
  };

  // Create threads to sort each partition into radix buckets.
  std::vector<std::thread> pushToRadixBucketThreads;
  for (unsigned int i = 0; i <= numPartitions; ++i) {
    pushToRadixBucketThreads.push_back(std::thread(push_to_radix_buckets, i));
  }

  // Wait for threads to finish before closing them.
  for (auto& t : pushToRadixBucketThreads) t.join();

  // Update m partition size to sort by partition each of the radix buckets.
  m = radixBuckets.size()/numPartitions;

  // This function works similarly to the push_to_radix_buckets() function
  // except that instead of partitioning to push to a radix bucket, it partitions
  // the radix buckets vector to sort each bucket.
  auto sort_each_radix_bucket = [&](unsigned int radixPartitionNum) {
    try {
       // Retrieve start and end points of the current partition number.
       unsigned int startPos, endPos;
       startPos = get_partition_pos(radixPartitionNum);
       // Check end case.
       if (radixPartitionNum == numPartitions) {
         endPos = radixBuckets.size() - 1;
       } else {
         endPos = get_partition_pos(radixPartitionNum + 1) - 1;
       }

       // Sort each of the buckets that are contained within the start and
       // end points calculated.
       for (unsigned int i = startPos; i <= endPos; ++i) {
         std::vector<unsigned int> currBucket = radixBuckets.at(i);
         std::sort(currBucket.begin(), currBucket.end(),
                [](const unsigned int& x, const unsigned int& y) {
                  return aLessB(x, y, 0);
                });

         // Write operation to shared resource - radixBuckets vector. Lock until
         // write process is complete.
         locked_scope(radixBucketsLock, [&] {
           radixBuckets[i] = currBucket;
         });
       }
    } catch (std::runtime_error &e) {
      std::cout << "Exception caught: " << e.what() << std::endl;
    }
  };

  // Create threads to sort each radix bucket in partitions.
  std::vector<std::thread> sortingThreads;
  for (unsigned int i = 0; i < numPartitions; ++i) {
    sortingThreads.push_back(std::thread(sort_each_radix_bucket, i));
  }

  // Wait for threads to finish before closing them.
  for (auto& t : sortingThreads) t.join();

  // Merge buckets.
  // First clear the existing numbers array.
  numbersToSort.clear();
  // Insert all the numbers back in, in the order of the buckets.
  for (auto& bucket : radixBuckets) {
    for (auto& num : bucket) {
      numbersToSort.push_back(num);
    }
  }
}
