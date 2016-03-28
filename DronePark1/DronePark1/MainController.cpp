#include "MainController.h"
#include "dronepark1.h"
#include "QMessageBox.h"
//#include "Python.h"
//#include "PythonQt.h"
#include "qdebug.h"
#include "qmutex.h"
#include "newlot.h"


#define DEFAULT_CONFIG 1


//Initialize all controllers and models needed to start and display the first
//screen.
int DroneParkController::initialize(DronePark1* _gui)
{
	//Declarations--------------

	int rc = RC_OK;			//Return code

	//End Delcarations----------

	gui = _gui;

	// Allocate and set databaseController, deallocated in destructor hopefully
	databaseController = new DatabaseController();

	// Connect to the database (This doesn't really need an argument, it does nothing)
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
	QObject::connect(sweepController, SIGNAL(flyingChanged(bool)), gui->returnUI().flightStatus, SLOT(updateStatus(bool)));
	QObject::connect(sweepController, SIGNAL(updateLiveViewChain(QImage*)), this, SLOT(updateLiveView(QImage*)));
	QObject::connect(sweepController, SIGNAL(stopImage()), this, SLOT(clearLiveView()));

	//Tell the gui that we've finished a sweep
	QObject::connect(sweepController, SIGNAL(flightSuccess(int,int,int)), gui, SLOT(flightSuccessSlot(int,int,int)));

	//Advances the spot with 'n' key, for testing
	connect(gui, SIGNAL(enterPressed()), this, SLOT(enterPressed()));

	//Emergency Stop button
	QObject::connect(gui->returnUI().emergencyStopButton, SIGNAL(clicked()),
		sweepController, SLOT(emergencyShutDown()));

	//toggle schedule button
	QObject::connect(gui->returnUI().toggleScheduleButton, SIGNAL(clicked()),
		this, SLOT(toggleUseScheduleButtonSlot()));

	//Set up slots for loading additional gui windows
	//Load Config
	connect(gui->loadConfigAct, SIGNAL(triggered()), this, SLOT(getConfigs()));
	connect(gui, SIGNAL(acceptConfigPass(int)), this, SLOT(loadNewConfig(int)));
	connect(this, SIGNAL(loadConfigWindow(std::list<Config*>*)), gui, SLOT(loadConfigSlot(std::list<Config*>*)));

	//New Schedule
	connect(gui->newScheduleAct, SIGNAL(triggered()), this, SLOT(getSchedule()));
	connect(this, SIGNAL(loadNewSchedWindow(Schedule*)), gui, SLOT(loadSchedSlot(Schedule*)));
	connect(gui, SIGNAL(acceptSchedPass(QTime*, QTime*, int)), this, SLOT(loadNewSched(QTime*, QTime*, int)));

	//New Lot
	//radio button group for graphs
	//QObject::connect(gui->returnUI().illSpotButton, SIGNAL(pressed()), this, SLOT(DronePark1::updateGraphSlot()));
	//QObject::connect(gui->returnUI().validSpotButton, SIGNAL(pressed()), this, SLOT(DronePark1::updateGraphSlot()));
	//QObject::connect(gui->returnUI().emptySpotButton, SIGNAL(pressed()), this, SLOT(DronePark1::updateGraphSlot()));

	connect(gui, SIGNAL(newLotOpen(NewLot*)), this, SLOT(newLotDialogOpen(NewLot*)));

	//Last Minute UI Tweaks
	gui->returnUI().schedLabel->setText(currentConfig->getCurrentSchedule()->getStartTime()->toString("hh:mm") + " to " + currentConfig->getCurrentSchedule()->getEndTime()->toString("hh:mm"));

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
		QObject::connect(sweepController, SIGNAL(updateLiveViewChain(QImage*)), this, SLOT(updateLiveView(QImage*)));
		QObject::connect(sweepController, SIGNAL(stopImage()), this, SLOT(clearLiveView()));
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

	QString time1;
	QString time2;

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

	//qDebug() << *(stub->getExpireTime());
	//qDebug() << QDateTime::currentDateTime();
	//qDebug << QDateTime::QDateTime::currentDateTime().toString("hh:mm:ss");;

	// if there has been a read, there must be a car there
	spot->setEmpty(1); // yes, 1 means occupied

	// If stub is passed expiry!!
	if (*(stub->getExpireTime()) < QDateTime::currentDateTime())
	{
		spot->setIllegal(true);
	}
	else
	{
		spot->setIllegal(false);
	}



exit:
	return;
}

void DroneParkController::enterPressed()
{
	//qDebug() << "next spot button pressed";

	if (sweepController != NULL && sweepController->getFLYING())
	{
		sweepController->advanceSpot();
	}
}

// Queries db for configs and associated lots and emits them to loadConfig gui
void DroneParkController::getConfigs()
{
	std::list<Config*>* configs;

	//Bail if we're flying right now
	if (sweepController && sweepController->getFLYING())
	{
		int ret = QMessageBox::information(NULL, tr("DronePark"),
			tr("Cannot open new configuration.\n"
				"Please finish or cancel the current sweep first."));
		return;
	}
	
	//Grab the list of configs
	databaseController->queryConfigs(&configs);

	//Tells gui to load the new window
	emit loadConfigWindow(configs);

	return;
}

void DroneParkController::getSchedule()
{
	emit loadNewSchedWindow(currentConfig->getCurrentSchedule());
	return;
}

void DroneParkController::loadNewConfig(int id)
{
	int rc = RC_OK;

	qDebug() << "new config:" + QString::number(id);

	if (currentConfig->getId() == id)
	{
		//We already have this config loaded, just bail
		return;
	}

	//We have to nuke the old spots
	for (std::list<Spot*>::const_iterator iterator = currentConfig->getCurrentLot()->getSpots()->begin(),
		end = currentConfig->getCurrentLot()->getSpots()->end();
		iterator != end;
		++iterator)
	{
		delete *iterator;
	}

	//Then detele the lot and config
	delete currentConfig->getCurrentLot();
	delete currentConfig;

	//Load the default config
	rc |= loadConfig(id);
	DP_ASSERT(rc, "loadConfig");

	//Start populating the gui with the lot
	gui->replaceLotGui(currentConfig->getCurrentLot());

	//Nuke the histroy table
	gui->returnUI().historyTable->clearContents();
	while (gui->returnUI().historyTable->rowCount() > 0)
	{
		gui->returnUI().historyTable->removeRow(0);
	}

	gui->returnUI().lastSweep->setText("Never");

	//Nuke the scheduler info and turn it off
	currentConfig->setUseSchedule(false);
	gui->returnUI().toggleScheduleButton->setText("Enable Schedule");
	gui->returnUI().scheduleStatus->setText("DISABLED");
	gui->returnUI().scheduleStatus->setStyleSheet("QLabel { background-color : rgb(53, 53, 53); color : white; }");
	gui->clearSchedule();
	gui->returnUI().schedLabel->setText(currentConfig->getCurrentSchedule()->getStartTime()->toString("hh:mm") + " to " + currentConfig->getCurrentSchedule()->getEndTime()->toString("hh:mm"));

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

	//Connect sweepController
	if (sweepController == NULL)
	{
		sweepController = new SweepController(currentConfig->getCurrentLot());
		QObject::connect(sweepController, SIGNAL(updateLiveViewChain(QImage*)), this, SLOT(updateLiveView(QImage*)));
		QObject::connect(sweepController, SIGNAL(stopImage()), this, SLOT(clearLiveView()));
	}
	else
	{
		sweepController->setNewLot(currentConfig->getCurrentLot());
	}

	return;
}

//Load a new schedule
void DroneParkController::loadNewSched(QTime* startTime, QTime* endTime, int interval)
{

	//Start by nuking the old times
	if (currentConfig->getCurrentSchedule()->getStartTime() != NULL)
	{
		delete currentConfig->getCurrentSchedule()->getStartTime();
	}
	if (currentConfig->getCurrentSchedule()->getEndTime() != NULL)
	{
		delete currentConfig->getCurrentSchedule()->getEndTime();
	}

	currentConfig->getCurrentSchedule()->setStartTime(startTime);
	currentConfig->getCurrentSchedule()->setEndTime(endTime);
	currentConfig->getCurrentSchedule()->setInterval(interval);

	currentConfig->setUseSchedule(false);
	gui->returnUI().toggleScheduleButton->setText("Enable Schedule");
	gui->returnUI().scheduleStatus->setText("DISABLED");
	gui->returnUI().scheduleStatus->setStyleSheet("QLabel { background-color : rgb(53, 53, 53); color : white; }");
	gui->clearSchedule();
	gui->returnUI().schedLabel->setText(currentConfig->getCurrentSchedule()->getStartTime()->toString("hh:mm") + " to " + currentConfig->getCurrentSchedule()->getEndTime()->toString("hh:mm"));

	//idk what to do about this, it crashes sometimes. No time left to fix it
	/*
	if (schedulerTimer != NULL)
	{
		schedulerTimer->stop();
	}
	*/

	return;
}

//function is called when the newLotOkSig is emited from newlot
//assignes the values from the ui to a new lot to be inserted into the db 
void DroneParkController::createLot(int _numspot, int _rows, int _col, QString _lotName, QString _city)
{
	//instatiate a Lot object to load the config from 
	Lot* newLot;

	//assign parameters to temp values to pass to the db controller
	int numspot = _numspot;
	int rows = _rows;
	int col = _col;
	QString lotName = _lotName;
	QString city = _city;

	int lot_id = -1;
	int config_id = -1;
	
	//insert lot into db
	databaseController->insertLot(numspot, rows, col, lotName, city);
	databaseController->queryLastLotId(&lot_id);
	databaseController->insertNewSpots(lot_id, numspot);

	//TODO: INSERT NEW LOT HAS A DUMMY SCHEDULE ID
	//SCHEDULE ID HAS A DUMMY
	databaseController->insertConfig(lot_id, 1);
	databaseController->queryLastConfigId(&config_id);

	loadNewConfig(config_id);

	//Notification that the Lot has been added to the db
	QMessageBox msgBox;
	msgBox.setText("The new Lot has been added to the DataBase");
	msgBox.exec();
}

//function to call the createLot slot when the newLotOkSig is emitted
void DroneParkController::newLotDialogOpen(NewLot* LotDialog)
{
	//connects newLotOkSig to createLot
	 connect(LotDialog, SIGNAL(newLotOkSig(int, int, int, QString, QString )), this, SLOT(createLot(int , int , int, QString, QString )));
}

//Stops all current operations and shuts down the physical drone.
void SweepController::emergencyShutDown()
{
	//Set last spot overhead to false, we done
	(*spot_iterator)->setOverhead(false);

	if (getFLYING())
	{
		emit stopImage();

		// break ImageCapture out of the capture loop
		*captureLoop = false;

		captureThread->exit();
		processorThread->exit();

		// these will get re-instantiated at the beginning of each flight
		cap = NULL;
		delete cap;

		proc = NULL;
		delete proc;
	}

	//Definitely should not be doing this, but not much of a choice right now!!
	setFLYING(false);

	return;
}

void DroneParkController::toggleUseScheduleButtonSlot()
{
	//Turn off scheduler mode
	if (currentConfig->getUseSchedule() == true)
	{
		currentConfig->setUseSchedule(false);
		gui->returnUI().toggleScheduleButton->setText("Enable Schedule");
		gui->returnUI().scheduleStatus->setText("DISABLED");
		gui->returnUI().scheduleStatus->setStyleSheet("QLabel { background-color : rgb(53, 53, 53); color : white; }");
		gui->clearSchedule();

		if (schedulerTimer != NULL)
		{
			delete schedulerTimer;
		}
		
	}
	else //Turn on scheduler mode
	{
		currentConfig->setUseSchedule(true);
		gui->returnUI().toggleScheduleButton->setText("Disable Schedule");
		gui->returnUI().scheduleStatus->setText("ENABLED");
		gui->returnUI().scheduleStatus->setStyleSheet("QLabel { background-color : red; color : white; }");
		gui->showSchedule(currentConfig->getCurrentSchedule());

		schedulerTimer = new QTimer();

		connect(schedulerTimer, SIGNAL(timeout()), this, SLOT(triggerSweep()));

		//We need to calculate when to set the timer to expire.

		QDateTime scheduledTime(QDate::currentDate(), *(currentConfig->getCurrentSchedule()->getStartTime()));

		QDateTime endTime(QDate::currentDate(), *(currentConfig->getCurrentSchedule()->getEndTime()));

		int interval = currentConfig->getCurrentSchedule()->getInterval();

		int timerValue = 0;

		while (scheduledTime < endTime)
		{

			scheduledTime = scheduledTime.addSecs(interval * 60);

			if (scheduledTime > QDateTime::currentDateTime())
			{
				//We found the next scheduled run!

				timerValue = scheduledTime.toMSecsSinceEpoch() - QDateTime::currentDateTime().toMSecsSinceEpoch();

				//That number is in milliseconds!
				schedulerTimer->start(timerValue);

				break;
			}
		}
		
	}
	return;
}

//Called when the timer that controls the scheduler runs down
void DroneParkController::triggerSweep()
{
	int rc = RC_OK;

	//Start the sweep!
	rc |= sweepController->initiateSweep(currentConfig->getCurrentLot());

	//Set up the next alarm
	QDateTime scheduledTime(QDate::currentDate(), *(currentConfig->getCurrentSchedule()->getStartTime()));

	QDateTime endTime(QDate::currentDate(), *(currentConfig->getCurrentSchedule()->getEndTime()));

	int interval = currentConfig->getCurrentSchedule()->getInterval();

	int timerValue = 0;

	while (scheduledTime < endTime)
	{

		scheduledTime = scheduledTime.addSecs(interval * 60);

		if (scheduledTime > QDateTime::currentDateTime())
		{
			//We found the next scheduled run!

			timerValue = scheduledTime.toMSecsSinceEpoch() - QDateTime::currentDateTime().toMSecsSinceEpoch();

			//That number is in milliseconds!
			schedulerTimer->start(timerValue);

			break;
		}
	}

	//Redraw the schedule
	gui->clearSchedule();
	gui->showSchedule(currentConfig->getCurrentSchedule());
	

	return;
}

void DroneParkController::updateLiveView(QImage* image)
{
	QTabWidget* tabs = gui->returnUI().tabWidget;
	QLabel* display = gui->returnUI().liveDisplay;

	if (tabs->currentIndex() == 2)
	{

		// resize if necessary
		// currently trying out scaledContents on the QLabel
		
		int dw = display->width();
		int dh = display->height();

		int iw = image->width();
		int ih = image->height();

		QImage scaled;

		if (iw > dw && ih > dh && iw / dw > ih / dh || //both width and high are bigger, ratio at height is bigger or
			iw > dw && ih <= dh || //only the width is bigger or
			iw < dw && ih < dh && dw / iw < dh / ih //both width and height is smaller, ratio at width is smaller
			)
			scaled = image->scaledToWidth(dw, Qt::TransformationMode::FastTransformation);
		else if (iw > dw && ih > dh && iw / dw <= ih / dh || //both width and high are bigger, ratio at width is bigger or
			ih > dh && iw <= dw || //only the height is bigger or
			iw < dw && ih < dh && dw / iw > dh / ih //both width and height is smaller, ratio at height is smaller
			)
			scaled = image->scaledToHeight(dh, Qt::TransformationMode::FastTransformation);
		else
			scaled = *image;
		display->setPixmap(QPixmap::fromImage(scaled));
		

		//display->setPixmap(QPixmap::fromImage(*image));
	}

	// Can't forget to delete since this is a different copy than what ImageProcessor is deleting
	delete image;
}

void DroneParkController::clearLiveView()
{
	gui->returnUI().liveDisplay->clear();
	gui->returnUI().liveDisplay->setText("No Video Available");
}

void SweepController::updateLiveView(QImage* image)
{
	// the if statement is just to make sure that after the live view
	// is cleared, no straggling signals set the image again
	// (otherwise that was happening)
	if (*captureLoop)
	{

		// Use a separate image pointer than the processor,
		// otherwise sometimes the processor will delete it before it
		// has a chance to get to the live view
		QImage* newImage = new QImage(*image);

		emit updateLiveViewChain(newImage);
	}
}


//Start a sweep of the supplied Lot immediately. Starts the member controllers to perform the sweep.
int SweepController::initiateSweep(Lot* lot)
{

	//As of now, this is totally useless. Drone cannot fly autonomously.
	/*


	//Used this for debugging
	QMetaObject::Connection con;
	*/

	//Interface for communicating between python and this sweepController
	ControlInterface* contInt;
	contInt = new ControlInterface();

	//If we're already flying, we should get out and let it keep flying (NOOP)
	if (FLYING)
	{
		//No error, user is just clicking like a mad man
		goto exit;
	}

	spot_iterator = lot->getSpots()->begin();
	empty = 0;
	occupied = 0;
	illegal = 0;


	/*
	//A controller object which handles the actual flight of the drone.
	dronePilot = new FlightController();

	contInt = new ControlInterface();

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
//	Py_Initialize();
//	PyEval_InitThreads();

	// IMAGE STUFF---------------------------------------------------------------

	*/

	QMutex* mutex = new QMutex();
	
	captureThread = new QThread();
	processorThread = new QThread();

	cap = new ImageCapture(mutex, captureLoop);
	proc = new ImageProcessor(mutex);

	connect(this, SIGNAL(fireSweep(ControlInterface*)), cap, SLOT(asyncCaptureStart()), Qt::QueuedConnection);
	connect(proc, SIGNAL(qrCodeReady(QString)), this, SLOT(receiveCode(QString)), Qt::QueuedConnection);
	connect(cap, SIGNAL(imageReady(QImage*)), proc, SLOT(handleImage(QImage*)), Qt::QueuedConnection);
	connect(cap, SIGNAL(imageReady(QImage*)), this, SLOT(updateLiveView(QImage*)), Qt::QueuedConnection);

	//Kill switches
	connect(this, SIGNAL(stopImage()), cap, SLOT(stopCapture()));
	connect(this, SIGNAL(stopImage()), proc, SLOT(stopProcess()));

	cap->moveToThread(captureThread);
	proc->moveToThread(processorThread);

	captureThread->start();
	processorThread->start();

	//IMAGE STUFF END------------------------------------------------------------

	//Emit the fireSweep to start the async flight task
	*captureLoop = true;
	emit fireSweep(contInt);

	//Assuming all is well, we should be flying!
	setFLYING(true);

	//Set first spot as overhead
	(*spot_iterator)->setOverhead(true);

exit:
	return RC_OK;
}



//Change currentSpot to the next spot to be examined.
void SweepController::advanceSpot()
{

	//Move spot iterator to next spot
	if (spot_iterator != --(lot->getSpots()->end()))
	{
		if (*spot_iterator != NULL)
		{

			if (read == true)
			{
				read = false;
				//carry on
			}
			else // mark the spot empty!
			{
				(*spot_iterator)->setIllegal(0);
				(*spot_iterator)->setEmpty(0);
			}

			if ((*spot_iterator)->getEmpty() == 0)
			{
				empty++;
			}
			else if((*spot_iterator)->getIllegal() == 1)
			{
				illegal++;
			}
			else
			{
				occupied++;
			}

			//Clear ticketed status
			(*spot_iterator)->setTicketed(false);

			(*spot_iterator)->setOverhead(false);
			spot_iterator++;
			(*spot_iterator)->setOverhead(true);
		}
	}
	else // We've hit all the spots!
	{
		if (read == true)
		{
			read = false;
			//carry on
		}
		else // mark the spot empty!
		{
			(*spot_iterator)->setIllegal(0);
			(*spot_iterator)->setEmpty(0);
		}

		//Count the last spot!!
		if ((*spot_iterator)->getEmpty() == 0)
		{
			empty++;
		}
		else if ((*spot_iterator)->getIllegal() == 1)
		{
			illegal++;
		}
		else
		{
			occupied++;
		}

		//Clear ticketed status
		(*spot_iterator)->setTicketed(false);

		//Set last spot overhead to false, we done
		(*spot_iterator)->setOverhead(false);

		emit stopImage();

		// This will break ImageCapture out of the capture loop
		*captureLoop = false;

		captureThread->exit();
		processorThread->exit();

		// these will get re-instantiated at the beginning of each flight
		cap = NULL;
		delete cap;

		proc = NULL;
		delete proc;

		//Definitely should not be doing this, but not much of a choice right now!!
		setFLYING(false);

		emit flightSuccess(empty, occupied, illegal);
	}

	//QMessageBox msgBox;
	//msgBox.setText("advanceSpot has been fired");
	//msgBox.exec();

	return;
}

void SweepController::connectionFail()
{
	//THIS IS A DUMMY FUNCTION
	//QMessageBox msgBox;
	//msgBox.setText("connectionFail has been fired");
	//msgBox.exec();

	return;
}


bool SweepController::getFLYING()
{
	return FLYING;
}

//This should probably not exist
void SweepController::setFLYING(bool _flying)
{
	if (FLYING != _flying)
	{
		emit flyingChanged(_flying);
		FLYING = _flying;
	}
}

//Slot to recieve the QR code, and construct a new signal to pass through to DroneParkController
void SweepController::receiveCode(QString _stub_id)
{
	//THIS IS A DUMMY FUNCTION

	if (_stub_id.length() < 1)
		return;

	qDebug() << QTime::currentTime().toString() << "I read:" << _stub_id;

	emit decideSpotPass(*spot_iterator, true, _stub_id.toInt());

	//We got a read!
	read = true;

	return;
}

SweepController::SweepController(Lot* _lot)
{
	lot = _lot;

	spot_iterator = lot->getSpots()->begin();

	FLYING = false;

	// Keep this in the heap so ImageCapture and DroneParkController can both access it
	captureLoop = new bool(false);

	//A controller object which handles all communications with the physical drone
	//droneComms = new FlightCommsController();



	//A controller object which handles all communications with the physical camera.
	//imageComms = new ImageCommsController();

	//A controller object which handles all the image analysis.
	//imageProcessor = new ImageProcessController();

	//A controller object which handles the decision of validity of the spot.
	//stubDecider = new DecideSpotController();
}

void SweepController::setNewLot(Lot* _lot)
{
	lot = _lot;

	spot_iterator = lot->getSpots()->begin();

	FLYING = false;

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


