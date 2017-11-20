#include <stdio.h>
#include <string>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "replEnvironment.h"

//colors for fanciness
#define GRN "\x1B[38;5;10m"
#define NRM "\x1B[0m"

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

//Buffer input after a loop is opened to delay execution
void beginBuffering(char* buffer){
	
	char* input;
	while((input = readline("\x1B[1;31;5;10m>>>\x1B[0m"))){
		strcat(buffer, input);
		if(strchr(input, ']'))
			break;
	}
}

int main(int argc, char* argv[]){
	//Create a new repl environment based on command line arguments
	replEnvironment* shell = handleArguments(argc, argv);
	char* input; char* prompt;
	sprintf(prompt, "%s[BFSH:]%s", GRN, NRM);

	while((input = readline(prompt))){
		if(input[strlen(input)-1] == '['){
			beginBuffering(input);
		}
		add_history(input);
		shell->process(input);
	}
	printf("\n");
}
