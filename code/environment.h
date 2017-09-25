#ifndef __env__
#define __env__
#include <string>
#include <cstdlib>
#include <iostream>
using namespace std;

class environment{
private:
	//Size of array in which our program exists
	int bodySize;
	int dataPointer = 0;
	int* body;
	string alphabet;

public:
	environment(int bSize, string AB);

	/*Functions corresponding to the 8 BF operations*/
	int incPointer(); //>, return new position
	int decPointer(); //<, return new position

	int incCell(); //+, return new cell value
	int decCell(); //-, return new cell value

	void output(); //return value in current cell
	void input(); //Change value of current cell to new arg

	void process(string toProcess);

	~environment();
};

#endif
