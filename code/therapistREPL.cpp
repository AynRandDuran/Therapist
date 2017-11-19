#include <stdio.h>
#include <string>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "replEnvironment.h"

using namespace std;

replEnvironment* handleArguments(int argc, char* argv[]){
	bool AIO = false;
	bool signedCells = true;
	int tapeLength = 30000; int opCount;
	while((opCount = getopt(argc, argv, "asl:")) != -1){
		switch(opCount){
			case 'a': //Toggle ASCII IO
				AIO = true;
				break;
			case 's': //Toggle unsigned cells
				signedCells = false;
				break;
			case 'l': //Define tape length
				tapeLength = atoi(optarg);
				break;
			default:
				exit(1);
		}
	}
	return new replEnvironment(AIO, signedCells, tapeLength);	
}

int main(int argc, char* argv[]){
	//Create a new repl environment based on command line arguments
	replEnvironment* shell = handleArguments(argc, argv);

	char* input;
	while((input = readline("[bfsh:]"))){
		add_history(input);
		shell->process(input);
	}
	printf("\n");
}
