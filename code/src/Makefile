all: machine repl debugGTK

machine: therapist.cpp machine.cpp debugC.cpp
	g++ therapist.cpp machine.cpp debugC.cpp -o therapist -lcurses -ltinfo
repl: therapist.cpp machine.cpp therapistREPL.cpp replEnvironment.cpp
	g++ machine.cpp therapistREPL.cpp replEnvironment.cpp -o bfsh -lreadline
debugGTK: debugGTKMain.cpp debugGTK.cpp
	g++ debugGTKMain.cpp debugGTK.cpp -o gtkDebug `pkg-config gtkmm-3.0 --cflags --libs`

