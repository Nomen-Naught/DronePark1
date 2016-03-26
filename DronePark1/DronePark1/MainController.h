#pragma once
#include "ParkingLot.h"
#include "Config.h"
#include "DecideController.h"
#include "DatabaseController.h"
#include "DronePilot.h"
#include "dronepark1.h"
#include <QObject>
#include "QMessageBox.h"
#include <QThread>
#include "ControlInterface.h"
#include "ImageCapture.h"
#include "ImageProcessor.h"
#include "qpixmap.h"
#include <QTimer>

class SweepController : public QObject
{
	Q_OBJECT
	QThread pilotWorkerThread;

private:

	//Current lot we're examining
	Lot* lot;

	//Indicates if we're flying or not!
	bool FLYING;

	//count of last sweeps stats
	int empty;
	int occupied;
	int illegal;

	//Iterator that points to the current spot
	std::list<Spot*>::const_iterator spot_iterator;

	//Thread objects
	QThread* captureThread;
	QThread* processorThread;

	ImageCapture* cap;
	ImageProcessor* proc;

	bool* captureLoop;

	//Update spot based of the return value sent from the DecideSpotController
	int updateSpot(bool decision);

public:

	//A controller object which handles the actual flight of the drone.
	FlightController* dronePilot;

	SweepController(Lot*);
	~SweepController();

	//Points the sweepController to a new lot to examine
	void setNewLot(Lot*);

	//Start a sweep of the supplied Lot immediately. Starts the member controllers to perform the sweep.
	int initiateSweep(Lot* lot);

	//Checks if we're currently flyin
	bool getFLYING();

	//Sets if we're flyin
	void setFLYING(bool _flying);

public slots:

	//Move to the next spot and start camera taking pictures again
	void advanceSpot();

	//The connection to the drone has failed
	void connectionFail();

	//Receive QR code reading
	void receiveCode(QString stub_id);

	//Stops all current operations and shuts down the physical drone.
	void emergencyShutDown();

	//Update the live view with the video feed
	void updateLiveView(QImage* image);


signals:
	//Should initiate a sweep
	void fireSweep(ControlInterface*);

	//A pass through signal that should alert DroneParkController who can do database queries
	void decideSpotPass(Spot* spot, bool success, int stub_id);

	//Flying status has changed
	void flyingChanged(bool);

	//Tell image processing to stop and exit
	void stopImage();

	//We managed to successfully iterate the entire lot, tell everybody! Celebrate!
	void flightSuccess(int empty, int occupied, int illegal);

	//Pass image to DroneParkController
	void updateLiveViewChain(QImage*);

};

class DroneParkController : public QObject
{
	Q_OBJECT

private:


	//controls the Drone and all processing associated with it.
	SweepController* sweepController;

	//Deviation from design, adding DatabaseController
	DatabaseController* databaseController;

	//Is keeping a reference to the gui a good idea? not sure lol
	DronePark1* gui;

	QTimer* schedulerTimer = NULL;

public:

	DroneParkController();
	~DroneParkController();

	//The currently loaded configuration for the session
	Config* currentConfig;

	//nitialize all controllers and models needed to start and display the first
	//screen.
	int initialize(DronePark1* gui);

	//Queries the database with an id and loads a new configuration.
	int loadConfig(int id);

public slots:
	// Slot for startSweepButton slot
	void startSweepButtonSlot();

	//Decide the spot by looking up the stub_id and comparing it to the current time.
	//Only do this if read was a success
	void decideSpot(Spot* spot, bool success, int stub_id);

	//Button has been rpessed
	void enterPressed();

	//Gets a list of configs for loadConfig and will fires them with loadConfigWindow at gui
	void getConfigs();

	//Gets the schedule for newSchedule and fires them at gui
	void getSchedule();

	//Load a new config with the id passed in
	void loadNewConfig(int id);

	//Load a new schedule
	void loadNewSched(QTime* , QTime* , int id);

	//Slot to receiver new lot 
	void createLot(int numspot, int rows, int col, QString lotName, QString _city);

	//Opens new lot dialog window
	void newLotDialogOpen(NewLot* LotDialog);

	//Slot for enable/disable schedule button
	void toggleUseScheduleButtonSlot();

	//Update the live view with the video feed
	void updateLiveView(QImage* image);

	// Reset the live view when the video feed is finished
	void clearLiveView();

	//Triggers a flight from the scheduler
	void triggerSweep();

signals:

	//For loading an open config window
	void loadConfigWindow(std::list<Config*>*);

	//For loading a new sched window
	void loadNewSchedWindow(Schedule*);

};

