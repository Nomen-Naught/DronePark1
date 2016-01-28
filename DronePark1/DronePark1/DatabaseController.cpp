#include "DatabaseController.h"
#include "Tests_Misc.h"
#include <QMessageBox>


//TODO: Nick: DatabaseController Constructor maybe should do something?
// Constructor, should call Observer constructor
DatabaseController::DatabaseController()
{
	//Start with no db connected
	dbConnected = false;
}

// CURRENTLY A NO-OP PLACEHOLDER!!!!
// Connect to db using specified connectionString, repeated calls should be no-op/return error
int DatabaseController::connectToDb(QString connectionString)
{

	int rc = RC_OK;	// return code

	if (dbConnected)
	{
		rc = RC_2DB_CONN_ATTEPTS;
		goto exit;
	}

	//TODO: connect to databse

	//Placeholder!!
	dbConnected = true;


exit:
	return RC_OK;
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

//TODO: Nick: Implement queryConfig
// Queries db for config from id
int DatabaseController::queryConfig(int id, Config** config)
{
	//dummy function for now

	int rc = RC_OK;		// return code

	//TestObject
	Tester tester;
	//TestObject

	//Genreate a dummy config
	tester.generateConfigPtr(config);

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
