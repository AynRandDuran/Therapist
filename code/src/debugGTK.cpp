#include "../include/debugGTK.h"
#include "machine.cpp"
#include <iostream>

debugGTK::debugGTK()
	:
	stepForward("step"),
	end("quit")
{
	BFM = new machine(30000, false, true, "++++++++++[->+++++++>++++++++++<<]>++.>+++++.>");

	add(masterGrid);
	masterGrid.set_border_width(5);
	set_title("Therapist Debugger");
	set_size_request(700, 500);

	stepForward.signal_clicked().connect(sigc::mem_fun(*this, &debugGTK::stepF));
	end.signal_clicked().connect(sigc::mem_fun(*this, &debugGTK::endDebugger));

	show_all_children();
}

debugGTK::~debugGTK(){}

void debugGTK::stepF(){
}

void debugGTK::endDebugger(){
	exit(0);
}
