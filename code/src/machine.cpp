/*
Machine class, intended to hold
variables and functions relevent to the
program being interpreted.
*/
#include <string.h>
#include <cstdlib>
#include <iostream>
#include <climits>
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

	MAXWRAP = INT_MAX;
	if(isSigned){
		MINWRAP = INT_MIN;
	}else{
		MINWRAP = 0;
	}
	RANGE = MAXWRAP+(abs(MINWRAP))+1;

	tape = new int[tapeSize]; 
	for(int i = 0; i < tapeSize; i++) tape[i] = 0;

	stacc = new int[1000];
	topOfStacc = -1;

	this->output = (AIO)? &machine::AO : &machine::NAO;
	this->input = (AIO)? &machine::AI : &machine::NAI;
}

int machine::getTapeLength(){ //Pretty much only for testing
	return tapeSize;
}

//Allow general modification of tape contents
int machine::modifyTape(int DP, int newContents){
	if(newContents > MAXWRAP)
		newContents-=RANGE;
	if(newContents < MINWRAP)
		newContents+=RANGE;

	tape[DP] = newContents;

	m_tape_change_signal.emit();
	return getTapeAt(DP);
}

int machine::getTapeAt(int DP){
	return tape[DP];
}

int machine::modifyDataPointer(int newDP){
	if(newDP > getTapeLength()-1)
		newDP = newDP - getTapeLength();
	if(newDP < 0)
		newDP = getTapeLength() + newDP;

	m_tape_change_signal.emit();
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
	modifyDataPointer(getDataPointer()+1);
	return dataPointer;
}

/*
Decrement the data pointer.
"Move left" on the array
*/
int machine::decPointer(){
	modifyDataPointer(getDataPointer()-1);
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
	fprintf(file, "%i", getTapeAt(dataPointer));
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

int machine::leftBracket(int position){
	if(getTapeAt(getDataPointer())){
		topOfStacc++;
		stacc[topOfStacc] = position;
		return position;
	}
	int balanced = -1;
	while(balanced != 0){
		position++;
		if(getSource()[position] == '[')
			balanced--;
		if(getSource()[position] == ']')
			balanced++;
	}
	return position++;
}

int machine::rightBracket(int position){
		int tmp = stacc[topOfStacc];
		topOfStacc--;
		if(getTapeAt(getDataPointer())){
			return tmp-1;
		}
		return position;
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
			universalIterator = leftBracket(universalIterator);
			break;
		case ']':
			universalIterator = rightBracket(universalIterator);
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

machine::type_tape_change_signal machine::notify_tape_change(){
	return m_tape_change_signal;
}
//Delete the machine object
machine::~machine(){
	delete tape;
}
