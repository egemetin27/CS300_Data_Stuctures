#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <stack>
#include "myGraph.h"
#include <queue>

using namespace std;

struct wordAndRoot
{
	string word;
	string root;
	int distance;
	wordAndRoot(string w, string r, int d) : word(w), root(r), distance(d)
	{}
};

void putWordsInside(string currentword, vector<string> list, vector<wordAndRoot>& db, int distance, string nextword)
{
	for (int a = 0; a < list.size(); a++)
	{
		if (list[a] != currentword && list[a] != nextword)
		{
			wordAndRoot wr(list[a], currentword, distance);
			db.push_back(wr);
		}
	}
}

bool checkAndFixShortestIfNot(string currentword, vector<wordAndRoot>& db, vector<string>& transformSteps, stack<string>& myStack, int &distance, myGraph& graph)
{
	for (int a = 0; a < db.size(); a++)
	{
		if (db[a].word == currentword )//&& distance - db[a].distance != 1)
		{
			string temp = myStack.top();
			myStack.pop();
			string rt = myStack.top();
			myStack.push(temp);
			string root = db[a].root;
			if (root != rt)
			{
				do
				{
					transformSteps.pop_back();
					myStack.pop();
					distance--;
				} while (myStack.top() != root);
				graph.unVisit(currentword);
				return true;
			}
		}
	}
	return false;
}


bool TransformWords(string A, string B, myGraph graphWords, vector<string>& transformSteps)
{
	if (A == B)
	{
		cout << "The words are the same, no transformations are needed." << endl;
		return true;
	}
	string derivated_word1, derivated_word2, current_word;
	stack<string> myStack;
	vector<wordAndRoot> database;
	int distance = 1;

	transformSteps.push_back(A);
	// If this word (A) is even popped during loop, then we could not find our transformation then we just exit from loop and return false
	myStack.push(A);
	graphWords.Visit(A);

	while (!myStack.empty())
	{
		bool check = true;
		string lastStep = "";
		//string lastWord = "";
		current_word = myStack.top();

		if (checkAndFixShortestIfNot(current_word, database, transformSteps, myStack, distance, graphWords))
			current_word = myStack.top();

		//distance--;
		myStack.pop();
		if (transformSteps.size() != 0)
		{
			lastStep = transformSteps[transformSteps.size() - 1];
			transformSteps.pop_back();
		}
		if (current_word == B)
		{
			transformSteps.push_back(lastStep);
			return true;
		}
		for (int a = 0; a < current_word.length(); a++)
		{
			// Looking for deletion or change of letter(swap)
			derivated_word1 = current_word.substr(0, a) + "*" + current_word.substr(a + 1);
			vector<string> list_word1 = graphWords.GetListofWords(derivated_word1);
			for (int b = 0; b < list_word1.size(); b++)
			{
				if (!graphWords.CheckVisited(list_word1[b]))
				{
					distance++;
					putWordsInside(current_word, list_word1, database, distance, list_word1[b]);
					graphWords.Visit(list_word1[b]);
					if (list_word1[b].length() == current_word.length())
					{
						// Pushing the current step to our Steps Vector 
						if (lastStep != "")
							transformSteps.push_back(lastStep);
						transformSteps.push_back(list_word1[b] + " (change " + current_word[a] + " at position " + to_string(a + 1) + " to " + list_word1[b][a] + ")");
					}
					else
					{
						if (lastStep != "")
							transformSteps.push_back(lastStep);
						transformSteps.push_back(list_word1[b] + " (delete " + current_word[a] + " at position " + to_string(a + 1) + ")");
					}
					check = false;
					//distance++;
					myStack.push(current_word);
					myStack.push(list_word1[b]);		// Pushing the current word to our stack to proceed with next transformation 
					break;
				}
			}
			if (check == false) break;

			// Looking for insertion
			derivated_word2 = current_word.substr(0, a + 1) + "*" + current_word.substr(a + 1);
			vector<string> list_word2 = graphWords.GetListofWords(derivated_word2);
			for (int b = 0; b < list_word2.size(); b++)
			{
				if (!graphWords.CheckVisited(list_word2[b])) {
					distance++;
					putWordsInside(current_word, list_word2, database, distance, list_word2[b]);
					graphWords.Visit(list_word2[b]);
					if (lastStep != "")
						transformSteps.push_back(lastStep);
					transformSteps.push_back(list_word2[b] + " (insert " + list_word2[b][a + 1] + " after position " + to_string(a + 1) + ")");
					//distance++;
					myStack.push(current_word);
					myStack.push(list_word2[b]);
					check = false;
					break;
				}
			}
			if (check == false)
				break;
		}

		// For inserting to the first position
		vector<string> list_word = graphWords.GetListofWords("*" + current_word);
		for (int j = 0; j < list_word.size() && check; j++)
		{
			if (!graphWords.CheckVisited(list_word[j]))
			{
				distance++;
				putWordsInside(current_word, list_word, database, distance, list_word[j]);
				graphWords.Visit(list_word[j]);
				if (lastStep != "")
					transformSteps.push_back(lastStep);
				transformSteps.push_back(list_word[j] + " (insert " + list_word[j][0] + " after position 0)");
				//distance++;
				myStack.push(current_word);
				myStack.push(list_word[j]);
				break;
			}
		}

	}
	return false;
}

// Checks if the word is in Word List
bool isExists(vector<string> words, string word)
{
	int size = words.size();
	for (int a = 0; a < size; a++)
	{
		if (words[a] == word) return true;
	}
	return false;
}


int main()
{
	vector<string> wordsList;
	string word, A, B;
	ifstream file_input("words.txt");

	// Fill our wordList vector with words from dictionary
	while (file_input >> word)
	{
		wordsList.push_back(word);
	}
	file_input.close();
	
	myGraph graphWords(wordsList);		// Initialize the graph with all connections of words inside
	
	while (1)
	{
		vector<string> transform;		// Delete the old content if there is (from previous search)
		cout << "Please enter the first word: ";
		cin >> A;
		cout << "Please enter the second word: ";
		cin >> B;
		if (A[0] == '*')
		{
			cout << "The program is terminating..." << endl;
			return 0;
		}
		if (!isExists(wordsList, A) || !isExists(wordsList, B))
			cout << "Either one of the words is not in the words list." << endl;
		else
		// If both words exits in the words list
		{
			if (TransformWords(A, B, graphWords, transform) == false)
				cout << "There is no such sequence of one letter transformations between " << A << " and " << B << "." << endl;
			else
			// If there is such transformation between words A and B
			{
				int size = transform.size();
				for (int a = 0; a < size; a++)
				{
					cout << transform[a] << endl;
				}
			}
		}
	}

	return 0;
}
