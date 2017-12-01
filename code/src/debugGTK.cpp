#include "../include/debugGTK.h"
#include "machine.cpp"
#include <iostream>

debugGTK::debugGTK()
	:
	step("Step"),
	advance("Advance"),
	quit("Quit")
{
	BFM = new machine(30000, false, false, "+++.");

	add(masterGrid);
	masterGrid.set_border_width(5);
	set_title("Therapist Debugger");
	set_size_request(800, 500);


	masterGrid.attach(controlFrame, 1, 1, 1, 3);

	drawControlFrame();
	show_all_children();

	step.signal_clicked().connect(sigc::mem_fun(*this, &debugGTK::stepF));
	advance.signal_clicked().connect(sigc::mem_fun(*this, &debugGTK::advanceToEnd));
	quit.signal_clicked().connect(sigc::mem_fun(*this, &debugGTK::endDebugger));
}

void debugGTK::drawControlFrame(){

	controlFrame.set_border_width(10);
	controlFrame.set_shadow_type(Gtk::SHADOW_ETCHED_OUT);
	controlFrame.set_label("Controls");
	controlFrame.add(controlGrid);
	controlGrid.set_border_width(5);
	controlGrid.attach(step, 1, 1, 1, 1);
	controlGrid.attach(advance, 1, 2, 1, 1);
	controlGrid.attach(quit, 1, 3, 1, 1);
	step.set_border_width(5);
	advance.set_border_width(5);
	quit.set_border_width(5);
}

debugGTK::~debugGTK(){}

void debugGTK::stepF(){
	BFM->processChar(1);
}

void debugGTK::advanceToEnd(){
	BFM->processSource();
}

void debugGTK::endDebugger(){
	exit(0);
}
