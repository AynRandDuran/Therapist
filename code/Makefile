all: machineMake replMake

machineMake: therapist.cpp machine.cpp debugC.cpp
	g++ therapist.cpp machine.cpp debugC.cpp -o therapist -lcurses -ltinfo
replMake: therapist.cpp machine.cpp therapistREPL.cpp replEnvironment.cpp
	g++  machine.cpp therapistREPL.cpp replEnvironment.cpp -o bfsh -lreadline
