#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include "HashTable.h"

using namespace std;



int main()
{
	int num1, num2;
	string item_not_found = "", search_str = "";
	HashTable<string> table(item_not_found,4096);
	ifstream deneme("compout");
	ofstream sonuc("decompout");
	deneme >> num1;			// Get the first number to start decompressing from second number
	sonuc << table.arr[num1].element;	// Directly output the first number in the input file 
	while (deneme >> num2)
	{
		search_str = "";
		if (table.arr[num2].element != "" && table.isActive(num2))		// If position num2 is ACTIVE,
		{
			search_str = table.arr[num1].element + table.arr[num2].element[0];
		}
		else		// If position num2 is not ACTIVE
		{
			search_str = table.arr[num1].element + table.arr[num1].element[0];
		}
		table.insert(search_str);
		sonuc << table.arr[num2].element;
		num1 = num2;
	}
	deneme.close();
	sonuc.close();

	return 0;
}