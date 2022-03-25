#include <iostream>
#include "HashTable.h"

using namespace std;

template <class HashedObj>
HashTable<HashedObj>::HashTable(const HashedObj &notFound, int size)
	: ITEM_NOT_FOUND(notFound), currentSize(size)
{
	int i = 0;
	for (; i < 256; i++)
	{
		string a = "";
		a += char(i);
		arr.push_back(HashEntry<HashedObj>(a, ACTIVE));
	}
	for (; i < 4096; i++)
	{
		arr.push_back(HashEntry<HashedObj>(ITEM_NOT_FOUND, EMPTY));
	}
}


template <class HashedObj>
int HashTable<HashedObj>::findPos(const HashedObj &x) const
{
	int p = myHash(x, 4096);          // current position

	while (arr[p].info != EMPTY && arr[p].element != x)     // if p th index is not available, perform linear probing (p++)
	{
		p++;
		if (p >= currentSize)             // perform the mod
			p -= currentSize;                // if necessary
	}
	return p;
}


template <class HashedObj>
int HashTable<HashedObj>::findIt(const HashedObj& x) const
{
	int p = 0;          // current position

	while (arr[p].element != x)     // if p th index is not available, perform linear probing (p++)
	{
		p++;
		if (p >= currentSize)             // perform the mod
			p -= currentSize;                // if necessary
	}
	return p;
}

template <class HashedObj>
const HashedObj &HashTable<HashedObj>::find(const HashedObj &x) const
{
	int currentPos = findPos(x);

	if (isActive(currentPos))
		return arr[currentPos].element;

	return   ITEM_NOT_FOUND;
}


template <class HashedObj>
bool HashTable<HashedObj>::isActive(int currentPos) const
{
	return arr[currentPos].info == ACTIVE;
}


template <class HashedObj>
void HashTable<HashedObj>::insert(const HashedObj &x)
{
	int currentPos = findPos(x);

	if (isActive(currentPos))
		return;

	// Insert x as active
	arr[currentPos] = HashEntry<HashedObj>(x, ACTIVE);
}

