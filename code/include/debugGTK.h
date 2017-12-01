#ifndef GTKMM_BF_DEBUG
#define GTKMM_BF_DEBUG

#include <gtkmm/button.h>
#include <gtkmm/window.h>
#include <gtkmm/grid.h>
#include <gtkmm/frame.h>
#include "machine.h"

class debugGTK : public Gtk::Window{
private:
	machine* BFM;

	Gtk::Grid masterGrid;
	Gtk::Frame controlFrame;
	Gtk::Grid controlGrid;
	Gtk::Grid tapeGrid;
	Gtk::Frame codeFrame;
	Gtk::Grid stackGrid;
	Gtk::Frame outputFrame;

	Gtk::Button step;
	Gtk::Button quit;
	Gtk::Button advance;

public:
	debugGTK();
	void drawControlFrame();

	void stepF();
	void advanceToEnd();
	void endDebugger();

	virtual ~debugGTK();

};

#endif
