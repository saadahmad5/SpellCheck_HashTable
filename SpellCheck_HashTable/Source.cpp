#include <iostream>
#include <fstream>
#include <string.h>
#include <string>
#include <sstream>
#include <iomanip>
#include "QuadraticProbing.h"
using namespace std;

/*
* Name: Saad Ahmad
* Course: CIS 350
* Project 2 - Hash Table - Spell Dictionary
* The program requires 3 arguments; number of words, dictionary file and
* text file. It is implemented with a Hash table and it reads a dictionary file
* which has thousands of words and it inserts them into a hash table.
* Furthermore, it provides the statistical metadata also. Then it
* reads a regular text file, cancels out each and every punctuation letters
* and then compares all the words with hash table. If word is fine, no output
* comes, but if the word is not detected, it is lowercased and checked again.
* Still not found, the program has 4 spell check techniques and the word go
* through it and provides suggestions for replacements.
*
*/



string toLowerCase(string word);

// Simple main
int main(int argc, char* argv[])
{
	cout << "CIS350 Project 2" << endl << endl;

	// Arguments provided by command line 
	if (argc == 4)
	{
		// 0th in Visual Studio is program name itself

		cout << "Number of words: " << argv[1] << endl
			<< "Dictionary file: " << argv[2] << endl
			<< "Text file name : " << argv[3] << endl << endl;

		int numWords;
		string dictFName, textFName;

		numWords = stoi(argv[1]);
		dictFName = argv[2];
		textFName = argv[3];

		ifstream dictFile;
		dictFile.open(dictFName);
		cout << "Creating dictionary..." << endl;
		HashTable<string> dictionary = HashTable <string>(2 * numWords);
		if (dictFile)
		{
			cout << "Dictionary file found! (Inserting items, Please wait!)" << endl;
			string tempWord;
			dictFile >> tempWord;
			while (dictFile)
			{
				dictionary.insert(tempWord);
				dictFile >> tempWord;
			}
			// Reporting statistical data to cerr
			cerr << "Number of words " << dictionary.get_currSize()
				<< ", Total size : " << dictionary.get_totalSize()
				<< ", Load factor : " << dictionary.get_loadFactor() << endl
				<< "Collisions : " << dictionary.get_NumCollisions()
				<< ", Average chain length: " << dictionary.get_AvgCLen()
				<< ", Longest Chain length: " << dictionary.get_LonChainLen()
				<< endl;
		}
		else
		{
			cout << "Dictionary file NOT found!" << endl;
		}

		ifstream textFile;
		textFile.open(textFName);
		if (textFile)
		{
			int lineNum = 1;
			cout << "Spell mistake(s) in the text file:" << endl << endl;
			string currLine;
			getline(textFile, currLine);
			while (textFile)
			{
				for (int i = 0; i < currLine.length(); i++)
				{
					if (!isalpha(currLine[i]))
						currLine[i] = ' ';
				}

				stringstream ssin(currLine);
				while (ssin.good())
				{
					string theWord;
					ssin >> theWord;
					if (theWord != "")
					{
						if (!dictionary.contains(theWord))
						{
							string currWord = toLowerCase(theWord);
							if (!dictionary.contains(currWord))
							{
								cout << left << setw(10) << theWord
									<< "(" << setw(2) << lineNum << ") : ";

								//Inserted extra alphabet elimination spell check starts here
								for (int i = 0; i < theWord.length(); ++i)
								{
									string temp;
									temp = theWord.substr(0, i) + theWord.substr(i + 1, theWord.length());
									if (dictionary.contains(temp))
										cout << temp << ' ';
								}
								//Ends here

								//Forgot to insert alphabet spell check starts here
								for (int i = 0; i <= theWord.length(); ++i)
								{
									for (char ch = 'a'; ch <= 'z'; ch++)
									{
										string temp;
										temp = theWord.substr(0, i) + ch + theWord.substr(i, theWord.length());
										if (dictionary.contains(temp))
											cout << temp << ' ';
									}
								}
								//Ends here

								//Swapping immediate alphabets spell check starts here
								for (int i = 0; i < theWord.length() - 1; ++i)
								{
									string temp = theWord;
									swap(temp[i], temp[i + 1]);
									if (dictionary.contains(temp))
										cout << temp << ' ';
								}
								//Ends here

								//Trying all alphabets on each character spell check starts here
								for (int i = 0; i < theWord.length(); ++i)
								{
									string temp = theWord;
									for (char ch = 'a'; ch <= 'z'; ch++)
									{
										temp[i] = ch;
										if (dictionary.contains(temp))
											cout << temp << ' ';
									}
								}
								//Ends here

								cout << endl;
							}
						}
					}
				}
				++lineNum;
				getline(textFile, currLine);
			}

		}
		else
		{
			cout << "Text file NOT found!" << endl;
		}
		cout << "Press any key to continue..." << endl;
		getchar();
		return 0;
	}
	else
		return -1;
}

string toLowerCase(string word)
{
	int i = 0;
	char c = word[i];
	while (i < word.length())
	{
		if (c >= 65 && c <= 90)
		{
			c = c + 32;
			word[i] = c;
		}
		++i;
	}
	return word;
}