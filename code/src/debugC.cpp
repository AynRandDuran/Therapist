#include <curses.h>
#include <stdio.h>
#include <string.h>
#include "../include/debugC.h"
#include "../include/machine.h"

using namespace std;

debugC::debugC(machine* BFM){
	localMachine = BFM;
}

debugC::~debugC(){
	delete this;
}

void debugC::setupDebugger(){
	initscr();
	raw();
	noecho();
	start_color();
	curs_set(0);

	init_pair(1, COLOR_WHITE, COLOR_GREEN);
	attron(A_BOLD);

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
	delwin(stackWindow);
	delwin(tapeWindow);
	delwin(inputWindow);
	delwin(outputWindow);
	delwin(codeWindow);
	delwin(stdscr);
	endwin();

	printf("\rFinished debugging\n");
	exit(0);
}

void debugC::redrawStackWindow(int stackHeight){
	//stack is only redrawn when it changes, don't waste time changing every iteration
	int top = localMachine->getStackTop();
	int* stack = localMachine->getStack();

	for(int y = 0; y < 16; y++){
		mvwprintw(stackWindow, 16-y, 1, "   ");
		if(y <= top){
			char* currentElement;
			sprintf(currentElement, "%i", stack[y]);
			mvwprintw(stackWindow, 16-y, 1, currentElement);
		}
	}

	wrefresh(stackWindow);
}

void debugC::redrawTapeWindow(){	
	char* tmp = (char*)malloc(20);
	int n = 1;
	for(int i = 0; i < 17; i++){
		sprintf(tmp, "%d", localMachine->getTapeAt(i));

		if(i == localMachine->getDataPointer())
			wattron(tapeWindow, COLOR_PAIR(1) | A_BOLD);

		mvwprintw(tapeWindow, 2, n, tmp);
		wprintw(tapeWindow, "|");
		wattroff(tapeWindow, COLOR_PAIR(1));
		n+=4;
	}
	wrefresh(tapeWindow);
}

void debugC::redrawCodeWindow(){
	string sourceToDraw = localMachine->getSource();

	wmove(codeWindow, 1, 1); int n = 0, tmpY, tmpX;
	while(n < localMachine->getSource().length()){
		if(n == localMachine->getSourceIterator())
			wattron(codeWindow, COLOR_PAIR(1) | A_BOLD);
		waddch(codeWindow, sourceToDraw[n]);
		wattroff(codeWindow, COLOR_PAIR(1));

		//When moving to a new line, start at x=1 instead of 0
		getyx(codeWindow, tmpY, tmpX);
		if(tmpX == 0)
			wmove(codeWindow, tmpY, 1);
		n++;
	}

	wborder(codeWindow, '|', '|', '-', '-', 'x', 'x', 'x', 'x');
	mvwprintw(codeWindow, 0, 2, "Source code");
	wrefresh(codeWindow);
}

void debugC::redrawOutputWindow(){
	FILE* abyss = fopen("/dev/null", "w");
	int returnedOutput = localMachine->NAO(abyss);
	char* output; sprintf(output, "%i", returnedOutput);
	mvwprintw(outputWindow, 1, 1, "   ");
	mvwprintw(outputWindow, 1, 1, output);
	wrefresh(outputWindow);
	fclose(abyss);
}

void debugC::updateScreen(){
	redrawCodeWindow();
	redrawTapeWindow();

	refresh();
}

void debugC::step(int mod){
	//have the machine return the operator that was just processed
	char retOperator = localMachine->processChar(mod);
	
	if(retOperator == '.')
			redrawOutputWindow();
}

void debugC::specialActions(char op){
	switch(op){
		case ',':
			endwin();
			printf("\rbfsh>");
			break;
		case '$': //absolute end of source code
			tearDown();
			break;
	}
}

//begin debugger, create window with curses
void debugC::start(string source){
	setupDebugger();
	updateScreen();

	int stackHeight = -1;
	int input;
	while((input = getch()) != '!'){ //Exit debugger on !

		char nextOp = source[localMachine->getSourceIterator()];
		specialActions(nextOp);

		//need to start working on over/underflow protection/wrapping now that I think about it
		if(input == '>')
			step(1);
		if(input == '<')
			step(-1); //for the love of god this probably won't work for a while

		//Only update the stack window if it actually changes
		if(stackHeight != localMachine->getStackTop()){
			stackHeight = localMachine->getStackTop();
			redrawStackWindow(stackHeight);
		}
		updateScreen();
	}

	tearDown();
};
