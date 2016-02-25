#pragma once
#include "Config.h"
#include "ParkingLot.h"
#include <QObject>
#include <QThread>
#include "gui/PythonQtScriptingConsole.h"
#include "ControlInterface.h"

class SweepController;

class FlightController : public QObject
{
	Q_OBJECT
		QThread pilotWorkerThread;

private:

	//Placeholder representing connection object
	int connection;

	//Current parking lot information
	Config currentConfig;

	//Current spot
	Spot currentSpot;

	//The lot we’re going to traverse.
	Lot currentLot;

public:

	FlightController();
	~FlightController();

	//Creates a flightpath based on the current configuration
	int setFlightPath(Config config);

	//Ends the drones flight
	int asyncStopFlight();

	//Begins the image capture for the indicated spot
	int captureAtSpot(Spot spot);

	//Sets flight values to safely land the Drone
	int emergencyStop();

public slots:
	//Initiates the drone flightpath
	void asyncStartFlight(ControlInterface*);

signals:
	void resultReady();

};

