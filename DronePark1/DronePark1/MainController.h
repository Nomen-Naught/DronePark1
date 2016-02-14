#pragma once
#include "ParkingLot.h"
#include "Config.h"
#include "DecideController.h"
#include "DatabaseController.h"
#include "ImageController.h"
#include "DronePilot.h"
#include "dronepark1.h"
#include <QObject>

class SweepController
{
private:
	//The current spot under review
	Spot* currentSpot;

	//A controller object which handles all communications with the physical drone
	FlightCommsController* droneComms;

	//A controller object which handles the actual flight of the drone.
	FlightController* dronePilot;

	//A controller object which handles all communications with the physical camera.
	ImageCommsController* imageComms;

	//A controller object which handles all the image analysis.
	ImageProcessController* imageProcessor;

	//A controller object which handles the decision of validity of the spot.
	DecideSpotController* stubDecider;

	//Change currentSpot to the next spot to be examined.
	int advanceSpot();

	//Update spot based of the return value sent from the DecideSpotController
	int updateSpot(bool decision);

public:

	SweepController();
	~SweepController();

	//Stops all current operations and shuts down the physical drone.
	int emergencyShutDown();

	//If the drone operation is currently engaged in scheduled mode, this method ends the scheduled
	//mode.If the drone isn’t in scheduled mode, this is a no - op.
	int endScheudle();

	//Start the drone in scheduled mode using the supplied schedule on the supplied Lot.
	int initiateSchedule(Schedule schedule, Lot lot);

	//Start a sweep of the supplied Lot immediately. Starts the member controllers to perform the sweep.
	int initiateSweep(Lot* lot);

	//Initializes the controllers and the connection to the camera and the drone
	int initializeDrone();

};

class DroneParkController : public QObject
{
	Q_OBJECT

public :
	//The currently loaded configuration for the session
	Config* currentConfig;
private:


	//controls the Drone and all processing associated with it.
	SweepController* sweepController;

	//Deviation from design, adding DatabaseController
	DatabaseController* databaseController;

public:

	DroneParkController();
	~DroneParkController();

	//Begins automated drone operations based on currentConfig.
	int beginDroneOperations();

	//Ends automated drone operations. Completes any current processing
	//before shutting down hardware and communications.
	int endDroneOperations();

	//Ends automated drone operations immediately.Disregards and destroys
	//any currently happening processing.
	int emergencyShutDown();

	//Instantiates and initializes all objects to do with operating the drone.Queries
	//the database to construct Lot and Spot data, creates controllers, and initializes
	//communications.
	int initiateDrone();

	//nitialize all controllers and models needed to start and display the first
	//screen.
	int initialize(DronePark1* gui);

	//Queries the database with an id and loads a new configuration.
	int loadConfig(int id);

	//Sets the currentConfig’s currentLot to the Lot passed in.
	int updateConfig_Lot(Lot newlot);

	//Sets the currentConfig’s currentSchedule
	int updateConfig_Schedule(Schedule newSchedule);

	//Adds the passed in DateTime to the currentConfig’s currentSchedule.
	int updateConfig_AddScheduleTime(QDateTime newTime);
	
	//Removes the passed in DateTime from the currentConfig’s currentSchedule
	int updateConfig_RemoveScheduleTime(QDateTime remTime);

	//Sets the currentConfig’s useSchedule member to true.Start the sweepControl
	//in scheduled mode
	int updateConfig_EnableSchedule();

	//Sets the currentConfig’s useSchedule member to false.End the
	//sweepControl’s scheduled mode
	int updateConfig_DisableSchedule();

	public slots:
	// Slot for startSweepButton slot
	void startSweepButtonSlot();

};

