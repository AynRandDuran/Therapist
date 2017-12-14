#include <string>
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <unistd.h>
#include "../include/machine.h"
#include "../include/debugC.h"

using namespace std;

void ErrorCheck(bool condition, const char * message){
	if(condition){
		perror(message);
		exit(1);
	}
}


//Read source code from a file into a string
string loadSource(const char *fileName){

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
	//Process command line args
	int opCount;

	//Machine defaults unless specified otherwise
	int tapeLength = 30000;
	bool sCells = true;
	bool AIO = false;
	bool debug = false;

	string sourceFile;

	//Recognize command line flags
	while((opCount = getopt(argc, argv, "asf:l:c")) != -1){
		switch(opCount){
			case 'a':
				AIO = true;
				break;
			case 's':
				sCells = false;
				break;
			case 'f': //Provide a BF source code file to interpret
				sourceFile = optarg;
				break;
			case 'l':
				tapeLength = atoi(optarg);
				break;
			case 'c':
				debug = true;
				break;
			default:
				exit(1);
		}
	}	

	string sourceString = loadSource(sourceFile.c_str());

	//Create a new machine with given command line args
	machine* BFM = new machine(tapeLength, sCells, AIO, sourceString);

	if(debug){
		debugC *debugger = new debugC(BFM);
		debugger->start(sourceString);
	}else
		BFM->processSource();

	delete BFM;
}
