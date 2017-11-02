#ifndef __BFdebug
#define __BFdebug

#include <curses.h>
#include "machine.h"

class debugC{
private:
	machine* localMachine;
public:
	debugC(machine* BFM);
	WINDOW* createDebugWindow(int height, int width, int startY, int startX);
	void setupDebugger();
	void tearDown();
	void start();
};

#endif
