#include "../include/debugGTK.h"
#include <gtkmm/application.h>

int main(int argc, char** argv){
	auto app = Gtk::Application::create(argc, argv, "org.gtkmm.brainfuck_debugger");
	debugGTK debugger;
	debugger.coupleForObservation();
	return app->run(debugger);
}
