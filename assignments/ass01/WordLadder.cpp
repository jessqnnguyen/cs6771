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

class Node {
	public:
		vector<string> words;
		Node(vector<string> wl);
};

vector<string> getAllValidChildWords(string word);
void pushChildWordsToQueue(vector<string> wl, string destWord, queue<Node> &q);

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

	// Create a queue for BFS.
	queue<Node> q;
	vector<string> startWordLadder{startWord};
	Node n(startWordLadder);
  pushChildWordsToQueue(startWordLadder, destWord, q);
	// std::cout << q.size() << std::endl;

	// Commence BFS for destination word.
	while (!q.empty()) {
		// Dequeue node and remove from the queue.
		n = q.front();
		q.pop();
    if (n.words[n.words.size()-1] == destWord) {
			break;
		}
		// Get all child nodes to current node, that is all nodes which continue
		// the word ladder.
		pushChildWordsToQueue(n.words, destWord, q);

	}

	std::cout << "Found ladder: ";
	for (string word : n.words) {
		std::cout << word << " ";
	}
	std::cout << std::endl;

	return 0;
}

void pushChildWordsToQueue(vector<string> wordLadder,
													 string destWord, queue<Node> &q) {
  vector<string> currWordLadder = wordLadder;
	Node currNode(wordLadder);
	string fromWord = wordLadder[wordLadder.size()-1];
	// Load up the queue with all word ladders of all the words
	// which differ from the start word by one letter.
	for (string childWord : getAllValidChildWords(fromWord)) {
			currWordLadder.push_back(childWord);
			currNode.words = currWordLadder;
			q.push(currNode);
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
vector<string> getAllValidChildWords(string word) {
	Lexicon english("EnglishWords.dat");
	vector<string> result;
	char c0;
	string newWord;
	for (unsigned int i = 0; i < word.size(); ++i) {
		 c0 = word[i];
		 newWord = word;
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
