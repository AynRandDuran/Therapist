/*
Machine class, intended to hold
variables and functions relevent to the
program being interpreted.
*/
#include <string.h>
#include <cstdlib>
#include <iostream>
#include "../include/machine.h"
using namespace std;

/*
Create a machine
*/
machine::machine(int tSize, bool sCells, bool AIO, string source){
	sourceBF += source;
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
int machine::modifyTape(int DP, int newContents){
	if(isSigned){
		tape[DP] = newContents;
		return getTapeAt(DP);
	}
	
	uTape[DP] = newContents;
	return getTapeAt(DP);
}

int machine::getTapeAt(int DP){
	if(isSigned)
		return tape[DP];
	return uTape[DP];
}

int machine::modifyDataPointer(int newDP){
	dataPointer = newDP;
	return dataPointer;
}

int machine::getDataPointer(){
	return dataPointer;
}

string machine::getSource(){
	return sourceBF;
}

int machine::getSourceIterator(){
	return universalIterator;
}

void machine::addToSource(char* moreSource){
	sourceBF.append(moreSource);
}

int machine::getStackTop(){
	return topOfStacc;
}

int* machine::getStack(){
	return stacc;
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
	return modifyTape(dataPointer, getTapeAt(dataPointer)-1);
}

/*
Increment the value in the cell
the data pointer is pointing at
*/
int machine::incCell(){
	return modifyTape(dataPointer, getTapeAt(dataPointer)+1);
}

//Non-ASCII output
int machine::NAO(FILE* file){
	fprintf(file, "%s", getTapeAt(dataPointer));
	return getTapeAt(dataPointer);
}

//ASCII output
int machine::AO(FILE* file){
	fprintf(file, "%c", getTapeAt(dataPointer));
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
int machine::rightBracket(){
		int i = stacc[topOfStacc]-1;
		topOfStacc--;
		return i;
}

//Process a single char
int machine::processChar(int iterMod){
	switch(sourceBF[universalIterator]){
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
			(this->*this->output)(stdout);
			break;
		case '[':
			leftBracket(universalIterator);
			break;
		case ']':
			if(getTapeAt(dataPointer) != 0){
				universalIterator = rightBracket();
			}
			else
				topOfStacc--;
			break;
		}
	universalIterator+=iterMod;
	return sourceBF[universalIterator-iterMod];
}

/*Process a full string of BF source*/
void machine::processSource(){
	while(universalIterator < sourceBF.length()){
		processChar(1);
	}

}

//Delete the machine object
machine::~machine(){
	delete tape;
}
