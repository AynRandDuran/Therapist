#include <stdio.h>
#include "../include/machine.h"
#include "machine.cpp"
#include "../include/debugC.h"
#include "../include/replEnvironment.h"

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

TEST_CASE("Should be able to process code char by char", "[machine]"){
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

TEST_CASE("Testing change in loop stack", "[machine][looping]"){
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
