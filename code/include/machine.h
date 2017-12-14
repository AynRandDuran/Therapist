#ifndef __env__
#define __env__
#include <string>
#include <cstdlib>
#include <iostream>
#include <sigc++/sigc++.h>
using namespace std;

class machine{
private:
	int tapeSize;
	int dataPointer = 0;
	bool isSigned;
	bool asciiMode;

	int* tape;

	int* stacc;
	int topOfStacc;

	string sourceBF = "";
	int instructionPointer = 0;

	long int MINWRAP, MAXWRAP, RANGE;

	//create signal types for the GTK debugger
	typedef sigc::signal<void> type_tape_change_signal;
	type_tape_change_signal m_tape_change_signal;

	typedef sigc::signal<void, int> output_cell_signal;
	output_cell_signal send_cell_output;

	typedef sigc::signal<void> stack_change_signal;
	stack_change_signal signal_redraw_stack;

	typedef sigc::signal<void> input_request;
	input_request signal_input_request;

public:
	machine(int tSize, bool sCells, bool AIO, string source);

	type_tape_change_signal notify_tape_change();
	output_cell_signal get_cell_for_output();
	stack_change_signal notify_stack_change();
	input_request request_some_input();

	void toggleASCIIMode();
	bool getASCIIMode();

	int getTapeLength();
	int modifyTape(int DP, int mod);
	int getTapeAt(int DP);

	int modifyDataPointer(int mod);
	int getDataPointer();

	string getSource();
	int getSourceIterator();
	void addToSource(char* moreSource);
	void replaceSource(string newSource);

	int getStackTop();
	int* getStack();

	/*Functions corresponding to the 8 BF operations*/
	int incPointer();
	int decPointer();

	int incCell();
	int decCell();

	int NAO(FILE* file);
	int AO(FILE* file);

	int NAI();
	int AI();

	int leftBracket(int position);
	int rightBracket(int position);

	/*Recieve a string to process as BF code*/
	int processChar(int iterMod);
	void processSource();

	/*
	Define IO function pointers
	These allow us to easily switch which IO functions
	are used based on the -a flag
	*/
	int (machine::*output)(FILE* file);
	int (machine::*input)();

	~machine();
};

#endif
