#include <stdio.h>
#include <string.h>
#include <unordered_map>
#include "replEnvironment.h"
#include "machine.h"
using namespace std;

replEnvironment::replEnvironment(bool AIO, bool signedCells, int tapeLength){
	localMachine = new machine(tapeLength, signedCells, AIO, "");

	bindings.insert({"pushRightOne", "[->+<]"}); //example procedure
}

char* replEnvironment::expandProcedure(char* statements){
	unordered_map<string, string>::const_iterator result = bindings.find(statements);
	if(result == bindings.end())
		return statements;
	else
		return (char*)result->second.c_str();	
}

void replEnvironment::splitStatements(char* input){
	int count = 0;
	char* tmpStatement = strtok(input, ";");

	while(tmpStatement){
		tmpStatement = expandProcedure(tmpStatement);
		statements[count] = tmpStatement;
		tmpStatement = strtok(NULL, ";");
		count++;
	}
	statements[count] = NULL;
}

bool replEnvironment::process(char* input){
	splitStatements(input);
	int count = 0;
	while(statements[count]){
		localMachine->addToSource(statements[count]);
		localMachine->processSource();
		if(strchr(statements[count], '.'))
			printf("\n");
		count++;
	}
}
