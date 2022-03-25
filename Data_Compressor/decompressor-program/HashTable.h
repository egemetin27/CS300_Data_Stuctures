#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <iostream>
#include <vector>
#include <cmath>
#include <string>

using namespace std;

template <class HashedObj>
class HashTable
{
public:
	HashTable(const HashedObj &notFound, int size);
	HashTable(const HashTable &rhs) : ITEM_NOT_FOUND(rhs.ITEM_NOT_FOUND), arr(rhs.arr), currentSize(rhs.currentSize) {}

	const HashedObj &find(const HashedObj &x) const;	// Find the HashedObj and return it (if not exist, return ITEM_NOT_FOUND)
	int findPos(const HashedObj &x) const;				// Find the position of HashedObj and return it
	int findIt(const HashedObj& x) const;
	void insert(const HashedObj &x);					// Insert the HashedObj to our hash vector as HashEntry
	enum EntryType { ACTIVE, EMPTY, DELETED };
	bool isActive(int currentPos) const;
	template <class HashedObj>
	struct HashEntry
	{
		HashedObj element;
		EntryType info;

		HashEntry(EntryType i = EMPTY)
			: element(), info(i) { }

		HashEntry(HashedObj elmt, EntryType i) : element(elmt), info(i) {}
	};
	vector<HashEntry<HashedObj>> arr;
private:

	int currentSize;
	const HashedObj ITEM_NOT_FOUND;

	
};

int myHash(string key, int tableSize)
{
	//int len = key.length(), sum = 0;        // char icin length fonksiyonu bozulur!!!!!!
	//for (int i = 0; i < len; i++)
	//{
	//	sum += key[i] * pow(3, i);
	//}
	return (256 % tableSize);
}

#include "HashTable.cpp"

#endif
