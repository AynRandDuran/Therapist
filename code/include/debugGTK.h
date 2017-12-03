#ifndef GTKMM_BF_DEBUG
#define GTKMM_BF_DEBUG

#include <gtkmm/button.h>
#include <gtkmm/window.h>
#include <gtkmm/grid.h>
#include <gtkmm/frame.h>
#include <gtkmm/label.h>
#include <gtkmm/textview.h>
#include <gtkmm/scrollable.h>
#include <gtkmm/scrolledwindow.h>
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
	Gtk::Label tapeCells[10];

	Gtk::Frame codeFrame;
	Gtk::Grid stackGrid;
	Gtk::Frame outputFrame;
	Gtk::Frame sourceFrame;
	Gtk::ScrolledWindow sourceWindow;
	Gtk::TextView sourceView;

	Gtk::Button step;
	Gtk::Button advance;
	Gtk::Button quit;
	Gtk::Button finish;

public:
	debugGTK();
	void drawControlFrame();
	void createTapeFrame();
	void drawTapeFrame();
	void createSourceViewer();

	void stepF();
	void advanceToHalt();
	void advanceToEnd();
	void endDebugger();

	virtual ~debugGTK();

};

#endif
