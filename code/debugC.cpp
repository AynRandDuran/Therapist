#include <curses.h>
#include "debugC.h"
#include "machine.h"

using namespace std;

debugC::debugC(machine* BFM){
	localMachine = BFM;
}

void debugC::setupDebugger(){
	initscr();
	raw();
	noecho();
	curs_set(0);

	printw("\t\t\tBr**nfuck Curses Debugger");

	stackWindow = subwin(stdscr, 18, 7, 1, 0);
	wborder(stackWindow, '|', '|', '-', '-', '|', '|', 'x', 'x');
	mvwprintw(stackWindow, 0, 1, "Stack");

	tapeWindow = subwin(stdscr, 5, 72, 1, 8);
	wborder(tapeWindow, '|', '|', '-', '-', 'x', 'x', 'x', 'x');
	mvwprintw(tapeWindow, 0, 2, "Tape");

	inputWindow = subwin(stdscr, 5, 20, 14, 8);
	wborder(inputWindow, '|', '|', '-', '-', 'x', 'x', 'x', 'x');
	mvwprintw(inputWindow, 0, 2, "Input");

	outputWindow = subwin(stdscr, 5, 20, 14, 29);
	wborder(outputWindow, '|', '|', '-', '-', 'x', 'x', 'x', 'x');
	mvwprintw(outputWindow, 0, 2, "Output");

	codeWindow = subwin(stdscr, 8, 72, 6, 8);
	wborder(codeWindow, '|', '|', '-', '-', 'x', 'x', 'x', 'x');
	mvwprintw(codeWindow, 0, 2, "Source code");
}

void debugC::tearDown(){
	endwin();
}

void debugC::drawStack(){
	int* localStack = localMachine->getStack();
	int top = localMachine->getTopOfStack();
	int cursY = 1;

	for(int stkL = 0; stkL < top; stkL++){
		//print stuff. Take care of this after actual stepping
	}

}

void debugC::drawTape(){

}

void debugC::drawCode(){

}

void debugC::showInitialMachineStates(){
	drawStack();
	drawTape();
	drawCode();

	refresh();
}

void debugC::updateScreen(){
	
}

void stepForward(string){

}

//begin debugger, create window with curses
void debugC::start(string source){
	setupDebugger();
	showInitialMachineStates();

	int input;
	while((input = getch()) != '!'){ //Exit debugger on !
		//updateScreen();
		if(input == KEY_RIGHT)
			stepForward(source);
	}

	tearDown();
}
