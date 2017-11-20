#include <string>
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <unistd.h>
#include "../include/machine.h"
#include "../include/debugC.h"

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
	int tapeLength = 30000; //Default unless defined to be otherwise
	bool sCells = true; //Tape cells are signed by default
	bool AIO = false; //IO is not ascii by default;
	int debugMode = 0; //0: no debugger; 1: Curses debug mode; 2: GTK debug mode

	string sourceFile;

	//Recognize command line flags
	while((opCount = getopt(argc, argv, "asf:l:c")) != -1){
		switch(opCount){
			case 'a': //Toggle ASCII IO
				AIO = true;
				break;
			case 's': //Toggle unsigned cells
				sCells = false;
				break;
			case 'f': //Provide a BF source code file to interpret
				sourceFile = optarg;
				break;
			case 'l': //Define tape length
				tapeLength = atoi(optarg);
				break;
			case 'c':
				debugMode = 1;
				break;
			default:
				exit(1);
		}
	}	

	string sourceString = loadSource(sourceFile.c_str());

	//Create a new machine with given command line args
	machine* BFM = new machine(tapeLength, sCells, AIO, sourceString);

	switch(debugMode){
		case 0:
			BFM->processSource();	
			break;
		case 1:
			debugC *debugger = new debugC(BFM);
			debugger->start(sourceString);
			break;
	}

	delete BFM;
}
