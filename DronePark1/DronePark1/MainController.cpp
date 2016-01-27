#include "MainController.h"

#define DEFAULT_CONFIG 0

//TODO: Nick: implement beginDroneOperations
//Begins automated drone operations based on currentConfig.
int DroneParkController::beginDroneOperations() 
{
	return RC_ERR;
}

//TODO: Nick: implement endDroneOperations
//Ends automated drone operations. Completes any current processing
//before shutting down hardware and communications.
int DroneParkController::endDroneOperations()
{
	return RC_ERR;
}

//TODO: Nick: implement emergencyShutDown
//Ends automated drone operations immediately.Disregards and destroys
//any currently happening processing.
int DroneParkController::emergencyShutDown() 
{
	return RC_ERR;
}

//TODO: Nick: implement initiateDrone
//Instantiates and initializes all objects to do with operating the drone.Queries
//the database to construct Lot and Spot data, creates controllers, and initializes
//communications.
int DroneParkController::initiateDrone()
{
	return RC_ERR;
}

//TODO: Nick: implement initialize
//Initialize all controllers and models needed to start and display the first
//screen.
int DroneParkController::initialize()
{
	//Declarations--------------

	int rc = RC_OK;			//Return code

	//End Delcarations----------

	// Allocate and set databaseController, deallocated in destructor hopefully
	databaseController = new DatabaseController();

	//connectToDb is currently a placeholder btw
	rc = databaseController->connectToDb("Test");
	DP_ASSERT(rc == RC_OK, "databaseController->connectToDb")

	//Load the default config
	rc = loadConfig(DEFAULT_CONFIG);
	DP_ASSERT(rc == RC_OK, "loadConfig")

	//We need to hook up our Spots with our database observers
	for (std::list<Spot*>::const_iterator iterator = currentConfig->getCurrentLot()->getSpots()->begin(),
		end = currentConfig->getCurrentLot()->getSpots()->end();
		iterator != end;
		++iterator)
	{
		//Hook up ticketed field to database writer
		QObject::connect(*iterator, SIGNAL(spotTicketedChanged(int, bool)),
					     databaseController, SLOT(updateSpotTicketed(int, bool)));

		//Hook up empty field to database writer
		QObject::connect(*iterator, SIGNAL(spotEmptyChanged(int, bool)),
			databaseController, SLOT(updateSpotEmpty(int, bool)));
	}

	//Create sweepController object
	//sweepController = SweepController();
	//^^ above needs to be initialized (maybe? this is just supposed to be enough for first page)

	//At this point, something needs to update the UI with the data we have in currentConfig!!
	//TODO: Update UI in droneParkController->initialize();

	return rc;
}

//TODO: Nick: implement loadConfig
//Queries the database with an id and loads a new configuration.
int DroneParkController::loadConfig(int id)
{
	int rc = RC_OK;		// return code

	rc = databaseController->queryConfig(id, &currentConfig);

	return rc;
}

//TODO: Nick: implement ticketIssued
//Sets the Spot object’s member “ticketed” to true.
int DroneParkController::ticketIssued(Spot spot)
{
	return RC_ERR;
}

//TODO: Nick: implement updateConfig_Lot
//Sets the currentConfig’s currentLot to the Lot passed in.
int DroneParkController::updateConfig_Lot(Lot newlot)
{
	return RC_ERR;
}

//TODO: Nick: implement updateConfig_Schedule
//Sets the currentConfig’s currentSchedule
int DroneParkController::updateConfig_Schedule(Schedule newSchedule)
{
	return RC_ERR;
}

//TODO: Nick: implement updateConfig_AddScheduleTime
//Adds the passed in DateTime to the currentConfig’s currentSchedule.
int DroneParkController::updateConfig_AddScheduleTime(QDateTime newTime)
{
	return RC_ERR;
}

//TODO: Nick: implement updateConfig_RemoveScheduleTime
//Removes the passed in DateTime from the currentConfig’s currentSchedule
int DroneParkController::updateConfig_RemoveScheduleTime(QDateTime remTime)
{
	return RC_ERR;
}

//TODO: Nick: implement updateConfig_EnableSchedule
//Sets the currentConfig’s useSchedule member to true.Start the sweepControl
//in scheduled mode
int DroneParkController::updateConfig_EnableSchedule()
{
	return RC_ERR;
}

//TODO: Nick: implement updateConfig_DisableSchedule
//Sets the currentConfig’s useSchedule member to false.End the
//sweepControl’s scheduled mode
int DroneParkController::updateConfig_DisableSchedule()
{
	return RC_ERR;
}


//DroneParkController destructor
DroneParkController::~DroneParkController()
{
	// Not sure if this is a good idea... what about the chained pointers?
	// Nuke databaseController object if we got it
	if (databaseController != NULL)
	{
		delete databaseController;
	}

	// Nuke currentConfig object if we got it
	if (currentConfig != NULL)
	{
		delete currentConfig;
	}
}

//TODO: Nick: implement emergencyShutDown
//Stops all current operations and shuts down the physical drone.
int SweepController::emergencyShutDown()
{
	return RC_ERR;
}

//TODO: Nick: implement endScheudle
//If the drone operation is currently engaged in scheduled mode, this method ends the scheduled
//mode.If the drone isn’t in scheduled mode, this is a no - op.
int SweepController::endScheudle()
{
	return RC_ERR;
}

//TODO: Nick: implement initiateSchedule
//Start the drone in scheduled mode using the supplied schedule on the supplied Lot.
int SweepController::initiateSchedule(Schedule schedule, Lot lot)
{
	return RC_ERR;
}

//TODO: Nick: implement initiateSweep
//Start a sweep of the supplied Lot immediately. Starts the member controllers to perform the sweep.
int SweepController::initiateSweep(Lot lot)
{
	return RC_ERR;
}

//TODO: Nick: implement initializeDrone
//Initializes the controllers and the connection to the camera and the drone
int SweepController::initializeDrone()
{
	return RC_ERR;
}

//TODO: Nick: implement advanceSpot
//Change currentSpot to the next spot to be examined.
int SweepController::advanceSpot()
{
	return RC_ERR;
}

//TODO: Nick: implement updateSpot
//Update spot based of the return value sent from the DecideSpotController
int SweepController::updateSpot(bool decision)
{
	return RC_ERR;
}