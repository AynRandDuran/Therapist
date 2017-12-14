#include <stdio.h>
#include <string.h>
#include <unordered_map>
#include <sstream>
#include <readline/history.h>
#include "../include/replEnvironment.h"
#include "../include/machine.h"
using namespace std;

replEnvironment::replEnvironment(bool AIO, bool signedCells, int tapeLength){
	localMachine = new machine(tapeLength, signedCells, AIO, "");

	bindings.insert({"pushRightOne", "[->+<]"}); //example procedures
	bindings.insert({"pushLeftOne", "[-<+>]"});
	bindings.insert({"add", ",>, pushLeftOne <."});
}

machine* replEnvironment::getMachine(){
	return localMachine;
}

//recognize an attempt to bind a name
bool replEnvironment::tryingToBind(char* potentialBinding){
	return strchr(potentialBinding, '=');
}

//Add a new procedure to an unsorted map
bool replEnvironment::addNewProcedure(char* binding){
	char* key = strtok(binding, "=");
	char* value = strtok(NULL, "=");
		
	bindings[key] = value;

	//Confirm successful addition
	return bindings.count(key);
}

//recognize usage of a procedure and expand before being passed to the machine
char* replEnvironment::expandProcedure(char* statement){

	char* expandedHistory[256];
	history_expand(statement, expandedHistory);
	strcpy(statement, expandedHistory[0]);

	unordered_map<string, string>::const_iterator result = bindings.find(statement);
	if(result == bindings.end())
		return statement;
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

//process readline input before being passed to the machine
void replEnvironment::process(char* input){
	do
		tokenizeForExpansion(input);
	while(strchr(input, ' '));

	localMachine->addToSource(input);
	localMachine->processSource();
	if(strchr(input, '.'))
		printf("\n");
}
