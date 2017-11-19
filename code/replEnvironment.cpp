#include <stdio.h>
#include <string.h>
#include "replEnvironment.h"
#include "machine.h"
using namespace std;

replEnvironment::replEnvironment(bool AIO, bool signedCells, int tapeLength){
	localMachine = new machine(tapeLength, signedCells, AIO, "");
}

bool replEnvironment::process(char* input){
	localMachine->addToSource(input);
	localMachine->processSource();
	if(strchr(input, '.'))
		printf("\n");
}
