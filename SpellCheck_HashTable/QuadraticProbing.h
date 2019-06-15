
#ifndef QUADRATIC_PROBING_H
#define QUADRATIC_PROBING_H

#include <vector>
#include <string>
using namespace std;

int nextPrime(int n);
int hash1(const string & key);
int hash1(int key);

static int numColl = 0;
static int lonCLen = 0;
static double avgChainLen = 1.0;

int nextPrime(int n);

int hash1(const string & key);
int hash1(int key);

// QuadraticProbing Hash table class
//
// CONSTRUCTION: an approximate initial size or default of 101
//
// ******************PUBLIC OPERATIONS*********************
// bool insert( x )       --> Insert x
// bool remove( x )       --> Remove x
// bool contains( x )     --> Return true if x is present
// void makeEmpty( )      --> Remove all items
// int hash( string str ) --> Global method to hash strings

template <typename HashedObj>
class HashTable
{
public:
	explicit HashTable(int size = 101) : array(nextPrime(size))
	{
		makeEmpty();
	}

	bool contains(const HashedObj & x) const
	{
		return isActive(findPos(x));
	}

	void makeEmpty()
	{
		currentSize = 0;
		for (int i = 0; i < array.size(); i++)
			array[i].info = EMPTY;
	}

	bool insert(const HashedObj & x)
	{
		// Insert x as active
		int currentPos = findPosI(x);
		if (isActive(currentPos))
			return false;

		array[currentPos] = HashEntry(x, ACTIVE);

		// Rehash; see Section 5.5
		if (++currentSize > array.size() / 2)
			rehash();

		return true;
	}

	bool remove(const HashedObj & x)
	{
		int currentPos = findPos(x);
		if (!isActive(currentPos))
			return false;

		array[currentPos].info = DELETED;
		return true;
	}

	int get_currSize()
	{
		return currentSize;
	}

	int get_totalSize()
	{
		return array.size();
	}

	double get_loadFactor()
	{
		return double(currentSize) / array.size();
	}

	int get_NumCollisions()
	{
		// return numColl
		// I believe its half coz offset is incremented by 2
		return numColl / 2;
	}

	int get_LonChainLen()
	{
		// I believe its half coz offset is incremented by 2
		return lonCLen / 2;
	}

	double get_AvgCLen()
	{
		return avgChainLen / currentSize;
	}

	enum EntryType { ACTIVE, EMPTY, DELETED };

private:
	struct HashEntry
	{
		HashedObj element;
		EntryType info;

		HashEntry(const HashedObj & e = HashedObj(), EntryType i = EMPTY)
			: element(e), info(i) { }
	};

	vector<HashEntry> array;
	int currentSize;

	int longChainLen = 0;

	bool isActive(int currentPos) const
	{
		return array[currentPos].info == ACTIVE;
	}

	int findPos(const HashedObj & x) const
	{
		int offset = 1;
		int currentPos = myhash(x);

		// Assuming table is half-empty, and table length is prime,
		// this loop terminates
		while (array[currentPos].info != EMPTY &&
			array[currentPos].element != x)
		{
			currentPos += offset;  // Compute ith probe
			offset += 2;
			if (currentPos >= array.size())
				currentPos -= array.size();
		}

		return currentPos;
	}

	int findPosI(const HashedObj & x) const
	{
		int offset = 1;
		int currentPos = myhash(x);
		avgChainLen++;
		// Assuming table is half-empty, and table length is prime,
		// this loop terminates
		while (array[currentPos].info != EMPTY &&
			array[currentPos].element != x)
		{
			numColl++;
			avgChainLen++;
			currentPos += offset;  // Compute ith probe
			offset += 2;
			if (currentPos >= array.size())
			{
				currentPos -= array.size();
			}
			if (lonCLen < offset)
			{
				lonCLen = offset;
				++lonCLen;
			}
		}

		return currentPos;
	}

	void rehash()
	{
		numColl = 0;
		avgChainLen = 0.0;
		longChainLen = 0;
		vector<HashEntry> oldArray = array;

		// Create new double-sized, empty table
		array.resize(nextPrime(2 * oldArray.size()));
		for (int j = 0; j < array.size(); j++)
			array[j].info = EMPTY;

		// Copy table over
		currentSize = 0;
		for (int i = 0; i < oldArray.size(); i++)
			if (oldArray[i].info == ACTIVE)
				insert(oldArray[i].element);
	}
	int myhash(const HashedObj & x) const
	{
		int hashVal = hash1(x);

		hashVal %= array.size();
		if (hashVal < 0)
			hashVal += array.size();

		return hashVal;
	}
};


#endif
