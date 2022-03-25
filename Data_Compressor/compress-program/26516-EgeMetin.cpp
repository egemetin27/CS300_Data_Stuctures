#include <iostream>
#include <string>
#include <fstream>
#include "HashTable.h"

using namespace std;



int main()
{
	char ch;
	int len, pos;
	string as = "";
	HashTable<string> table(as,4096);
	ifstream deneme("compin");
	ofstream sonuc("compout");
	string search_str = "", longest_str;	// longest_str is the longest prefix that we have in our table while searching for new strings
	search_str += deneme.get();
	while (!deneme.eof())				// While not end of file, we will keep adding char combinations to the table
	{
		while (table.find(search_str) != "" && !deneme.eof())
		{
			deneme.get(ch);
			longest_str = search_str;
			search_str += ch;
		}
		table.insert(search_str);
		pos = table.findIt(longest_str);	// Position of longest prefix in the hash table to encode to output
		len = search_str.length();
		search_str = search_str[len- 1];	// Continue the loop from the last character 
		sonuc << pos << " ";
	}
	deneme.close();
	sonuc.close();
	return 0;
}