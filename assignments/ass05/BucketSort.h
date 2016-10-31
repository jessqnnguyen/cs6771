#ifndef BUCKET_SORT_H
#define BUCKET_SORT_H

#include <vector>
#include <mutex>

struct BucketSort {

	// vector of numbers
	std::vector<unsigned int> numbersToSort;
	template <typename F>
	void locked_scope(std::mutex& m, F f);
	void sort(unsigned int numCores);
};

#endif /* BUCKET_SORT_H */
