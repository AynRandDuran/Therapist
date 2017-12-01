#include "../include/debugGTK.h"
#include <gtkmm/application.h>

int main(int argc, char** argv){
	auto app = Gtk::Application::create(argc, argv, "Brainfuck GTKMM debugger");

	debugGTK debugger;

	return app->run(debugger);
}
