#include <stdio.h>
#include <cstdlib>
#include <string>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "../include/replEnvironment.h"

//colors for fanciness
#define GRN "\x1B[38;5;10m"
#define NRM "\x1B[0m"

using namespace std;

//handle command flags
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

/*
Buffer input after a loop is opened to delay execution
This subprompt is only created when the last char of input is [,
and ended when the first char is ]. Only supports
*/
void beginBuffering(char* buffer){
	char* input;
	while((input = readline("\x1B[1;31;5;10m>>>\x1B[0m"))){ //red subprompt
		strcat(buffer, input);
		if(strchr(input, ']'))
			break;
	}
}

//build the path to a user's unique history file
char* buildHistoryPath(){
	char* historyPath = (char*)malloc(64);
	sprintf(historyPath, "%s/.bfsh_history", getenv("HOME"));
	return historyPath;
}

int main(int argc, char* argv[]){
	//Create a new repl environment based on command line arguments
	replEnvironment* shell = handleArguments(argc, argv);
	char* input; char* prompt; char* historyPath = buildHistoryPath();
	sprintf(prompt, "%s[BFSH:]%s ", GRN, NRM); //nice green prompt
	read_history(historyPath);

	while((input = readline(prompt))){
		if(input[strlen(input)-1] == '['){
			beginBuffering(input);
		}

		add_history(input);
		write_history(historyPath);
		shell->process(input);
	}
	printf("\n");
}
