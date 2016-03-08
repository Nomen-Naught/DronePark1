#include "MainController.h"
#include "dronepark1.h"
#include "QMessageBox.h"
//#include "Python.h"
//#include "PythonQt.h"

#define DEFAULT_CONFIG 1


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

//Initialize all controllers and models needed to start and display the first
//screen.
int DroneParkController::initialize(DronePark1* gui)
{
	//Declarations--------------

	int rc = RC_OK;			//Return code

	//End Delcarations----------

	// Allocate and set databaseController, deallocated in destructor hopefully
	databaseController = new DatabaseController();

	// Connect to the database (This doesn't really need an argument, it does nothing
	rc |= databaseController->connectToDb("Test");
	DP_ASSERT(rc, "databaseController->connectToDb");

	////Load the default config
	rc |= loadConfig(DEFAULT_CONFIG);
	DP_ASSERT(rc, "loadConfig");

	//Start populating the gui with the lot
	gui->buildLotGui(currentConfig->getCurrentLot());

	//We need to hook up our Spots with our database observers and the gui
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

		//Hook up illegal field to database writer
		QObject::connect(*iterator, SIGNAL(spotIllegalChanged(int, bool)),
						 databaseController, SLOT(updateSpotIllegal(int, bool)));

		//Connect fields with gui
		gui->connectNewSpot(*iterator);
	}

	//Connect the startSweep button to this controller's handler.
	//For whatever reason, I have to do it like this, can't pass a 'this' pointer to the gui
	QObject::connect(gui->returnUI().startSweepButton, SIGNAL(startSweep()),
					 this, SLOT(startSweepButtonSlot()));

	//Instantiating the sweepController and FlightConttoller now... it caused weird bugs in release mode to do it later
	sweepController = new SweepController(currentConfig->getCurrentLot());
	QObject::connect(sweepController, SIGNAL(decideSpotPass(Spot*, bool, int)), this, SLOT(decideSpot(Spot*, bool, int)));

	connect(gui, SIGNAL(newLotOpen(NewLot*)), this, SLOT(newLotDialogOpen(NewLot*)));


	sweepController->dronePilot = new FlightController();

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

//DroneParkController constructor
DroneParkController::DroneParkController()
{
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

// Click handler for startSweepButton
void DroneParkController::startSweepButtonSlot()
{
	//Declarations
	int rc = RC_OK;

	// Create controllers if we need to
	if (sweepController == NULL)
	{
		// Create sweep controller, should create all other controllers
		sweepController = new SweepController(currentConfig->getCurrentLot());
		QObject::connect(sweepController, SIGNAL(decideSpotPass(Spot*, bool, int)), this, SLOT(decideSpot(Spot*, bool, int)));
	}

	// NICK: Getting rid of this for now, current plan is to allow python to do everything related to drone stuff and C++ only
	// hosts the python. C++ will handle threading as well, hopefully this works out!
	//------------------------------------------------------------------------------------------------------------------------
	// Call initialize drone, should handle everything, if already initialzed, should be a no-op
	//rc |= sweepController->initializeDrone();
	//DP_ASSERT(rc, "sweepController->initializeDrone");

	//Check that we have a lot to sweep
	if (currentConfig->getCurrentLot() == NULL)
	{
		//TODO: startSweep: Handle when there's no lot loaded!!!
		goto exit;
	}

	//Start the sweep, (2/16) this is gonna do everything!!!
	rc |= sweepController->initiateSweep(currentConfig->getCurrentLot());

exit:

	DP_ASSERT(rc, "sweepController->initializeDrone exit");

	return;
}

//Decide the spot passed in by checking the stub that was read
void DroneParkController::decideSpot(Spot* spot, bool success, int stub_id)
{
	Stub* stub;

	//If the read was not successful, bail out
	if (!success)
	{
		goto exit;
	}

	//Query the stub from the database
	databaseController->queryStub(stub_id, &stub);

	//If we couldn't find the stub, just bail
	if (stub == NULL)
	{
		goto exit;
	}

	// If stub is passed expiry!!
	if (*(stub->getExpireTime()) < QDateTime::currentDateTime())
	{
		spot->setIllegal(true);
	}

exit:
	return;
}

void DroneParkController::createLot(int numspot, int rows, int col)
{
	//THIS IS A DUMMY FUNCTION
	QMessageBox msgBox;
	msgBox.setText("createLot has been fired");
	msgBox.exec();
}

void DroneParkController::newLotDialogOpen(NewLot* LotDialog)
{
	connect(LotDialog, SIGNAL(newLotOkSig(int numSpots, int rows, int col)), this, SLOT(createLot(int numspot, int rows, int col)));
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

//Start a sweep of the supplied Lot immediately. Starts the member controllers to perform the sweep.
int SweepController::initiateSweep(Lot* lot)
{

	//A controller object which handles the actual flight of the drone.
	dronePilot = new FlightController();

	//Interface for communicating between python and this sweepController
	ControlInterface* contInt = new ControlInterface();

	//Used this for debugging
	QMetaObject::Connection con;

	//Not sure what we should do if a thread is already running, for now just goto exit
	
	if (pilotWorkerThread.isRunning())
	{
		//Below is for debugging
		//emit fireSweep(this);

		goto exit;
	}
	

	//Throw the drone pilot worker on a new thread
	dronePilot->moveToThread(&pilotWorkerThread);

	//Connect all signals and slots for the drone to operate-----------------------------------

	//Connect the fireSweep signal of this controller to the workers start flight
	con = QObject::connect(this, &SweepController::fireSweep, dronePilot, &FlightController::asyncStartFlight);

	//Connect done signal THIS IS A DUMMY FOR DEBUGGING PURPOSES
	con = QObject::connect(dronePilot, SIGNAL(resultReady()), this, SLOT(handleResults()));

	//Connect interface to sweepController slots so python can communicate with us
	con = QObject::connect(contInt, SIGNAL(sigTest()), this, SLOT(handleResults()));

	con = QObject::connect(contInt, SIGNAL(atNextSpot()), this, SLOT(advanceSpot()));

	con = QObject::connect(contInt, SIGNAL(connFail()), this, SLOT(connectionFail()));

	//End drone signals------------------------------------------------------------------------

	//Start the new thread
	pilotWorkerThread.start();

	/*
	//I think we need these, idk
	Py_Initialize();
	PyEval_InitThreads();
	*/

	//Emit the fireSweep to start the async flight task
	emit fireSweep(contInt);

exit:
	return RC_OK;
}

//TODO: Nick: implement initializeDrone
//Initializes the controllers and the connection to the camera and the drone
int SweepController::initializeDrone()
{
	return RC_ERR;
}

//TODO: Nick: implement advanceSpot
//Change currentSpot to the next spot to be examined.
void SweepController::advanceSpot()
{

	//Move spot iterator to next spot
	if (spot_iterator != lot->getSpots()->end())
	{
		spot_iterator++;
	}

	//THIS IS A DUMMY FUNCTION
	QMessageBox msgBox;
	msgBox.setText("advanceSpot has been fired");
	msgBox.exec();

	return;
}

void SweepController::connectionFail()
{
	//THIS IS A DUMMY FUNCTION
	//QMessageBox msgBox;
	//msgBox.setText("connectionFail has been fired");
	//msgBox.exec();

	receiveCode("1");

	return;
}

//TODO: Nick: implement updateSpot
//Update spot based of the return value sent from the DecideSpotController
int SweepController::updateSpot(bool decision)
{
	return RC_ERR;
}

void SweepController::handleResults()
{
	//THIS IS A DUMMY FUNCTION
	QMessageBox msgBox;
	msgBox.setText("Back from the thread!");
	msgBox.exec();
	return;
}

//Slot to recieve the QR code, and construct a new signal to pass through to DroneParkController
void SweepController::receiveCode(QString _stub_id)
{
	emit decideSpotPass(*spot_iterator, true, _stub_id.toInt());
	return;
}

SweepController::SweepController(Lot* _lot)
{
	lot = _lot;

	spot_iterator = lot->getSpots()->begin();

	//A controller object which handles all communications with the physical drone
	//droneComms = new FlightCommsController();



	//A controller object which handles all communications with the physical camera.
	//imageComms = new ImageCommsController();

	//A controller object which handles all the image analysis.
	//imageProcessor = new ImageProcessController();

	//A controller object which handles the decision of validity of the spot.
	//stubDecider = new DecideSpotController();
}

SweepController::~SweepController()
{
	/*

//	if (droneComms != NULL)
	{
	//	delete droneComms;
	}

	if (dronePilot != NULL)
	{

	}

	if (//imageComms != NULL)
	{
//		delete imageComms;
	}

	if (imageProcessor != NULL)
	{
		delete imageProcessor;
	}

	if (stubDecider != NULL)
	{
		delete stubDecider;
	}
	*/
}

