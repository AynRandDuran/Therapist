#include <string>
#include <iostream>
#include <cstdlib>
#include <fstream>
#include "environment.h"
#include "LinkedList.h"

using namespace std;

/*
Define legal brainfuck operators.
I don't like having a global var but eh.
At some point I'd like to add a flag to allow input
of new charsets for the sake of making code look cooler
or something. E.g, replace +-<>,.[] with 12345678
*/
string legalOps = "+-<>,.[]";

/*
~~~NOTES TO SELF~~~
TC: I want tests for this function
*/


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
TC
Read source code from a file into a string
*/
string loadSource(char *fileName){

	//create source file stream and make sure it opened right
	ifstream source(fileName);
	ErrorCheck(!source.good(), "FUCK");

	//Return the entire contents of the file
	string sourceString; string throwAway;
	while(getline(source, throwAway))
		sourceString+=throwAway;
	source.close();
	return sourceString;
}

/*
TC
Strip all characters from a given string that aren't valid Brainfuck operators
*/
string stripInvalid(string toStrip){
	string validString;
	for(int iter = 0; iter < toStrip.length(); iter++){
		if(legalOps.find(toStrip[iter]) != string::npos) validString+=toStrip[iter];
	}
	return validString;
}

int findLeftBracket(string source){
	for(int i = 0; i < source.length(); i++){
		if(source[i] == '[') return i;
	}
}

int findRightBracket(string source){
	for(int i = 0; i < source.length(); i++){
		if(source[i] == ']') return i;
	}
}

int main(int argc, char **argv){

	//Load source code from a file and strip invalid characters
	string sourceString = stripInvalid(loadSource(argv[1]).c_str());
	cout << sourceString << endl;

	int* array = new int[30000];
	for(int i = 0; i < 30000; i++) array[i] = 0;
	int DP = 0;

	int leftBracket = findLeftBracket(sourceString);
	int rightBracket = findRightBracket(sourceString);

	//Outrageously basic interpreter, supports a single set of brackets to loop	
	for(int iter = 0; iter < sourceString.length(); iter++){
		switch(sourceString[iter]){
			case '+':
				array[DP]++;
				break;
			case '-':
				array[DP]--;
				break;
			case '<':
				DP--;
				break;
			case '>':
				DP++;
				break;
			case ',':
				cin >> array[DP];
				break;
			case '.':
				cout << array[DP] << endl;
				break;
			case '[':
				if(array[DP] == 0) iter = rightBracket-1;
				break;
			case ']':
				if(array[DP] != 0)
					iter = leftBracket;
				break;
		}
	}

}
