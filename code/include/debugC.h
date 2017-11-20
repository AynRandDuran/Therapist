#ifndef __BFdebug
#define __BFdebug

#include <curses.h>
#include "machine.h"

class debugC{
private:
	machine* localMachine;

	WINDOW* master;
	WINDOW* stackWindow;
	WINDOW* codeWindow;
	WINDOW* tapeWindow;
	WINDOW* outputWindow;
	WINDOW* inputWindow;
public:
	debugC(machine* BFM);
	~debugC();

	WINDOW* createDebugWindow(int height, int width, int startY, int startX);
	void setupDebugger();
	void tearDown();

	void redrawStackWindow(int stackHeight);
	void redrawTapeWindow();
	void redrawCodeWindow();
	void redrawOutputWindow();

	void specialActions(char op);

	void updateScreen();
	void step(int mod);
	void start(string source);
};

#endif
