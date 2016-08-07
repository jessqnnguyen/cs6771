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

class Node {
	public:
		vector<string> words;
		Node(vector<string> wl);
};

vector<string> getAllValidChildWords(string word, string destWord);
void pushChildWordsToQueue(vector<string> wl, string destWord,
													 std::set<string> *visitedWords, queue<Node> &q);

Node::Node (vector<string> wl) {
	words = wl;
}
int main() {

	// [TODO: fill in the code]
	string startWord, destWord;
  std::cout << "Enter start word (RETURN to quit): ";
	std::cin >> startWord;
	std::cout << "Enter destination word: ";
	std::cin >> destWord;

	// Optimisation: Store all the words already traversed in an earlier ladder
	// in a set and don't expand words already expanded.
	std::set<string> visitedWords;

	// Create a queue for BFS.
	queue<Node> q;
	vector<string> startWordLadder{startWord};
	Node n(startWordLadder);
  pushChildWordsToQueue(startWordLadder, destWord,  &visitedWords, q);

	// Store optimal nodes that have reached the destination word in their
	// word ladder and length of shortest path word ladders.
  vector<Node> optimalNodes;
	unsigned int shortestPathLength = 0;

	// Commence BFS for destination word.
	while (!q.empty()) {
		// Dequeue node and remove from the queue.
		n = q.front();
		q.pop();

		// Early exit
		if (!optimalNodes.empty() && n.words.size() > shortestPathLength) {
			break;
		} else {
			// If destination word has been reached, push node onto
			// optimal nodes queue.
			if (n.words[n.words.size()-1] == destWord) {
				shortestPathLength = n.words.size();
				optimalNodes.push_back(n);
			}
			// Get all child nodes to current node, that is all nodes which continue
			// the word ladder.
			pushChildWordsToQueue(n.words, destWord, &visitedWords, q);
		}
	}
  std::cout << "queue size: " << q.size() << std::endl;

	if (optimalNodes.empty()) {
		std::cout << "No ladder found." << std::endl;
	} else {
		// Print found optimal nodes
		std::cout << "Found ladder: ";
		for (Node n : optimalNodes) {
			for (string word : n.words) {
				std::cout << word << " ";
			}
			std::cout << std::endl;
		}
	}
	return 0;
}

void pushChildWordsToQueue(vector<string> wordLadder, string destWord,
													 std::set<string> *visitedWords, queue<Node> &q) {
  vector<string> currWordLadder = wordLadder;
	Node currNode(wordLadder);
	string fromWord = wordLadder[wordLadder.size()-1];
	// [TODO: Remove these debug print statements when no longer needed.]
	std::cout << "Visted words size = " << visitedWords->size() << std::endl;
	std::cout << "Expanding child nodes and pushing to queue..." << std::endl;
	// Load up the queue with all word ladders of all the words
	// which differ from the start word by one letter.
	for (string childWord : getAllValidChildWords(fromWord, destWord)) {
			currWordLadder.push_back(childWord);
			currNode.words = currWordLadder;
			if (visitedWords->count(childWord) == 0) {
					q.push(currNode);
					std::cout << "Pushing current node to queue: ";
					for (string word : currNode.words) {
						std::cout << word << " ";
					}
					std::cout << std::endl;
					if (childWord == destWord) {
						 break;
					}
					visitedWords->insert(childWord);
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
vector<string> getAllValidChildWords(string word, string destWord) {
	Lexicon english("EnglishWords.dat");
	// Lexicon english("SmallDictionary.txt");
	vector<string> result;
	char c0;
	string newWord;
	for (unsigned int i = 0; i < word.size(); ++i) {
		 c0 = word[i];
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
		 for (char c1 = 'a'; c1 <= 'z'; c1++) {
				 if (c1 != c0) {
					 newWord[i] = c1;
				 }
				 if (english.containsWord(newWord)) {
					 result.push_back(newWord);
					//  std::cout << newWord << std::endl;
				 }
		 }
	}
	return result;
}
