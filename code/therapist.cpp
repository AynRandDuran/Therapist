#include <string>
#include <iostream>
#include <cstdlib>
#include <fstream>
#include "machine.h"
#include "LinkedList.h"

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

/*
Find an -a flag in the argv parameter to
allow custom alphabets.
*/
string findAlphabet(char** input, int args){
	string alphabet = "+-<>,.[]";
	for(int i = 0; i < args; i++){
		if(input[i][0] == '-' && input[i][1] == 'a')
			alphabet = input[i+1];
	}
	return alphabet;
}

int main(int argc, char **argv){
	string sourceString = loadSource(argv[1]).c_str();
	cout << sourceString << endl;

	//Create a new machine with tape length 30,000 and standard alphabet
	machine* BFM = new machine(30000, findAlphabet(argv, argc));
	BFM->process(sourceString);

	delete BFM;
}
