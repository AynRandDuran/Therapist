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

//Create a machine
machine::machine(int tSize, string AB){
	tapeSize = tSize;
	tape = new int[tapeSize];
	alphabet = AB;
	for(int i = 0; i < tapeSize; i++) tape[i] = 0;
}

int machine::incPointer(){
	dataPointer++;
	return dataPointer;
}

int machine::decPointer(){
	dataPointer--;
	return dataPointer;
}

int machine::decCell(){
	tape[dataPointer]--;
	return tape[dataPointer];
}

int machine::incCell(){
	tape[dataPointer]++;
	return tape[dataPointer];
}

void machine::output(){
	cout << tape[dataPointer] << endl;
}

void machine::input(){
	cin >> tape[dataPointer];
}

int machine::findLeftBracket(string input){
	for(int i = 0; i < input.length(); i++)
		if(input[i] == '[') return i;
}

int machine::findRightBracket(string input){
	for(int i = 0; i < input.length(); i++)
		if(input[i] == ']') return i;
}

void machine::process(string toProcess){
	int LB = this->findLeftBracket(toProcess);
	int RB = this->findRightBracket(toProcess);

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

machine::~machine(){
	delete tape;
}
