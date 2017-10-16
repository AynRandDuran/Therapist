#include <string>
#include <iostream>
#include <cstdlib>
#include <fstream>
#include "machine.h"

using namespace std;

/*
General error checker
*/
void ErrorCheck(bool condition, const char * message){
	if(condition){
		perror(message);
		exit(1);
	}
}

/*
Read source code from a file into a string
*/
string loadSource(char *fileName){

	//create source file stream and make sure it opened right
	ifstream source(fileName);
	ErrorCheck(!source.good(), "Read error");

	//Return the entire contents of the file
	string sourceString; string throwAway;
	while(getline(source, throwAway))
		sourceString+=throwAway+="\n";
	source.close();
	return sourceString;
}

int main(int argc, char **argv){
	string sourceString = loadSource(argv[1]).c_str();

	
	//Create a new machine with tape length 30,000 and standard alphabet
	machine* BFM = new machine(30000);
	BFM->process(sourceString);

	delete BFM;
}
