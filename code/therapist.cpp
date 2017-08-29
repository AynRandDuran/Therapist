#include <string>
#include <iostream>
#include <cstdlib>
#include <fstream>

using namespace std;

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

int main(int argc, char **argv){

	//Load source code from a file
	string sourceArray = loadSource(argv[1]).c_str();

}
