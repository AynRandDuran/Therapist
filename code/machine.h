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
	int* tape; //Array itself
	string alphabet; //Ordered collection of acceptable input characters

	int* stacc;
	int topOfStacc;

public:
	/*Constructor. Recieve a given tape size and alphabet*/
	machine(int tSize);

	/*Functions corresponding to the 8 BF operations*/
	int incPointer(); //>, return new position
	int decPointer(); //<, return new position

	int incCell(); //+, return new cell value
	int decCell(); //-, return new cell value

	int output(); //return value in current cell
	int input(); //Change value of current cell to new arg

	int leftBracket(int position);
	int rightBracket(string toProcess);

	/*Recieve a string to process as Brainfuck code*/
	void process(string toProcess);

	~machine();
};

#endif
