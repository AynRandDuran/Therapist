#ifndef GTKMM_BF_DEBUG
#define GTKMM_BF_DEBUG

#include <gtkmm/button.h>
#include <gtkmm/window.h>
#include <gtkmm/grid.h>
#include <gtkmm/frame.h>
#include <gtkmm/label.h>
#include "machine.h"

class debugGTK : public Gtk::Window{
private:
	machine* BFM;

	int windowWidth, windowHeight;

	Gtk::Grid masterGrid;
	Gtk::Frame controlFrame;
	Gtk::Grid controlGrid;

	Gtk::Frame tapeFrame;
	Gtk::Grid tapeGrid;
	Gtk::Label tapeLabels[10];

	Gtk::Frame codeFrame;
	Gtk::Grid stackGrid;
	Gtk::Frame outputFrame;

	Gtk::Button step;
	Gtk::Button quit;
	Gtk::Button finish;

public:
	debugGTK();
	void drawControlFrame();

	void createTapeFrame();
	void drawTapeFrame();

	void stepF();
	void advanceToEnd();
	void endDebugger();

	virtual ~debugGTK();

};

#endif
