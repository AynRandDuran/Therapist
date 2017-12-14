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


machine::machine(int tSize, bool sCells, bool AIO, string source){
	sourceBF += source;
	tapeSize = tSize;
	isSigned = sCells;
	asciiMode = AIO;

	//define cell min/max values based on sCells value
	MAXWRAP = INT_MAX;
	if(isSigned){
		MINWRAP = INT_MIN;
	}else{
		MINWRAP = 0;
	}
	RANGE = MAXWRAP+(abs(MINWRAP))+1;

	tape = new int[tapeSize];	
	for(int i = 0; i < tapeSize; i++) tape[i] = 0;

	//create the loop stack
	stacc = new int[1000];
	topOfStacc = -1;

	//define functions to be used for IO
	this->output = (AIO)? &machine::AO : &machine::NAO;
	this->input = (AIO)? &machine::AI : &machine::NAI;
}

int machine::getTapeLength(){
	return tapeSize;
}

void machine::toggleASCIIMode(){
	asciiMode = !asciiMode;
	this->output = (asciiMode)? &machine::AO : &machine::NAO;
	this->input = (asciiMode)? &machine::AI : &machine::NAI;
}

bool machine::getASCIIMode(){
	return asciiMode;
}

/*
general modification of a tape cell,
enable wrapping
*/
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

/*
change the data pointer and
handle wrapping on the tape
*/
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
	return instructionPointer;
}

void machine::addToSource(char* moreSource){
	sourceBF.append(moreSource);
}

/*
replace the source tape and reset the instruction pointer
if it's shorter than the original
*/
void machine::replaceSource(string newSource){
	if(getSourceIterator() > newSource.length())
		instructionPointer = 0;
	sourceBF = newSource;
}

//somehow to lazy to implement push and pop
int machine::getStackTop(){
	return topOfStacc;
}

int* machine::getStack(){
	return stacc;
}

int machine::incPointer(){
	modifyDataPointer(getDataPointer()+1);
	return dataPointer;
}

int machine::decPointer(){
	modifyDataPointer(getDataPointer()-1);
	return dataPointer;
}

int machine::decCell(){
	return modifyTape(dataPointer, getTapeAt(dataPointer)-1);
}

int machine::incCell(){
	return modifyTape(dataPointer, getTapeAt(dataPointer)+1);
}

int machine::NAO(FILE* file){
	fprintf(file, "%i", getTapeAt(dataPointer));
	return getTapeAt(dataPointer);
}

int machine::AO(FILE* file){
	fprintf(file, "%c", getTapeAt(dataPointer));
	return getTapeAt(dataPointer);
}

int machine::NAI(){
	int in; cin >> in;
	return modifyTape(dataPointer, in);
}

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
	switch(sourceBF[instructionPointer]){
		case '+':														//increment the current cell contents
			this->incCell();
			break;
		case '-':														//decrement the current cell contents
			this->decCell();
			break;
		case '<':														//decrement the data pointer
			this->decPointer();
			break;
		case '>':														//increment the data pointer
			this->incPointer();
			break;
		case ',':														//get a byte of input from the keyboard
			signal_input_request.emit();
			(this->*this->input)();
			break;
		case '.':														//put the contents of the current cell on a file stream
			(this->*this->output)(stdout);
			send_cell_output.emit(getTapeAt(getDataPointer()));
			break;
		case '[':														//push instruction pointer to the stack if non-zero, otherwise jump
			instructionPointer = leftBracket(instructionPointer);
			signal_redraw_stack.emit();
			break;
		case ']':														//pop instruction pointer from stack if non-zero, and jump
			instructionPointer = rightBracket(instructionPointer);
			signal_redraw_stack.emit();
			break;
		}
	instructionPointer+=iterMod;
	return sourceBF[instructionPointer-iterMod];
}

/*Process a full string of BF source*/
void machine::processSource(){
	while(instructionPointer < sourceBF.length()){
		processChar(1);
	}

}

//define signal types for the GTK debugger
machine::type_tape_change_signal machine::notify_tape_change(){
	return m_tape_change_signal;
}

machine::output_cell_signal machine::get_cell_for_output(){
	return send_cell_output;
}

machine::stack_change_signal machine::notify_stack_change(){
	return signal_redraw_stack;
}
machine::input_request machine::request_some_input(){
	return signal_input_request;
}

//Delete the machine object
machine::~machine(){
	delete tape;
}
