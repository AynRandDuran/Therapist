#include <stdio.h>
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
