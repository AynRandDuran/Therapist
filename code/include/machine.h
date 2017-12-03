#ifndef __env__
#define __env__
#include <string>
#include <cstdlib>
#include <iostream>
#include <sigc++/sigc++.h>
using namespace std;

class machine{
private:
	int tapeSize; //Size of program's array
	int dataPointer = 0; //Position in array
	bool isSigned;
	bool asciiMode;

	int* tape;
	string alphabet; //Ordered collection of acceptable input characters

	int* stacc;
	int topOfStacc;

	string sourceBF = "";
	int universalIterator = 0; //Track progress through code from any function. Make getter/setter some day

	long int MINWRAP, MAXWRAP, RANGE;

	typedef sigc::signal<void> type_tape_change_signal;
	type_tape_change_signal m_tape_change_signal;

public:
	/*Constructor. Recieve a given tape size and alphabet*/
	machine(int tSize, bool sCells, bool AIO, string source);

	type_tape_change_signal notify_tape_change();

	int getTapeLength();
	int modifyTape(int DP, int mod);
	int getTapeAt(int DP);

	int modifyDataPointer(int mod);
	int getDataPointer();

	string getSource();
	int getSourceIterator();
	void addToSource(char* moreSource);
	void replaceSource(string newSource);

	int getStackTop();
	int* getStack();

	/*Functions corresponding to the 8 BF operations*/
	int incPointer(); //>, return new position
	int decPointer(); //<, return new position

	int incCell(); //+, return new cell value
	int decCell(); //-, return new cell value

	int NAO(FILE* file); //non-ascii output
	int AO(FILE* file); //ascii output

	int NAI(); //non-ascii input
	int AI(); //ascii input

	int leftBracket(int position);
	int rightBracket(int position);

	/*Recieve a string to process as Brainfuck code*/
	int processChar(int iterMod);
	void processSource();

	/*
	Define IO function pointers
	These allow us to easily switch which IO functions
	are used based on the -a flag
	*/
	int (machine::*output)(FILE* file);
	int (machine::*input)();

	~machine();
};

#endif
