#ifndef __REPL__
#define __REPL__
#include "machine.h"
#include <unordered_map>
class replEnvironment{

private:
	machine* localMachine;
	unordered_map<string, string> bindings;

public:
	replEnvironment(bool AIO, bool signedCells, int tapeLength);
	char* expandProcedure(char* statements);
	bool process(char* input);
	void tokenizeForExpansion(char* input);
};

#endif
