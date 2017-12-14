#include "../include/debugGTK.h"
#include <gtkmm/application.h>

//begin GTK
int main(int argc, char** argv){
	auto app = Gtk::Application::create(argc, argv, "org.gtkmm.brainf*_debugger");
	debugGTK *debugger = new debugGTK;
	debugger->startObserving();
	debugger->createWindow();
	debugger->drawWindowContents();
	app->run(*debugger);
	delete debugger;
	return 0;
}
