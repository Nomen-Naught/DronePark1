#include "dronepark1.h"
#include <QtWidgets/QApplication>
#include "Model.h"
#include "Controllers.h"
#include "Tests_Misc.h"

int main(int argc, char *argv[])
{
	//Declarations--------------

	int rc = RC_OK;								//Return code

	DroneParkController droneParkController;	//Main Program Controller, stays in scope of main function for now... might need to be moved to somewhere more available to Qt events

	//End Delcarations----------

	// Launches Qt window
	QApplication a(argc, argv);
	DronePark1 w;

	//Let's find some UI elements

	//

	w.show();

	// Program Start use-case BEGIN***

	//Should instantiate a database controller for this session AND do everything needed to load first page!!
	rc = droneParkController.initialize();
	DP_ASSERT(rc == RC_OK, "droneParkController.initialize()");

	// Program Start use-case END***

	return a.exec();
}
