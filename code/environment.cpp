/*
Environment class, intended to hold
variables and functions relevent to the
program being interpreted.
I'll get to this eventually I swear
*/
#include <string>
#include <cstdlib>
#include <iostream>
#include "environment.h"
using namespace std;

environment::environment(int bSize, string AB){
	bodySize = bSize;
	body = new int[bodySize];
	alphabet = AB;
	for(int i = 0; i < bodySize; i++) body[i] = 0;
}

int environment::incPointer(){
	dataPointer++;
	return dataPointer;
}

int environment::decPointer(){
	dataPointer--;
	return dataPointer;
}

int environment::decCell(){
	body[dataPointer]--;
	return body[dataPointer];
}

int environment::incCell(){
	body[dataPointer]++;
	return body[dataPointer];
}

void environment::output(){
	cout << body[dataPointer] << endl;
}

void environment::input(){
	cin >> body[dataPointer];
}

int findLeftBracket(string input){
	for(int i = 0; i < input.length(); i++)
		if(input[i] == '[') return i;
}

int findRightBracket(string input){
	for(int i = 0; i < input.length(); i++)
		if(input[i] == ']') return i;
}

void environment::process(string toProcess){
	int LB = findLeftBracket(toProcess);
	int RB = findRightBracket(toProcess);

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
				if(body[dataPointer] == 0)
					iter = RB-1;
				break;
			case ']':
				if(body[dataPointer] != 0)
					iter = LB;
				break;
		}
	}
}

environment::~environment(){
	delete body;
}
