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
Create a machine, specifying tape size
and alphabet, building off that.
*/
machine::machine(int tSize, string AB){
	tapeSize = tSize;
	tape = new int[tapeSize];
	alphabet = AB;
	for(int i = 0; i < tapeSize; i++) tape[i] = 0;

	/*
	Make use of a stack for looping.
	I doubt a br**nfuck program will ever have
	loops nested to 1000, but eh
	*/
	stacc = new int[1000];
	topOfStacc = -1;
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
	tape[dataPointer]--;
	return tape[dataPointer];
}

/*
Increment the value in the cell
the data pointer is pointing at
*/
int machine::incCell(){
	tape[dataPointer]++;
	return tape[dataPointer];
}

/*
Output the value in the current cell
*/
void machine::output(){
	cout << tape[dataPointer];
}

/*
Take keyboard input and
deposit it in the current cell
*/
void machine::input(){
	cin >> tape[dataPointer];
}

/*
Upon encountering a left bracket,
begin a loop by pushing the location
of the bracket just encountered to a stack.
*/
void machine::leftBracket(int position){
	topOfStacc++;
	stacc[topOfStacc] = position;
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
				this->input();
				break;
			case '.':
				this->output();
				break;
			case '[':
				leftBracket(iter);
				break;
			case ']':
				if(tape[dataPointer] != 0){
					iter = rightBracket(toProcess);
					//iter = stacc[topOfStacc]-1;
					//topOfStacc--;
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
