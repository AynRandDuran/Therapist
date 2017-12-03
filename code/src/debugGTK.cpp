#include "../include/debugGTK.h"
#include "machine.cpp"
#include <iostream>

debugGTK::debugGTK()
	:
	step("Step"),
	finish("Finish"),
	quit("Quit"),
	advance("Advance"),
	modifySource("Modify")
{
	BFM = new machine(30000, false, true, "A new Brainf* project");
	highlightNextChar();

	add(masterGrid);
	masterGrid.set_border_width(5);
	set_title("Therapist Debugger");
	set_size_request(600, 400);

	get_size(windowWidth, windowHeight);

	masterGrid.attach(controlFrame, 1, 1, 1, 5);
	masterGrid.attach(tapeFrame, 2, 1, windowWidth-1, 1);
	masterGrid.attach(sourceFrame, 2, 2, windowWidth-1, windowHeight-1);

	drawControlFrame();
	createTapeFrame();
	createTagTable();
	createSourceViewer();
	highlightNextChar();
	show_all_children();
}

void debugGTK::coupleForObservation(){
	BFM->notify_tape_change().connect(sigc::mem_fun(*this, &debugGTK::drawTapeFrame));
	step.signal_clicked().connect(sigc::mem_fun(*this, &debugGTK::stepF));
	step.signal_clicked().connect(sigc::mem_fun(*this, &debugGTK::highlightNextChar));
	advance.signal_clicked().connect(sigc::mem_fun(*this, &debugGTK::advanceToHalt));
	advance.signal_clicked().connect(sigc::mem_fun(*this, &debugGTK::highlightNextChar));
	finish.signal_clicked().connect(sigc::mem_fun(*this, &debugGTK::advanceToEnd));
	finish.signal_clicked().connect(sigc::mem_fun(*this, &debugGTK::highlightNextChar));
	modifySource.signal_clicked().connect(sigc::mem_fun(*this, &debugGTK::replaceSource));
	modifySource.signal_clicked().connect(sigc::mem_fun(*this, &debugGTK::highlightNextChar));
	quit.signal_clicked().connect(sigc::mem_fun(*this, &debugGTK::endDebugger));
}

void debugGTK::createTapeFrame(){
	tapeFrame.set_border_width(10);
	tapeFrame.set_shadow_type(Gtk::SHADOW_ETCHED_IN);
	tapeFrame.set_label("Tape");
	tapeFrame.add(tapeGrid);

	for(int i = 0; i < 10; i++){
		tapeCells[i].set_margin_start(10);
		tapeCells[i].set_margin_end(10);
		tapeCells[i].set_margin_top(5);
		tapeCells[i].set_margin_bottom(5);
		tapeCells[i].property_use_markup();
		tapeGrid.attach(tapeCells[i], i+1, 1, 1, 1);
	}
	
	drawTapeFrame();
}

void debugGTK::drawTapeFrame(){
	for(int i = -1; i < 9; i++){
		if(!i)	
			tapeCells[i+1].override_color(Gdk::RGBA("red"), Gtk::STATE_FLAG_NORMAL);
		else
			tapeCells[i+1].override_color(Gdk::RGBA("black"), Gtk::STATE_FLAG_NORMAL);
		tapeCells[i+1].set_label(std::to_string(BFM->getTapeAt(BFM->getDataPointer()+i)));
		tapeCells[i+1].set_tooltip_text(std::to_string(BFM->getDataPointer()+i));
	}
}

void debugGTK::drawControlFrame(){

	controlFrame.set_border_width(10);
	controlFrame.set_shadow_type(Gtk::SHADOW_ETCHED_OUT);
	controlFrame.set_label("Controls");
	controlFrame.add(controlGrid);

	controlGrid.set_border_width(5);
	controlGrid.attach(step, 1, 1, 1, 1);
	step.set_tooltip_text("Execute the next operation");

	controlGrid.attach(advance, 1, 2, 1, 1);
	advance.set_tooltip_text("Execute operations until a breakpoint is hit");

	controlGrid.attach(finish, 1, 3, 1, 1);
	finish.set_tooltip_text("Complete execution of the program");

	controlGrid.attach(modifySource, 1, 4, 1, 1);
	modifySource.set_tooltip_text("Replace the machine's source code with the contents of the text editor");

	controlGrid.attach(quit, 1, 5, 1, 1);
	quit.set_tooltip_text("Quit the debugger");

	step.set_border_width(5);
	finish.set_border_width(5);
	advance.set_border_width(5);
	quit.set_border_width(5);
}

void debugGTK::createSourceViewer(){
	sourceFrame.set_label("show file name some day");
	sourceFrame.set_shadow_type(Gtk::SHADOW_ETCHED_OUT);
	sourceFrame.set_border_width(10);
	sourceFrame.add(sourceWindow);
	sourceWindow.add(sourceView);
	sourceView.get_buffer()->set_text(BFM->getSource());

}

void debugGTK::highlightNextChar(){
		Gtk::TextIter startCharHighlight = sourceView.get_buffer()->get_iter_at_offset(BFM->getSourceIterator());
		Gtk::TextIter endCharHighlight = sourceView.get_buffer()->get_iter_at_offset(BFM->getSourceIterator()+1);

		Gtk::TextIter begSource = sourceView.get_buffer()->get_iter_at_offset(0);
		Gtk::TextIter endSource = sourceView.get_buffer()->get_iter_at_offset(BFM->getSource().length());
		sourceView.get_buffer()->remove_tag(charHighlight, begSource, endSource);
		sourceView.get_buffer()->apply_tag(charHighlight, startCharHighlight, endCharHighlight);
}

void debugGTK::createTagTable(){
	charHighlight = Gtk::TextBuffer::Tag::create();
	charHighlight->property_background() = "orange";
	sourceView.get_buffer()->get_tag_table()->add(charHighlight);
}

debugGTK::~debugGTK(){}

void debugGTK::stepF(){
	char retOp = BFM->processChar(1);
}

void debugGTK::advanceToEnd(){
	BFM->processSource();
	drawTapeFrame();
}

void debugGTK::advanceToHalt(){
	while(BFM->processChar(1) != '@');
	drawTapeFrame();
}

void debugGTK::endDebugger(){
	exit(0);
}

void debugGTK::replaceSource(){
	string newSource = sourceView.get_buffer()->get_text();
	BFM->replaceSource(newSource);
}
