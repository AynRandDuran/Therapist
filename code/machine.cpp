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
	cout << tape[dataPointer] << endl;
}

/*
Take keyboard input and
deposit it in the current cell
*/
void machine::input(){
	cin >> tape[dataPointer];
}

/*
Find the position of a [ for looping.
Currently only supports a single, non-nested loop
*/
int machine::findLeftBracket(string input){
	for(int i = 0; i < input.length(); i++)
		if(input[i] == '[') return i;
}

/*
Find the position of a ] for looping.
*/
int machine::findRightBracket(string input){
	for(int i = 0; i < input.length(); i++)
		if(input[i] == ']') return i;
}

/*Process a given string as Brainfuck source code*/
void machine::process(string toProcess){
	/*
	  Find left and right bracket locations for looping.
	  Eventually I'll have to abandon this setup to support nested loops.
	*/
	int LB = this->findLeftBracket(toProcess);
	int RB = this->findRightBracket(toProcess);

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
				if(tape[dataPointer] == 0)
					iter = RB-1;
				break;
			case ']':
				if(tape[dataPointer] != 0)
					iter = LB;
				break;
		}
	}
}

//Delete the machine object
machine::~machine(){
	delete tape;
}
