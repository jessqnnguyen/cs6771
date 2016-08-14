/*
 * File: WordLadder.cpp
 * --------------------
 * Name: Jessica Nguyen [z5018882]
 * Assignment #1.
 */

#include "genlib.h"
#include "lexicon.h"
#include <queue>
#include <vector>
#include <iostream>
#include <set>
#include <algorithm>
#include <map>

struct Node {
		vector<std::string> words;
};

vector<vector<std::string>> getAllOptimalWordLadders(const std::string &startWord,
																										 const std::string &destWord);

void printWordLadders(const vector<vector<std::string>> &wordLadders);

void pushChildWordsToQueue(const vector<std::string> &wordLadder,
													 const std::string &destWord,
													 Lexicon &english,
													 Lexicon *visitedWords,
													 std::map<std::string, unsigned int> *visitedWordsLength,
												   unsigned int *lastIndexModified,
													 queue<Node> *q);

vector<std::string> getAllValidChildWords(const std::string &word,
	       																	const std::string &destWord,
																					Lexicon &english,
																				  unsigned int *lastIndexModified);

int main() {
	// Obtain input.
	std::string startWord, destWord;
  std::cout << "Enter start word (RETURN to quit): ";
	std::cin >> startWord;
	std::cout << "Enter destination word: ";
	std::cin >> destWord;

	printWordLadders(getAllOptimalWordLadders(startWord, destWord));

	return EXIT_SUCCESS;
}

/**
* Member function: getAllOptimalWordLadders
* ----------------------------------------
* Given a start and an destination word, obtains all optimal word ladders
* that get from the start word to the destination word by modifying only one
* letter at a time. Optimal word ladders are found through a BFS search strategy.
*/
vector<vector<std::string>> getAllOptimalWordLadders(const std::string &startWord,
																										 const std::string &destWord) {
  // Store optimal word ladders that have reached the destination word
 	// and the length of the shortest path word ladders.
  vector<vector<std::string>> optimalWordLadders;

	// Optimisation: Store all the words already traversed in an earlier ladder
 	// in a set and don't expand words already expanded.
  Lexicon visitedWords;
 	Lexicon english("EnglishWords.dat");

 	// Store in a map all words visited to what ladder length
 	// the word was visited at.
  std::map<std::string, unsigned int> visitedWordsLength;

 	// Create a node with the start word ladder.
 	vector<std::string> startWordLadder{startWord};

	// Store some optimisation variables to avoid exploring redundant paths and
	// assist with early exit strategy.
	unsigned int lastIndexModified = -1, shortestPathLength = 0;

 	// Mark start word as visited.
 	visitedWords.add(startWord);
 	visitedWordsLength.insert(std::pair<std::string, unsigned int>
													 (startWord, startWordLadder.size()));

	// Create a queue for BFS.
 	queue<Node> q;
	Node n;
  pushChildWordsToQueue(startWordLadder, destWord, english,
 		 										&visitedWords, &visitedWordsLength, &lastIndexModified, &q);

 	// Commence BFS for destination word.
 	while (!q.empty()) {
 		// Dequeue node and remove from the queue.
 		n = q.front();
 		q.pop();
 		// Early exit
 		if (!optimalWordLadders.empty() && n.words.size() > shortestPathLength
 				&& shortestPathLength != 0) {
 			break;
 		} else {
 			// If destination word has been reached, push node onto
 			// optimal nodes queue.
 			if (n.words[n.words.size()-1] == destWord) {
 				shortestPathLength = n.words.size();
 				optimalWordLadders.push_back(n.words);
 			}
 			// Get all child nodes to current node, that is all nodes which continue
 			// the word ladder.
 			pushChildWordsToQueue(n.words, destWord, english, &visitedWords,
 														&visitedWordsLength, &lastIndexModified, &q);
 		}
 	}

	// Sort all optimal word ladders in alphabetical order in terms
	// of traversal through the word ladder.
	std::sort(optimalWordLadders.begin(), optimalWordLadders.end());
	optimalWordLadders.erase(std::unique(optimalWordLadders.begin(),
																			 optimalWordLadders.end()),
																			 optimalWordLadders.end());
	return optimalWordLadders;
}

/**
* Member function: printWordLadders
* ----------------------------------------
* Given a vector of word ladders, prints all word ladders to the console
* in assignment spec format. If empty, prints "No ladder found.".
*/
void printWordLadders(const vector<vector<std::string>> &wordLadders) {
	// Print final output of BFS.
	if (wordLadders.empty()) {
		std::cout << "No ladder found." << std::endl;
	} else {
		std::cout << "Found ladder: ";
		for (const auto wordLadder : wordLadders) {
			for (const auto word : wordLadder) {
				std::cout << word << " ";
			}
			std::cout << std::endl;
		}
	}
}

/**
* Member function: pushChildWordsToQueue
* ----------------------------------------
* This is a helper function to assist the BFS with finding all the child word
* ladders and push it to the queue.
* This member function, given a current word ladder to expand from and a queue
* will find all valid child word ladders and push that word ladder to the given
* queue.
* The destination word string, a Lexicon storing all the previously
* visited words, a map storing at which word ladder length that word was
* visited at and the last index modified to get the word at the end of the word
* ladder (i.e. the word to expand from) is also passed in to assist with
* early exit optimisation strategies and avoid expanding redundant path.
*/
void pushChildWordsToQueue(const vector<std::string> &wordLadder,
													 const std::string &destWord,
													 Lexicon &english,
													 Lexicon *visitedWords,
													 std::map<std::string, unsigned int> *visitedWordsLength,
												   unsigned int *lastIndexModified, queue<Node> *q) {
	vector<std::string> currWordLadder = wordLadder;
	Node currNode{wordLadder};
	std::string fromWord = wordLadder[wordLadder.size()-1];

	// Load up the queue with all word ladders of all the valid words
	// which differ from the start word by one letter.
	vector<std::string> validChildWords = getAllValidChildWords(fromWord, destWord,
																										english, lastIndexModified);
	for (const auto childWord : validChildWords) {
			currWordLadder.push_back(childWord);
			currNode.words = currWordLadder;
			bool wordHasNotBeenVisited = !visitedWords->containsWord(childWord);
			if (wordHasNotBeenVisited) {
					q->push(currNode);
					visitedWords->add(childWord);
					visitedWordsLength->insert(std::pair<std::string, unsigned int>
																		(childWord, currWordLadder.size()));
			} else {
				// Optimisation: If the word has been visited before check if it was
				// at a longer ladder, if so visit however if word was visited before
				// at shorter ladder, don't visit. This optimisation avoids the BFS
				// visiting redudant paths.
				bool wordWasVisitedAtALongerLadder = currWordLadder.size()
																						<= visitedWordsLength->at(childWord);
				if (wordWasVisitedAtALongerLadder) {
					q->push(currNode);
					// Update word was last visited at ladder size.
					(visitedWordsLength->find(childWord))->second = currWordLadder.size();
				}
			}
			if (childWord == destWord) {
				 break;
			}
			currWordLadder = wordLadder;
	}
}

/*
 * Member function: getAllValidChildWords
 * Usage: getAllValidChildWords("con", "cat",...")
 * ----------------------------------------
 * This is a helper function to assist the BFS with finding all child words.
 * This member function returns a vector of all strings
 * that are:
 * - only one letter different fom the given word
 * - a real English word (checked via the third parameter).
 * It also requires the destination word and the last index that was modified
 * in the current word by the last iteration of the expansion which is an
 * optimisation to assist the BFS via an early exit strategy.
 */
vector<std::string> getAllValidChildWords(const std::string &word,
	 																				const std::string &destWord,
																					Lexicon &english,
																					unsigned int *lastIndexModified) {
	vector<std::string> result;
	char c0;
	std::string newWord;
	for (unsigned int i = 0; i < word.size(); ++i) {
		 // If in the last iteration of the BFS expansion, the current index of the
		 // loop was modified to get the current word, skip to the next index of
		 // the word.
		 if (*lastIndexModified == i) {
			 continue;
		 }

		 c0 = word[i];
		 newWord = word;

		 // Early exit optimisation:
		 // If child word is the destination word, no need to check if the word is
		 // in the lexicon, empty the list of child words (since an optimal path to
		 // the destination word has been found, no need to push the other ladders
		 // along this path) and just push it to the newly empty list of child words
		 // and return early.
		 if (newWord == destWord) {
			 result = {};
			 result.push_back(newWord);
			 break;
		 }

		 for (auto c1 = 'a'; c1 <= 'z'; c1++) {
				 if (c1 != c0) {
					 newWord[i] = c1;
				 }
				 if (english.containsWord(newWord)) {
					 result.push_back(newWord);
					 *lastIndexModified = i;
				 }
		 }
	}
	return result;
}
