#include "../include/debugGTK.h"
#include "machine.cpp"
#include <iostream>

debugGTK::debugGTK():
	step("Step"),
	finish("Finish"),
	quit("Quit"),
	advance("Advance"),
	modifySource("Insert"),
	ASCIIToggle("ASCII IO")
{
	//define a default machine on creation
	BFM = new machine(30000, true, false, "");
}

void debugGTK::createWindow(){
	add(masterGrid);
	masterGrid.set_border_width(5);
	set_title("Therapist Debugger");
	set_size_request(600, 400);
}

void debugGTK::drawWindowContents(){
	get_size(windowWidth, windowHeight);

	//add major components to the window
	masterGrid.attach(controlFrame, 1, 1, 1, 7);
	masterGrid.attach(tapeFrame, 2, 1, windowWidth-1, 1);
	masterGrid.attach(sourceFrame, 2, 2, windowWidth-1, windowHeight-1);
	masterGrid.attach(outputFrame, 1, 8, 1, 1);
	masterGrid.attach(stackFrame, windowWidth+1, 1, 1, 4);

	//handle actual creation of those components
	drawControlFrame();
	createTapeFrame();
	createTagTable();
	createSourceViewer();
	createOutputViewer();
	createStackViewer();
	highlightNextChar();
	show_all_children();
}

//Link observers and subjects, buttons and machine changes, and so on
void debugGTK::startObserving(){
	BFM->notify_tape_change().connect(sigc::mem_fun(*this, &debugGTK::drawTapeFrame));
	BFM->get_cell_for_output().connect(sigc::mem_fun(*this, &debugGTK::outputCell));
	BFM->notify_stack_change().connect(sigc::mem_fun(*this, &debugGTK::updateStackViewer));
	BFM->request_some_input().connect(sigc::mem_fun(*this, &debugGTK::inputCell));
	this->add_events(Gdk::KEY_PRESS_MASK);
	this->signal_key_release_event().connect(sigc::mem_fun(*this, &debugGTK::handleKeyPress));

	step.signal_clicked().connect(sigc::mem_fun(*this, &debugGTK::stepF));
	step.signal_clicked().connect(sigc::mem_fun(*this, &debugGTK::highlightNextChar));
	advance.signal_clicked().connect(sigc::mem_fun(*this, &debugGTK::advanceToHalt));
	advance.signal_clicked().connect(sigc::mem_fun(*this, &debugGTK::highlightNextChar));
	finish.signal_clicked().connect(sigc::mem_fun(*this, &debugGTK::advanceToEnd));
	finish.signal_clicked().connect(sigc::mem_fun(*this, &debugGTK::highlightNextChar));
	modifySource.signal_clicked().connect(sigc::mem_fun(*this, &debugGTK::replaceSource));
	modifySource.signal_clicked().connect(sigc::mem_fun(*this, &debugGTK::highlightNextChar));
	quit.signal_clicked().connect(sigc::mem_fun(*this, &debugGTK::endDebugger));
	ASCIIToggle.signal_clicked().connect(sigc::mem_fun(*this, &debugGTK::toggleASCIIMode));
}

//handle key presses for direct tape manipulation
bool debugGTK::handleKeyPress(GdkEventKey* event){
	tapeFrame.set_focus_on_click(true);
	switch(event->keyval){
		case 105: //i
			BFM->incCell();
			break;
		case 107: //k
			BFM->decCell();
			break;
		case 106: //j
			BFM->decPointer();
			break;
		case 108: //l
			BFM->incPointer();
			break;
	}

	return true;
}

//toggle the machine's ASCII IO mode
void debugGTK::toggleASCIIMode(){
	cout << "ascii mode: " << (ASCIIToggle.get_active() ? "on":"off") << endl;
	BFM->toggleASCIIMode();
}

//Create and handle the containers that draw the machine's stack
void debugGTK::createStackViewer(){
	stackFrame.set_border_width(10);
	stackFrame.set_label("Stack");
	stackFrame.set_tooltip_text("The machine's stack");
	stackFrame.set_shadow_type(Gtk::SHADOW_ETCHED_OUT);
	stackFrame.add(stackWindow);
	stackWindow.add(stackGrid);

	for(int i = 0; i < 5; i++){
		stackCells[i].set_margin_start(10);
		stackCells[i].set_margin_end(10);
		stackCells[i].set_margin_top(5);
		stackCells[i].set_margin_bottom(5);
		stackGrid.attach(stackCells[i], 1, 5-i, 1, 1);
	}
	updateStackViewer();
	
}

//redraw the machine's stack as it changes
void debugGTK::updateStackViewer(){
	int top = BFM->getStackTop();
	int *localStack = BFM->getStack();

	for(int i = 0; i < 5; i++){
		stackCells[i].set_text("");
		if(i <= top)
			stackCells[i].set_text(std::to_string(localStack[top-i]));
	}
}

//Create the container that will show the machine's output as it's processed
void debugGTK::createOutputViewer(){
	outputFrame.set_border_width(10);
	outputFrame.set_label("Output");
	outputFrame.set_tooltip_text("The output of your program");
	outputFrame.set_shadow_type(Gtk::SHADOW_ETCHED_OUT);
	outputFrame.add(outputWindow);
	outputWindow.add(outputLabel);

	outputLabel.set_selectable(true);
}

void debugGTK::inputCell(){
	cout << ">>> ";
}

//handle the machine's ASCII mode before displaying output
void debugGTK::outputCell(int cell){
	FILE* abyss = fopen("/dev/null", "w");
	(BFM->*BFM->output)(abyss);

	if(BFM->getASCIIMode())
		outputBuffer << (char)cell;
	else
		outputBuffer << cell;
	outputLabel.set_text(outputBuffer.str());
}

//Create the container responsible for drawing the machine's tape
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

//handle out-of-bounds data pointer requests
int debugGTK::wrappedDataPointer(int newDP){
	if(newDP > BFM->getTapeLength()-1)
		newDP = newDP - BFM->getTapeLength();
	if(newDP < 0)
		newDP = BFM->getTapeLength() + newDP;
	return newDP;
}

//Redraw the tape as it changes, moving the highlighting 'cursor'
void debugGTK::drawTapeFrame(){
	for(int i = -1; i < 9; i++){
		if(!i){
			tapeCells[i+1].override_color(Gdk::RGBA("red"), Gtk::STATE_FLAG_NORMAL);
		}
		int dp = BFM->getDataPointer();
		tapeCells[i+1].set_label(std::to_string(BFM->getTapeAt(wrappedDataPointer(i+dp))));
		tapeCells[i+1].set_tooltip_text(std::to_string(wrappedDataPointer(i+dp)));
	}
}

//create the control buttons held by controlFrame
void debugGTK::drawControlFrame(){

	controlFrame.set_border_width(10);
	controlFrame.set_shadow_type(Gtk::SHADOW_ETCHED_OUT);
	controlFrame.set_label("Controls");
	controlFrame.add(controlGrid);

	controlGrid.set_border_width(5);
	controlGrid.attach(ASCIIToggle, 1, 1, 1, 1);
	ASCIIToggle.set_tooltip_text("Enable or disable ASCII IO");

	controlGrid.attach(step, 1, 2, 1, 1);
	step.set_tooltip_text("Execute the next operation");

	controlGrid.attach(advance, 1, 3, 1, 1);
	advance.set_tooltip_text("Execute operations until a breakpoint is hit");

	controlGrid.attach(finish, 1, 4, 1, 1);
	finish.set_tooltip_text("Complete execution of the program");

	controlGrid.attach(modifySource, 1, 5, 1, 1);
	modifySource.set_tooltip_text("Insert the contents of the text editor into the machine's source");

	controlGrid.attach(quit, 1, 6, 1, 1);
	quit.set_tooltip_text("Quit the debugger");

	step.set_border_width(5);
	finish.set_border_width(5);
	advance.set_border_width(5);
	quit.set_border_width(5);
}

//Create a text editor container to contain BF source code
void debugGTK::createSourceViewer(){
	sourceFrame.set_label("A Brainf* project");
	sourceFrame.set_shadow_type(Gtk::SHADOW_ETCHED_OUT);
	sourceFrame.set_border_width(10);
	sourceFrame.add(sourceWindow);
	sourceWindow.add(sourceView);
	sourceView.get_buffer()->set_text(BFM->getSource());

}

//Highlight the next char of source to be executed
void debugGTK::highlightNextChar(){
	Gtk::TextIter startCharHighlight = sourceView.get_buffer()->get_iter_at_offset(BFM->getSourceIterator());
	Gtk::TextIter endCharHighlight = sourceView.get_buffer()->get_iter_at_offset(BFM->getSourceIterator()+1);

	Gtk::TextIter begSource = sourceView.get_buffer()->get_iter_at_offset(0);
	Gtk::TextIter endSource = sourceView.get_buffer()->get_iter_at_offset(BFM->getSource().length());
	sourceView.get_buffer()->remove_tag(charHighlight, begSource, endSource);
	sourceView.get_buffer()->apply_tag(charHighlight, startCharHighlight, endCharHighlight);
}

//Create the tag necessary to change text color in a TextView
void debugGTK::createTagTable(){
	charHighlight = Gtk::TextBuffer::Tag::create();
	charHighlight->property_background() = "orange";
	sourceView.get_buffer()->get_tag_table()->add(charHighlight);
}

debugGTK::~debugGTK(){
	delete BFM;
	delete this;
}

//Step the machine forward once
void debugGTK::stepF(){
	char retOp = BFM->processChar(1);
}

//Execute until the end of the machine's source code
void debugGTK::advanceToEnd(){
	BFM->processSource();
	drawTapeFrame();
}

//execute until the next breakpoint @
void debugGTK::advanceToHalt(){
	while(BFM->processChar(1) != '@');
	drawTapeFrame();
}

void debugGTK::endDebugger(){
	exit(0);
}

//insert the contents of the machine's source with contents of the source TextView
void debugGTK::replaceSource(){
	string newSource = sourceView.get_buffer()->get_text();
	BFM->replaceSource(newSource);
	outputBuffer.str(std::string());
	outputLabel.set_text("");
}
