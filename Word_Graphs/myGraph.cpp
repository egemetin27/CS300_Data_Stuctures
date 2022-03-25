#include "myGraph.h"



myGraph::myGraph()
{

}

/*
myGraph::myGraph(vector<string> wordList)
{
	int size = wordList.size();
	string word;
	for (int a = 0; a < size; a++)
	{
		word = wordList[a];
		for (int b = 0; b < word.length(); b++)
		{
			// For deletions and char changes(swaps)
			string str = word.substr(0, b) + "-" + word.substr(b + 1);
			graphWords[str].push_back(word);

			// For insertions
			string str2 = word.substr(0, b + 1) + "-" + word.substr(b + 1);
			graphWords[str2].push_back(word);
		}
	}
}*/
myGraph::myGraph(vector<string> wordList)
{
	for (auto it = wordList.begin(); it != wordList.end(); it++)
	{
		string word = *it;
		for (int j = 0; j < word.size(); j++)
		{
			string word1 = word.substr(0, j) + word.substr(j + 1);
			string str = word.substr(0, j) + "*" + word.substr(j + 1);
			graphWords[str].push_back(word);
			if (isExist(wordList, word1))
				graphWords[str].push_back(word1);
			string str2 = word.substr(0, j + 1) + "*" + word.substr(j + 1);
			graphWords[str2].push_back(word);
		}
		graphWords[word.substr(1)].push_back(word);
	}
}
void myGraph::Visit(string word)
{
	allVisits[word] = true;
}

void myGraph::unVisit(string word)
{
	allVisits[word] = false;
}

bool myGraph::CheckVisited(string word)
{
	return allVisits[word];
}


vector<string> myGraph::GetListofWords(string word)
{
	return graphWords[word];
}

bool isExist(vector<string> words, string word)
{
	int size = words.size();
	for (int a = 0; a < size; a++)
	{
		if (words[a] == word) return true;
	}
	return false;
}