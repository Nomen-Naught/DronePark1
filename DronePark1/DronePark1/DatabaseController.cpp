#include "DatabaseController.h"
#include "Tests_Misc.h"
#include <QMessageBox>
#include <string>

#define UID "nicholas"
#define PWD "nicholas"
#define DSN "DronePark_MySQL"


//TODO: Nick: DatabaseController Constructor maybe should do something?
// Constructor, should call Observer constructor
DatabaseController::DatabaseController()
{
	//Start with no db connected
	dbConnected = false;
}

// Destructor
DatabaseController::~DatabaseController()
{
	if (db != NULL)
	{
		delete db;
	}
}

// CURRENTLY A NO-OP PLACEHOLDER!!!!
// Connect to db using specified connectionString, repeated calls should be no-op/return error
int DatabaseController::connectToDb(QString connectionString)
{
	//Declarations
	int rc = RC_OK;	// return code

	char logon[50]; // logon connection string for ODBC

	//TestObject
	Tester tester;
	//TestObject

	// We should only have one connection at a time
	if (dbConnected)
	{
		rc = RC_2DB_CONN_ATTEMPTS;
		goto exit;
	}

	otl_connect::otl_initialize(); // initialize ODBC environment

	//Create a new db connect object
	db = new otl_connect();
	
	try {

		//Construct the connection string
		sprintf(logon, "UID=%s;PWD=%s;DSN=%s", UID, PWD, DSN);

		// I think this connect is not thread safe!!
		db->rlogon(logon); // connect to ODBC

		//TEST Generate a completely new schema and tables
		tester.generateDB(db);
	}
	catch (otl_exception& p) // intercept OTL exceptions
	{	
		rc = RC_DB_CONN_FAIL;
		dbConnected = false;
		goto exit;
	}

	dbConnected = true;

exit:
	return rc;
}

//TODO: Nick: Implement insertLot
// Inserts lot into db, returns rc
int DatabaseController::insertLot(Lot newLot)
{
	return RC_ERR;
}

//TODO: Nick: Implemet insertSchedule
// Inserts schedule into db, returns rc
int DatabaseController::insertSchedule(Schedule newSchedule)
{
	return RC_ERR;
}

//TODO: Nick: Implement insertStub
// Inserts stub into db, returns rc
int DatabaseController::insertStub(Stub newStub)
{
	return RC_ERR;
}

// Queries db for config from id
int DatabaseController::queryConfig(int id, Config** config)
{
	//Declarations

	int rc = RC_OK;		// return code

	//Config temp variables
	int config_id;
	int config_lot;
	int config_schedule;

	//Lot temp variables
	int lot_id;
	char create_date[50];
	int num_spots;
	char lot_name[20];
	char city[20];

	//Spot temp variables
	int spot_id;
	int stub_id;
	int is_empty;
	int is_ticketed;
	int state;

	Config* newConfig;
	Lot* newLot;
	Spot* newSpot;

	std::list<Spot*>* spots;

	// Query everything we need for the Config object
	try {

		//Create the stream object for Config query
		otl_stream i(1, // buffer size
			"select * from Config where config_id=:config_id<int>",
			// SELECT statement
			*db // connect object
			);

		//Write variables into query
		otl_write_row(i, id);

		//Loop through results
		for (auto& it : i) {
			otl_read_row(it, config_id, config_lot, config_schedule);
		}

		// We have enough data to construct the config object
		newConfig = new Config();
		newConfig->setId(config_id);

		//Create the stream object for Lot query
		otl_stream j(1, // buffer size
			"select * from Lot where lot_id=:lot_id<int>",
			// SELECT statement
			*db // connect object
			);

		//Write variables into query
		otl_write_row(j, config_lot);

		//Loop through results
		for (auto& it : j) {
			otl_read_row(it, lot_id, num_spots, lot_name, city);
		}

		// We have enough data to construct the Lot object
		newLot = new Lot();
		newLot->setId(lot_id);

		//Create the stream object for Spot query
		otl_stream k(1, // buffer size
			"select * from Spot where lot_id=:lot_id<int>",
			// SELECT statement
			*db // connect object
			);

		//Write variables into query
		otl_write_row(k, lot_id);

		spots = new std::list<Spot*>;

		//Loop through results
		for (auto& it : k) {
			otl_read_row(it, spot_id, lot_id, stub_id, is_empty, is_ticketed, state);
			spots->push_front(new Spot(spot_id, is_empty, is_ticketed));
		}

		//Set up Config structure with our newly queried data!
		newLot->setSpots(spots);
		newConfig->setCurrentLot(newLot);

		//TODO: Query stub info

	}
	catch (otl_exception& p) // intercept OTL exceptions
	{
		rc = RC_ERR;
		goto exit;
	}

	//Test stuff
	//newConfig->setCurrentLot(tester.generateLot());

	*config = newConfig;

exit:
	return rc;
}

//TODO: Nick: Implement queryLot
// Queries db for Lot from id
Lot* DatabaseController::queryLot(int id)
{
	return new Lot();
}

//TODO: Nick: Implement querySchedule
// Queries db for Schedule from id
Schedule* DatabaseController::querySchedule(int id)
{
	return new Schedule();
}

//TODO: Nick: Implement queryStub
// Queries db for Stub from id
Stub* DatabaseController::queryStub(int id)
{
	return new Stub();
}

//TODO: Nick: Implement querySpot
// Queries db for Spot from id
Spot* DatabaseController::querySpot(int id)
{
	return new Spot();
}

//TODO: Nick: Implement removeLot
// Removes Lot from db by id, returns rc
int DatabaseController::removeLot(int id)
{
	return RC_ERR;
}

//TODO: Nick: Implement removeSpot
// Removes Spot from db by id, returns rc
int DatabaseController::removeSpot(int id)
{
	return RC_ERR;
}

//TODO: Nick: Implement removeSchedule
// Removes Schedule from db by id, returns rc
int DatabaseController::removeSchedule(int id)
{
	return RC_ERR;
}

//TODO: Nick: Implement removeStub
// Removes Stub from db by id, returns rc
int DatabaseController::removeStub(int id)
{
	return RC_ERR;
}

//TODO: Nick: Implement updateConfig
// Updates config from db by id, returns rc
int DatabaseController::updateConfig(Config newConfig, int id)
{
	return RC_ERR;
}

//TODO: Nick: Implement updateLot
// Updates Lot from db by id, returns rc
int DatabaseController::updateLot(Lot newLot, int id)
{
	return RC_ERR;
}

//TODO: Nick: Implement updateScheudle
// Updates Schedule from db by id, returns rc
int DatabaseController::updateSchedule(Schedule newScheudle, int id)
{
	return RC_ERR;
}

//TODO: Nick: Implement updateStub
// Updates Stub from db by id, returns rc
int DatabaseController::updateStub(Stub newStub, int id)
{
	return RC_ERR;
}

//TODO: Nick: Implement updateSpot
// Updates Spot from db by id, returns rc
int DatabaseController::updateSpot(Spot newSpot, int id)
{
	return RC_ERR;
}

//TODO: Implement updateSpotTicketed
//Slot for updating a spot object
void DatabaseController::updateSpotTicketed(int id, bool ticketed) 
{
	//STUB
	//QMessageBox msgBox;
	//msgBox.setText("Ticketed has been updated");
	//msgBox.exec();
}

//TODO: Implement updateSpotEmpty
//Slot for updating a spot object
void DatabaseController::updateSpotEmpty(int id, bool empty)
{
	//STUB
	//QMessageBox msgBox;
	//msgBox.setText("Empty has been updated");
	//msgBox.exec();
}
