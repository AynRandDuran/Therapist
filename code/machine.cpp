/*
Machine class, intended to hold
variables and functions relevent to the
program being interpreted.
*/
#include <string>
#include <cstdlib>
#include <iostream>
#include "machine.h"
using namespace std;

/*
Create a machine
*/
machine::machine(int tSize, bool sCells, bool AIO){
	tapeSize = tSize;
	isSigned = sCells;
	asciiMode = AIO;

	if(sCells){
		tape = new int[tapeSize];
		for(int i = 0; i < tapeSize; i++) tape[i] = 0;
	}
	else{
		uTape = new unsigned int[tapeSize];
		for(int i = 0; i < tapeSize; i++) uTape[i] = 0;
	}

	stacc = new int[1000];
	topOfStacc = -1;

	/*
	Use function pointers for IO to allow for more generic references.
	The actual function used depends on the -a flag.
	I should rename these to something better sometime.
	*/
	this->output = (AIO)? &machine::AO : &machine::NAO;
	this->input = (AIO)? &machine::AI : &machine::NAI;
}

//Allow general modification of tape contents
int machine::modifyTape(int DP, int mod){
	if(isSigned){
		tape[DP] = tape[DP] + mod;
		return getTapeAt(DP);
	}
	
	uTape[DP] = tape[DP] + mod;
	return getTapeAt(DP);
}

int machine::getTapeAt(int DP){
	if(isSigned)
		return tape[DP];
	return uTape[DP];
}

/*
Increment the data pointer.
"Move right" on the array
*/
int machine::incPointer(){
	dataPointer++;
	return dataPointer;
}

/*
Decrement the data pointer.
"Move left" on the array
*/
int machine::decPointer(){
	dataPointer--;
	return dataPointer;
}

/*
Decrement the value in the cell
the data pointer is pointing at
*/
int machine::decCell(){
	return modifyTape(dataPointer, -1);
}

/*
Increment the value in the cell
the data pointer is pointing at
*/
int machine::incCell(){
	return modifyTape(dataPointer, 1);
}

//Non-ASCII output
int machine::NAO(){
	cout << getTapeAt(dataPointer);
	return getTapeAt(dataPointer);
}

//ASCII output
int machine::AO(){
	cout << (char)tape[dataPointer];
	return getTapeAt(dataPointer);
}

//Non-ASCII keyboard input
int machine::NAI(){
	int in; cin >> in;
	return modifyTape(dataPointer, in);
}

//ASCII input
//Take a single char from keyboard
int machine::AI(){
	char in; scanf("%c", &in);
	return modifyTape(dataPointer, in);
}

/*
Upon encountering a left bracket,
begin a loop by pushing the location
of the bracket just encountered to a stack.
Return the position of the left bracket.
*/
int machine::leftBracket(int position){
	topOfStacc++;
	stacc[topOfStacc] = position;
	return position;
}

/*
When a right bracket is found,
pop the stack of left bracket locations
and jump to the point given
*/
int machine::rightBracket(string toProcess){
		int i = stacc[topOfStacc]-1;
		topOfStacc--;
		return i;
}

/*Process a given string as Brainfuck source code*/
void machine::process(string toProcess){
	/*Loop through each char and switch to determine which BF operation to apply*/
	for(int iter = 0; iter < toProcess.length(); iter++){
		switch(toProcess[iter]){
			case '+':
				this->incCell();
				break;
			case '-':
				this->decCell();
				break;
			case '<':
				this->decPointer();
				break;
			case '>':
				this->incPointer();
				break;
			case ',':
				(this->*this->input)();
				break;
			case '.':
				(this->*this->output)();
				break;
			case '[':
				leftBracket(iter);
				break;
			case ']':
				if(getTapeAt(dataPointer) != 0){
					iter = rightBracket(toProcess);
				}
				else
					topOfStacc--;
				break;
		}
	}
}

//Delete the machine object
machine::~machine(){
	delete tape;
}
