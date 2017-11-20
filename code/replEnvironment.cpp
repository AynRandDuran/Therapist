#include <stdio.h>
#include <string.h>
#include <unordered_map>
#include <sstream>
#include "replEnvironment.h"
#include "machine.h"
using namespace std;

replEnvironment::replEnvironment(bool AIO, bool signedCells, int tapeLength){
	localMachine = new machine(tapeLength, signedCells, AIO, "");

	bindings.insert({"pushRightOne", "[->+<]"}); //example procedures
	bindings.insert({"pushLeftOne", "[-<+>]"});
	bindings.insert({"add", ",>, [-<+>] <."});
}

char* replEnvironment::expandProcedure(char* statements){
	unordered_map<string, string>::const_iterator result = bindings.find(statements);
	if(result == bindings.end())
		return statements;
	else
		return (char*)result->second.c_str();	
}

void replEnvironment::tokenizeForExpansion(char* input){
	char* tmpStatement = strtok(input, " ");
	stringstream line;

	while(tmpStatement){
		line << expandProcedure(tmpStatement);
		tmpStatement = strtok(NULL, " ");
	}
	
	strcpy(input, (char*)line.str().c_str());
}

bool replEnvironment::process(char* input){
	tokenizeForExpansion(input);
	cout << input << endl;
	localMachine->addToSource(input);
	localMachine->processSource();
	if(strchr(input, '.'))
		printf("\n");
}
