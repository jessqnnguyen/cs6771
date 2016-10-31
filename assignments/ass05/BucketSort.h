#ifndef BUCKET_SORT_H
#define BUCKET_SORT_H

#include <vector>
#include <mutex>

struct BucketSort {

	// vector of numbers
	std::vector<unsigned int> numbersToSort;
  // std::vector<std::vector<unsigned int> radixBuckets;
	// unsigned int m, numPartitions;
	// unsigned int getPartitionPos(unsigned int partitionNum);
															//  unsigned int m,
															//  unsigned int numPartitions);
	// void pushToRadixBuckets(unsigned int partitionNum);
	template <typename F>
	void locked_scope(std::mutex& m, F f);
	// unsigned int getDigit(unsigned int num, unsigned int digit);
	// unsigned int getDigits(unsigned int num);
	void sort(unsigned int numCores);
};


#endif /* BUCKET_SORT_H */
