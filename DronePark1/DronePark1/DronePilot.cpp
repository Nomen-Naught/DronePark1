#include "DronePilot.h"
#include "ReturnCodes.h"
#include <QThread>

#undef _DEBUG
#include "PythonQt.h"
#define _DEBUG 1



//FlightController constructor
FlightController::FlightController()
{

}

//FlightController destructor
FlightController::~FlightController()
{

}

//Initiates the drone flightpath
void FlightController::asyncStartFlight()
{

	PythonQt::init();
	PythonQtObjectPtr mainModule =
		PythonQt::self()->getMainModule();
//	QVariant result = mainModule.evalScript(mainModule, "19*2+4", Py_eval_input);

	QThread::msleep(5000);
	emit resultReady();
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

//Opens the link to the drone
int FlightCommsController::openLink()
{
	return RC_ERR;
}

//Scans for drones to connect with
int FlightCommsController::scanInterface()
{
	return RC_ERR;
}


//Calls the operations needed to find and connect to the drone
int FlightCommsController::connectToDrone()
{
	return RC_ERR;
}

//Placeholder for startup operations
int FlightCommsController::init_drivers()
{
	return RC_ERR;
}

//Operation used to identify drone for connection
int FlightCommsController::crazyflie()
{
	return RC_ERR;
}

//Operation used for receiving data from drone
int FlightCommsController::add_callback()
{
	return RC_ERR;
}

//Closes link to drone
int FlightCommsController::closeLink()
{
	return RC_ERR;
}