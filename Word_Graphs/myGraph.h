#ifndef _MYGRAPH
#define _MYGRAPH

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <set>


using namespace std;



class myGraph
{
	public:
		myGraph();
		myGraph(vector<string>);
		void Visit(string);
		void unVisit(string);
		bool CheckVisited(string);
		vector<string> GetListofWords(string);
	private:
		unordered_map<string, vector<string>> graphWords;
		unordered_map<string, bool> allVisits;
};

bool isExist(vector<string> words, string word);

#endif