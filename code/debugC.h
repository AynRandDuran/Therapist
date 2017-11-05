#ifndef __BFdebug
#define __BFdebug

#include <curses.h>
#include "machine.h"

class debugC{
private:
	machine* localMachine;

	WINDOW* stackWindow;
	WINDOW* codeWindow;
	WINDOW* tapeWindow;
	WINDOW* outputWindow;
	WINDOW* inputWindow;
public:
	debugC(machine* BFM);
	WINDOW* createDebugWindow(int height, int width, int startY, int startX);
	void setupDebugger();
	void tearDown();

	void drawStack();
	void drawTape();
	void drawCode();

	void showInitialMachineStates();
	void updateScreen();
	void start(string source);
};

#endif
