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
#include <gtkmm/textiter.h>
#include <gtkmm/checkbutton.h>
#include <gtkmm/dialog.h>
#include <gtkmm/entry.h>
#include <iostream>
#include "machine.h"

class debugGTK : public Gtk::Window{
private:
	machine* BFM;

	int windowWidth, windowHeight;
	stringstream outputBuffer;

	Gtk::Grid masterGrid;
	Gtk::Frame controlFrame;
	Gtk::Grid controlGrid;

	Gtk::Frame tapeFrame;
	Gtk::Grid tapeGrid;
	Gtk::Label tapeCells[10];

	Gtk::Frame sourceFrame;
	Gtk::ScrolledWindow sourceWindow;
	Gtk::TextView sourceView;

	Gtk::Frame outputFrame;
	Gtk::ScrolledWindow outputWindow;
	Gtk::Label outputLabel;
	
	Gtk::Frame stackFrame;
	Gtk::ScrolledWindow stackWindow;
	Gtk::Grid stackGrid;
	Gtk::Label stackCells[5];

	Gtk::Dialog inputDialog;

	Gtk::CheckButton ASCIIToggle;
	Gtk::Button step;
	Gtk::Button advance;
	Gtk::Button quit;
	Gtk::Button finish;
	Gtk::Button modifySource;

	Glib::RefPtr<Gtk::TextBuffer::Tag> charHighlight;
	
public:
	debugGTK();
	
	void toggleASCIIMode();
	void createWindow();
	void drawWindowContents();
	void drawControlFrame();
	void createTapeFrame();
	void drawTapeFrame();
	void createSourceViewer();
	void createTagTable();
	void createOutputViewer();
	void createStackViewer();
	void updateStackViewer();
	void highlightNextChar();
	void startObserving();
	void outputCell(int cell);

	void stepF();
	void advanceToHalt();
	void advanceToEnd();
	void replaceSource();
	void endDebugger();

	virtual ~debugGTK();

};

#endif
