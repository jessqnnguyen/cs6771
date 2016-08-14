/*
 * File: WordLadder.cpp
 * --------------------
 * Name: [TODO: enter name here]
 * This file is the starter project for the word ladder problem on Assignment #1.
 */

#include "genlib.h"
#include "lexicon.h"
#include <queue>
#include <vector>
#include <iostream>
#include <set>
#include <algorithm>
#include <map>
// [TODO: To remove.]
#include <fstream>

class Node {
	public:
		vector<std::string> words;
		Node(vector<std::string> wl);
};

void pushChildWordsToQueue(const vector<std::string> currWordLadder, const std::string &destWord,
													 Lexicon &english, Lexicon &visitedWords,
													 std::map<std::string, unsigned int> &visitedWordsLength,
													 queue<Node> &q,
												   unsigned int &lastIndexModified);

vector<std::string> getAllValidChildWords(const std::string &word,
	       																	const std::string &destWord,
																					Lexicon &english,
																				  unsigned int &lastIndexModified);

Node::Node (vector<string> wl) {
	words = wl;
}
// [TODO: To remove.]
ofstream output;

int main() {
	// [TODO: To remove.]
	output.open("output.txt");

	std::string startWord, destWord;
  std::cout << "Enter start word (RETURN to quit): ";
	std::cin >> startWord;
	std::cout << "Enter destination word: ";
	std::cin >> destWord;

	// Optimisation: Store all the words already traversed in an earlier ladder
	// in a set and don't expand words already expanded.
	Lexicon visitedWords;
	Lexicon english("EnglishWords.dat");
  std::map<std::string, unsigned int> visitedWordsLength;

	// Create a queue for BFS.
	queue<Node> q;

	// Create a node with the start word ladder.
	vector<std::string> startWordLadder{startWord};
	unsigned int lastIndexModified = -1;
	// Mark start word as visited.
	visitedWords.add(startWord);
	visitedWordsLength.insert(std::pair<std::string, unsigned int>(startWord, startWordLadder.size()));
	Node n(startWordLadder);
  pushChildWordsToQueue(startWordLadder, destWord, english,
		 										visitedWords, visitedWordsLength, q, lastIndexModified);

	// Store optimal word ladders that have reached the destination word
	// and the length of the shortest path word ladders.
  vector<vector<std::string>> optimalWordLadders;
	unsigned int shortestPathLength = 0;
	// Commence BFS for destination word.
	while (!q.empty()) {
		// Dequeue node and remove from the queue.
		n = q.front();
		q.pop();
		output << "Popping node off queue, current word ladder is: ";
		for (auto word : n.words) {
			output << word << " ";
		}
		output << std::endl;
		// Early exit
		if (!optimalWordLadders.empty() && n.words.size() > shortestPathLength
				&& shortestPathLength != 0) {
			break;
		} else {
			// If destination word has been reached, push node onto
			// optimal nodes queue.
			if (n.words[n.words.size()-1] == destWord) {
				shortestPathLength = n.words.size();
				// [TODO: To remove.]
				// output << "pushing optimal ladder to optimal nodes: " << std::endl;
				// for (std::string word : n.words) {
				// 		output << word << " ";
			  // }
				// output << std::endl;
				optimalWordLadders.push_back(n.words);
			}
			// Get all child nodes to current node, that is all nodes which continue
			// the word ladder.
			pushChildWordsToQueue(n.words, destWord, english, visitedWords,
														visitedWordsLength, q, lastIndexModified);
		}
	}

	// Sort all optimal word ladders to print them in alphabetical order in terms
	// of traversal through the word ladder.
  std::sort(optimalWordLadders.begin(), optimalWordLadders.end());
	optimalWordLadders.erase(std::unique(optimalWordLadders.begin(),
																			 optimalWordLadders.end()),
																			 optimalWordLadders.end());

	if (optimalWordLadders.empty()) {
		std::cout << "No ladder found." << std::endl;
	} else {
		// Print found optimal nodes
		std::cout << "Found ladder: ";
		for (auto wordLadder : optimalWordLadders) {
			for (std::string word : wordLadder) {
				std::cout << word << " ";
			}
			std::cout << std::endl;
		}
	}
	return EXIT_SUCCESS;
}

// [TODO: To remove this block when no longer needed.]
// bool sortWordLadder(const vector<std::string>& vec1, const vector<std::string>& vec2){
//     for(size_t i = 0; i < vec1.size() && i < vec2.size(); i++){
//          if(vec1[i] > vec2[i]){
//              return false;
//          } else if(vec1[i] < vec2[i]){
//              return true;
//          }
//     }
//     return false;
// }

void pushChildWordsToQueue(const vector<std::string> wordLadder, const std::string &destWord,
													 Lexicon &english, Lexicon &visitedWords,
													 std::map<std::string, unsigned int> &visitedWordsLength,
													 queue<Node> &q,
												   unsigned int &lastIndexModified) {
  vector<std::string> currWordLadder = wordLadder;
	Node currNode(wordLadder);
	std::string fromWord = wordLadder[wordLadder.size()-1];
	// [TODO: Remove these debug print statements when no longer needed.]
	// std::cout << "Visted words size = " << visitedWords.size() << std::endl;
	// std::cout << "Expanding child nodes and pushing to queue..." << std::endl;
	// if (wordLadder.at(wordLadder.size() - 1) == "cot") {
		// std::cout << "we're at cot!! here are the valid child words: " << std::endl;
		// for (std::string s : getAllValidChildWords(fromWord, destWord, english, lastIndexModified)) {
			// std::cout << s << " ";
		// }
		// std::cout << std::endl;
	// }
	// Load up the queue with all word ladders of all the words
	// which differ from the start word by one letter.
	for (const auto childWord : getAllValidChildWords(fromWord, destWord, english, lastIndexModified)) {
			currWordLadder.push_back(childWord);
			currNode.words = currWordLadder;
			if (!visitedWords.containsWord(childWord)) {
				  // [TODO: To remove.]
				  // std::cout << "Word to be pushed to the end of the current word ladder: "
					// << childWord << std::endl;
					q.push(currNode);
					// [TODO: To remove.]
					// output << "Pushing to queue a word visited for the first time: " << childWord << std::endl;
					// output << "Word ladder is:";
					// std::cout << "Pushing current node to queue: ";
					// for (string word : currNode.words) {
					// 	output << word << " ";
					// }
					// output << std::endl;
					visitedWords.add(childWord);
					visitedWordsLength.insert(std::pair<std::string, unsigned int>(childWord, currWordLadder.size()));
			} else {
				if (currWordLadder.size() <= visitedWordsLength.at(childWord)) {
					// [TODO: To remove.]
					// output << "Pushing an already visited word where current word ladder size is "
					//  << currWordLadder.size() << " and it was previously visited at a word ladder size of "
					//  << visitedWordsLength.size() << std::endl;
				  // output << "Pushing to queue: ";
					// for (string word : currNode.words) {
					// 	output << word << " ";
					// }
					// output << std::endl;
					q.push(currNode);
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
 * Usage: getAllValidChildWords("hello")..
 * ----------------------------------------
 * This member function returns a vector of all strings
 * which are only one letter different fom the given word
 * and is a real English word.
 */
vector<std::string> getAllValidChildWords(const std::string &word,
	 																				const std::string &destWord,
																					Lexicon &english,
																					unsigned int &lastIndexModified) {
	vector<std::string> result;
	char c0;
	std::string newWord;
	for (auto i = 0; i < word.size(); ++i) {
		 c0 = word[i];
		 if (lastIndexModified == i) {
			 continue;
		 }
		 newWord = word;
		 // Early exit optimisation:
		 // If child word is the destination word, no need to
		 // check if the word is in the lexicon, empty the list of child words
		 // (since an optimal path to the destination word has been found, no need
		 // to push the other ladders along this path) and
		 // just push it to the newly empty list of child words and return early.
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
					 lastIndexModified = i;
					//  std::cout << newWord << std::endl;
				 }
		 }
	}
	return result;
}
