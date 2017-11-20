#include <stdio.h>
#include <string.h>
#include <unordered_map>
#include <sstream>
#include "../include/replEnvironment.h"
#include "../include/machine.h"
using namespace std;

replEnvironment::replEnvironment(bool AIO, bool signedCells, int tapeLength){
	localMachine = new machine(tapeLength, signedCells, AIO, "");

	bindings.insert({"pushRightOne", "[->+<]"}); //example procedures
	bindings.insert({"pushLeftOne", "[-<+>]"});
	bindings.insert({"add", ",>, pushLeftOne .<."}); //nested procedure calls!
	
}

bool replEnvironment::tryingToBind(char* potentialBinding){
	return strchr(potentialBinding, '=');
}

void replEnvironment::addNewProcedure(char* binding){ //Turns out operators can be redefined. lel.
	char* key = strtok(binding, "=");
	char* value = strtok(NULL, "=");
		
	bindings[key] = value;
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
		if(tryingToBind(tmpStatement))
			addNewProcedure(tmpStatement);
		else	
			line << expandProcedure(tmpStatement);
		tmpStatement = strtok(NULL, " ");
	}
	
	strcpy(input, (char*)line.str().c_str());
}

void replEnvironment::process(char* input){
	do
		tokenizeForExpansion(input);
	while(strchr(input, ' '));

	localMachine->addToSource(input);
	localMachine->processSource();
	if(strchr(input, '.'))
		printf("\n");
}
