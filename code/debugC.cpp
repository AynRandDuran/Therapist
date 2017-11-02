#include <curses.h>
#include "debugC.h"
#include "machine.h"

using namespace std;

debugC::debugC(machine* BFM){
	localMachine = BFM;
}

WINDOW* debugC::createDebugWindow(int height, int width, int startY, int startX){
	WINDOW* debugWindow = newwin(height, width, startY, startX);

	wborder(debugWindow, '|', '|', '-', '-', 'X', 'X', 'X', 'X');
	
	wrefresh(debugWindow);
	return debugWindow;
}

void debugC::setupDebugger(){
	initscr();
	raw();
	noecho();
	createDebugWindow(24, 80, 0, 0);
}

void debugC::tearDown(){
	endwin();
}

//begin debugger, create window with curses
void debugC::start(){
	setupDebugger();
	getch(); //Draw a window, destroy it upon first key press
	tearDown();
}
