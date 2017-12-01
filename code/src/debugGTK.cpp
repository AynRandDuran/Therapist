#include "../include/debugGTK.h"
#include "machine.cpp"
#include <iostream>

debugGTK::debugGTK()
	:
	step("Step"),
	finish("Finish"),
	quit("Quit")
{
	BFM = new machine(30000, false, false, "+++.>++++");
	add(masterGrid);
	masterGrid.set_border_width(5);
	set_title("Therapist Debugger");
	set_size_request(800, 500);

	get_size(windowWidth, windowHeight);

	masterGrid.attach(controlFrame, 1, 1, 1, 3);
	masterGrid.attach(tapeFrame, 2, 1, windowWidth-1, 1);

	drawControlFrame();
	createTapeFrame();
	show_all_children();

	step.signal_clicked().connect(sigc::mem_fun(*this, &debugGTK::stepF));
	finish.signal_clicked().connect(sigc::mem_fun(*this, &debugGTK::advanceToEnd));
	quit.signal_clicked().connect(sigc::mem_fun(*this, &debugGTK::endDebugger));
}

void debugGTK::createTapeFrame(){
	tapeFrame.set_border_width(10);
	tapeFrame.set_shadow_type(Gtk::SHADOW_ETCHED_OUT);
	tapeFrame.set_label("Tape");
	tapeFrame.add(tapeGrid);

	for(int i = 0; i < 10; i++){
		tapeLabels[i].set_margin_start(10);
		tapeLabels[i].set_margin_end(10);
		tapeGrid.attach(tapeLabels[i], i+1, 1, 1, 1);
	}

	drawTapeFrame();
}

void debugGTK::drawTapeFrame(){
	for(int i = 0; i < 10; i++){
		tapeLabels[i].set_text(std::to_string(BFM->getTapeAt(i)));
	}
}

void debugGTK::drawControlFrame(){

	controlFrame.set_border_width(10);
	controlFrame.set_shadow_type(Gtk::SHADOW_ETCHED_OUT);
	controlFrame.set_label("Controls");
	controlFrame.add(controlGrid);

	controlGrid.set_border_width(5);
	controlGrid.attach(step, 1, 1, 1, 1);
	controlGrid.attach(finish, 1, 2, 1, 1);
	controlGrid.attach(quit, 1, 3, 1, 1);

	step.set_border_width(5);
	finish.set_border_width(5);
	quit.set_border_width(5);
}

debugGTK::~debugGTK(){}

void debugGTK::stepF(){
	BFM->processChar(1);
	drawTapeFrame();
}

void debugGTK::advanceToEnd(){
	BFM->processSource();
	drawTapeFrame();
}

void debugGTK::endDebugger(){
	exit(0);
}
