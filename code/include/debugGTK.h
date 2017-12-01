#ifndef GTKMM_BF_DEBUG
#define GTKMM_BF_DEBUG

#include <gtkmm/button.h>
#include <gtkmm/window.h>
#include <gtkmm/grid.h>
#include "machine.h"

class debugGTK : public Gtk::Window{
private:
	machine* BFM;

	void stepF();
	void endDebugger();

	Gtk::Grid masterGrid;
	Gtk::Button stepForward, end;

public:
	debugGTK();
	virtual ~debugGTK();

};

#endif
