#include <stdio.h>
#include <climits>
#include <readline/history.h>
#include "../include/machine.h"
#include "machine.cpp"

#include "../include/debugC.h"

#include "../include/replEnvironment.h"
#include "replEnvironment.cpp"

#define CATCH_CONFIG_MAIN
#include "../include/catch.hpp"

TEST_CASE("Creating and running a machine", "[machine]"){
	string source = "+++>++[-<+>]<.";
	machine* testBFM = new machine(30000, true, false, source);

	REQUIRE_FALSE(testBFM == NULL);
	
	testBFM->processSource();

	REQUIRE(testBFM->getDataPointer() == 0);
	REQUIRE(testBFM->getTapeAt(0) == 5);

	FILE* abyss = fopen("/dev/null", "w");
	REQUIRE(testBFM->NAO(abyss) == 5);
	fclose(abyss);

	delete testBFM;
}

TEST_CASE("Create and manually modify a machine", "[machine]"){
	string source = "+++";
	machine *BFM = new machine(30000, true, false, source);

	REQUIRE_FALSE(BFM == NULL);
	
	BFM->processSource();
	REQUIRE(BFM->getDataPointer() == 0);
	REQUIRE(BFM->getTapeAt(0) == 3);
	
	REQUIRE(BFM->modifyDataPointer(1) == 1);
	REQUIRE(BFM->getDataPointer() == 1);
	REQUIRE(BFM->modifyDataPointer(2) == 2);
	REQUIRE(BFM->getDataPointer() == 2);
	REQUIRE(BFM->getTapeAt(BFM->getDataPointer()) == 0);

	REQUIRE(BFM->getSource() == "+++");
	char* moreSource = (char*)malloc(4); strcpy(moreSource, ">--");
	BFM->addToSource(moreSource);
	REQUIRE(BFM->getSource() == "+++>--");

	BFM->processSource();
	REQUIRE(BFM->getTapeAt(BFM->getDataPointer()) == -2);

	strcpy(moreSource, "++++");
	BFM->addToSource(moreSource);
	REQUIRE(BFM->getSource() == "+++>--++++");

	BFM->processSource();
	REQUIRE(BFM->getTapeAt(BFM->getDataPointer()) == 2);

	REQUIRE(BFM->incCell() == 3);
	REQUIRE(BFM->decCell() == 2);
	REQUIRE(BFM->decCell() == 1);
	REQUIRE(BFM->decCell() == 0);
	REQUIRE(BFM->incCell() == 1);

	REQUIRE(BFM->modifyDataPointer(0) == 0);
	REQUIRE(BFM->incPointer() == 1);
	REQUIRE(BFM->modifyTape(1, 0) == 0);
	REQUIRE(BFM->decPointer() == 0);

	delete BFM;
}

TEST_CASE("Tape wraparound", "[machine][tape]"){
	string source = "";
	machine *BFM = new machine(30000, true, false, source);

	BFM->modifyDataPointer(29999);
	REQUIRE(BFM->getDataPointer() == 29999);
	BFM->incPointer();
	REQUIRE(BFM->getDataPointer() == 0);

	BFM->decPointer();
	REQUIRE(BFM->getDataPointer() == 29999);
	BFM->decPointer();
	REQUIRE(BFM->getDataPointer() == 29998);
	BFM->decPointer();
	REQUIRE(BFM->getDataPointer() == 29997);

	delete BFM;
}

TEST_CASE("Process code char by char", "[machine]"){
	string source = "+-+-+-+";
	machine *BFM = new machine(30000, true, false, source);

	REQUIRE_FALSE(BFM == NULL);
	REQUIRE((char)BFM->processChar(1) == '+');
	REQUIRE((char)BFM->processChar(1) == '-');
	REQUIRE((char)BFM->processChar(1) == '+');
	REQUIRE((char)BFM->processChar(1) == '-');
	REQUIRE((char)BFM->processChar(1) == '+');
	REQUIRE((char)BFM->processChar(1) == '-');
	REQUIRE((char)BFM->processChar(1) == '+');

	REQUIRE(BFM->getTapeAt(0) == 1);

	delete BFM;
}

TEST_CASE("Changing the loop stack", "[machine][looping]"){
	string source = "+++[-]";
	machine *BFM = new machine(30000, true, false, source);
	REQUIRE_FALSE(BFM == NULL);

	BFM->processChar(1);
	BFM->processChar(1);
	BFM->processChar(1);
	BFM->processChar(1);

	for(int i = 0; i < 3; i++){
		REQUIRE(BFM->getStackTop() == 0);
		BFM->processChar(1);
		BFM->processChar(1);
		REQUIRE(BFM->getStackTop() == -1);
		BFM->processChar(1);
	}

	
}

TEST_CASE("Create a REPL environment", "[REPL]"){
	replEnvironment* TRE = new replEnvironment(false, true, 30000);
	REQUIRE_FALSE(TRE == NULL);

	delete TRE;
}

TEST_CASE("Execution from REPL", "[REPL]"){
	replEnvironment* TRE = new replEnvironment(false, true, 30000);
	REQUIRE_FALSE(TRE == NULL);

	char* source = (char*)malloc(32);
	strcpy(source, "++++++");
	TRE->process(source);
	REQUIRE((TRE->getMachine()->getTapeAt(0)) == 6);
	
	strcpy(source, "+++[-]");
	TRE->process(source);
	REQUIRE((TRE->getMachine()->getTapeAt(0)) == 0);

	delete TRE;
}

TEST_CASE("Recognize attempts to bind a name", "[REPL][bindings]"){
	replEnvironment* TRE = new replEnvironment(false, true, 30000);
	REQUIRE_FALSE(TRE == NULL);

	char* testBinding = (char*)malloc(32);
	strcpy(testBinding, "adder=,>,[-<+>]<.");
	REQUIRE((TRE->tryingToBind(testBinding)));

	strcpy(testBinding, "echoUntilZero=[,.]");
	REQUIRE((TRE->tryingToBind(testBinding)));

	strcpy(testBinding, "notabinding");
	REQUIRE_FALSE((TRE->tryingToBind(testBinding)));

	strcpy(testBinding, "+++++[-]");
	REQUIRE_FALSE((TRE->tryingToBind(testBinding)));

	delete TRE;
}

TEST_CASE("Recognize existing bindings", "[REPL][bindings]"){
	replEnvironment* TRE = new replEnvironment(false, true, 30000);
	REQUIRE_FALSE(TRE == NULL);

	char* potentialKey = (char*)malloc(32);

	strcpy(potentialKey, "pushRightOne");
	REQUIRE(strcmp(TRE->expandProcedure(potentialKey), "[->+<]") == 0);
	strcpy(potentialKey, "add");
	REQUIRE(strcmp(TRE->expandProcedure(potentialKey), ",>, pushLeftOne .<.") == 0);

	strcpy(potentialKey, "definitelyNotBound");
	REQUIRE(strcmp(TRE->expandProcedure(potentialKey), potentialKey) == 0);
	strcpy(potentialKey, "notBoundEither");
	REQUIRE(strcmp(TRE->expandProcedure(potentialKey), potentialKey) == 0);

	delete TRE;
}

TEST_CASE("History usage", "[REPL][history]"){
	char* cmd1 = (char*)malloc(16);
	char* cmd2 = (char*)malloc(16);
	char* cmd3 = (char*)malloc(16);
	char* cmd4 = (char*)malloc(16);
	strcpy(cmd1, "+++");
	strcpy(cmd2, "--");
	strcpy(cmd3, "arbitrary");
	strcpy(cmd4, "commands");

	add_history(cmd1);
	add_history(cmd2);
	add_history(cmd3);
	add_history(cmd4);
	write_history("/home/pat/.bfsh_history");
	read_history("/home/pat/.bfsh_history");

	REQUIRE(history_search_pos(cmd1, 1, 0) != -1);
	REQUIRE(history_search_pos(cmd2, 1, 0) != -1);
	REQUIRE(history_search_pos(cmd3, 1, 0) != -1);
	REQUIRE(history_search_pos(cmd4, 1, 0) != -1);
}

TEST_CASE("Add or replace bindings", "[REPL][bindings]"){
	replEnvironment* TRE = new replEnvironment(false, true, 30000);
	REQUIRE_FALSE(TRE == NULL);

	char* potentialBinding = (char*)malloc(32);
	strcpy(potentialBinding, "test=+++.");
	REQUIRE(TRE->addNewProcedure(potentialBinding));
	strcpy(potentialBinding, "+=-");
	REQUIRE(TRE->addNewProcedure(potentialBinding));
	strcpy(potentialBinding, "test=++++[-].");
	REQUIRE(TRE->addNewProcedure(potentialBinding));
	delete TRE;
}

TEST_CASE("Prove I can do ASCII output or not", "[machine][options][ASCII]"){
	string source = "+++++[->+++++++++++++<]>"; //65, ASCII 'A'
	machine *BFM = new machine(30000, true, false, source);

	FILE* abyss = fopen("/dev/null", "w");
	BFM->processSource();
	REQUIRE((BFM->*BFM->output)(abyss) == 65);
	delete BFM;

	BFM = new machine(30000, true, true, source);
	BFM->processSource();
	REQUIRE((BFM->*BFM->output)(abyss) == 'A');
	delete BFM;

	fclose(abyss);
}

TEST_CASE("Define tape length at startup", "[machine][options][tape]"){
	string source = "who cares? I'm not running code";
	machine *BFM = new machine(1000, true, false, source);

	REQUIRE(BFM->getTapeLength() == 1000);
	REQUIRE_FALSE(BFM->getTapeLength() == 42);

	delete BFM;
	BFM = new machine(30000, true, false, source);

	REQUIRE(BFM->getTapeLength() == 30000);
	REQUIRE_FALSE(BFM->getTapeLength() == 1000);

	delete BFM;
}

TEST_CASE("Un/signed cells and wraparound", "[machine][options][tape]"){
	replEnvironment* TRE = new replEnvironment(false, false, 1000);
	
	char* source = (char*)malloc(16);

	strcpy(source, "+");
	REQUIRE(TRE->getMachine()->getTapeAt(0) == 0);
	TRE->process(source);
	REQUIRE(TRE->getMachine()->getTapeAt(0) == 1);

	strcpy(source, "--");
	TRE->process(source);
	REQUIRE(TRE->getMachine()->getTapeAt(0) == INT_MAX);

	strcpy(source, "+");
	TRE->process(source);
	REQUIRE(TRE->getMachine()->getTapeAt(0) == 0);

	delete TRE;

	TRE = new replEnvironment(false, true, 1000);

	REQUIRE(TRE->getMachine()->getTapeAt(0) == 0);
	TRE->getMachine()->modifyTape(0, INT_MAX);
	REQUIRE(TRE->getMachine()->getTapeAt(0) == INT_MAX);
	
	strcpy(source, "+");
	TRE->process(source);
	REQUIRE(TRE->getMachine()->getTapeAt(0) == INT_MIN);

	strcpy(source, "-");
	TRE->process(source);
	REQUIRE(TRE->getMachine()->getTapeAt(0) == INT_MAX);

	delete TRE;
}






