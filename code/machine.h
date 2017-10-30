#ifndef __env__
#define __env__
#include <string>
#include <cstdlib>
#include <iostream>
using namespace std;

class machine{
private:
	int tapeSize; //Size of program's array
	int dataPointer = 0; //Position in array
	bool isSigned;
	bool asciiMode;

	int* tape; //Signed array
	unsigned int* uTape; //Unsigned array

	string alphabet; //Ordered collection of acceptable input characters

	int* stacc;
	int topOfStacc;

public:
	/*Constructor. Recieve a given tape size and alphabet*/
	machine(int tSize, bool sCells, bool AIO);

	int modifyTape(int DP, int mod);
	int getTapeAt(int DP);
	int modifyDataPointer(int mod);
	int getDataPointer();

	/*Functions corresponding to the 8 BF operations*/
	int incPointer(); //>, return new position
	int decPointer(); //<, return new position

	int incCell(); //+, return new cell value
	int decCell(); //-, return new cell value

	int NAO(); //non-ascii output
	int AO(); //ascii output

	int NAI(); //non-ascii input
	int AI(); //ascii input

	int leftBracket(int position);
	int rightBracket(string toProcess);

	/*Recieve a string to process as Brainfuck code*/
	void process(string toProcess);

	/*
	Define IO function pointers
	These allow us to easily switch which IO functions
	are used based on the -a flag
	*/
	int (machine::*output)();
	int (machine::*input)();

	~machine();
};

#endif
