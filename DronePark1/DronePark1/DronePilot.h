#pragma once
#include "Config.h"
#include "ParkingLot.h"

class FlightController
{
private:

	//Placeholder representing connection object
	int connection;

	//Current parking lot information
	Config currentConfig;

	//Placeholders representing drone flight attributes
	int pitch, roll, thrust, yaw;

	//Current spot
	Spot currentSpot;

	//The lot we’re going to traverse.
	Lot currentLot;

public:

	FlightController();
	~FlightController();

	//Initiates the drone flightpath
	int asyncStartFlight();

	//Creates a flightpath based on the current configuration
	int setFlightPath(Config config);

	//Ends the drones flight
	int asyncStopFlight();

	//Begins the image capture for the indicated spot
	int captureAtSpot(Spot spot);

	//Sets flight values to safely land the Drone
	int emergencyStop();

};

class FlightCommsController
{
private: 

	//Flag indicating that the Drone is connected to the system
	bool crazyflieConnected;

	//Flag used for testing
	bool enable_debug_driver;

	//Generic connection object used to store connection information relating to the drone
	int connection;
	
	//Opens the link to the drone
	int openLink();

	//Scans for drones to connect with
	int scanInterface();

public:

	//Calls the operations needed to find and connect to the drone
	int connectToDrone();

	//Placeholder for startup operations
	int init_drivers();

	//Operation used to identify drone for connection
	int crazyflie();

	//Operation used for receiving data from drone
	int add_callback();

	//Closes link to drone
	int closeLink();

};