#include "../include/debugGTK.h"
#include <gtkmm/application.h>

int main(int argc, char** argv){
	auto app = Gtk::Application::create(argc, argv, "org.gtkmm.brainf*_debugger");
	debugGTK debugger;
	debugger.startObserving();
	debugger.createWindow();
	debugger.drawWindowContents();
	return app->run(debugger);
}
