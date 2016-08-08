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
		vector<std::string> words;
		Node(vector<std::string> wl);
};

vector<std::string> getAllValidChildWords(std::string word, std::string destWord);
void pushChildWordsToQueue(vector<std::string> wordLadder, std::string destWord,
													 std::set<string> *visitedWords, queue<Node> *q);

Node::Node (vector<string> wl) {
	words = wl;
}
int main() {

	std::string startWord, destWord;
  std::cout << "Enter start word (RETURN to quit): ";
	std::cin >> startWord;
	std::cout << "Enter destination word: ";
	std::cin >> destWord;

	// Optimisation: Store all the words already traversed in an earlier ladder
	// in a set and don't expand words already expanded.
	std::set<std::string> visitedWords;
	// Mark start word as visited.
	visitedWords.insert(startWord);

	// Create a queue for BFS.
	queue<Node> q;
	vector<std::string> startWordLadder{startWord};
	Node n(startWordLadder);
  pushChildWordsToQueue(startWordLadder, destWord,  &visitedWords, &q);

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
		if (!optimalNodes.empty() && n.words.size() >= shortestPathLength) {
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
			pushChildWordsToQueue(n.words, destWord, &visitedWords, &q);
		}
	}
  std::cout << "queue size: " << q.size() << std::endl;

	if (optimalNodes.empty()) {
		std::cout << "No ladder found." << std::endl;
	} else {
		// Print found optimal nodes
		std::cout << "Found ladder: ";
		for (Node n : optimalNodes) {
			for (std::string word : n.words) {
				std::cout << word << " ";
			}
			std::cout << std::endl;
		}
	}
	return 0;
}

void pushChildWordsToQueue(vector<std::string> wordLadder, std::string destWord,
													 std::set<std::string> *visitedWords, queue<Node> *q) {
  vector<std::string> currWordLadder = wordLadder;
	Node currNode(wordLadder);
	std::string fromWord = wordLadder[wordLadder.size()-1];
	// [TODO: Remove these debug print statements when no longer needed.]
	std::cout << "Visted words size = " << visitedWords->size() << std::endl;
	std::cout << "Expanding child nodes and pushing to queue..." << std::endl;
	// Load up the queue with all word ladders of all the words
	// which differ from the start word by one letter.
	for (std::string childWord : getAllValidChildWords(fromWord, destWord)) {
			currWordLadder.push_back(childWord);
			currNode.words = currWordLadder;
			if (visitedWords->count(childWord) == 0) {
				  std::cout << "Word to be pushed to the end of the current word ladder: "
					<< childWord << " has been visited " << visitedWords->count(childWord)
					<< std::endl;
					q->push(currNode);
					std::cout << "Pushing current node to queue: ";
					for (string word : currNode.words) {
						std::cout << word << " ";
					}
					std::cout << std::endl;
					visitedWords->insert(childWord);
					if (childWord == destWord) {
						 break;
					}
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
vector<std::string> getAllValidChildWords(std::string word, std::string destWord) {
	Lexicon english("EnglishWords.dat");
	// Lexicon english("SmallDictionary.txt");
	vector<std::string> result;
	char c0, destC;
	std::string newWord;
	for (unsigned int i = 0; i < word.size(); ++i) {
		 c0 = word[i];
		 destC = destWord[i];
		 if (c0 == destC) {
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
