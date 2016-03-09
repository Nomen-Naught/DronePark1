#include "DronePilot.h"
#include "ReturnCodes.h"
#include <QThread>
//#include "PythonQt.h"

class SweepController;

//FlightController constructor
FlightController::FlightController()
{

}

//FlightController destructor
FlightController::~FlightController()
{

}

//Initiates the drone flightpath
void FlightController::asyncStartFlight(ControlInterface* contInt)
{
	/*
	PythonQt::init(PythonQt::IgnoreSiteModule | PythonQt::RedirectStdOut);
	PythonQtObjectPtr mainModule = PythonQt::self()->getMainModule();
	PythonQtScriptingConsole* console = new PythonQtScriptingConsole(NULL, mainModule);
	QVariant result = mainModule.evalScript("19*2+4", Py_eval_input);

	mainModule.addObject("contInt", contInt);

	// evaluate the python script which is defined in the resources
	mainModule.evalFile(":/DronePark1/GettingStarted.py");

	//console->appendCommandPrompt();
	//console->show();

	*/
	return;
}

//Creates a flightpath based on the current configuration
int FlightController::setFlightPath(Config config)
{
	return RC_ERR;
}

//Ends the drones flight
int FlightController::asyncStopFlight()
{
	return RC_ERR;
}

//Begins the image capture for the indicated spot
int FlightController::captureAtSpot(Spot spot)
{
	return RC_ERR;
}

//Sets flight values to safely land the Drone
int FlightController::emergencyStop()
{
	return RC_ERR;
}

