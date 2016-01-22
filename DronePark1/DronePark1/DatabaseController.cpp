#include "DatabaseController.h"

//TODO: Nick: DatabaseController Constructor maybe should do something?
// Constructor, should call Observer constructor
DatabaseController::DatabaseController()
{

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
Config DatabaseController::queryConfig(int id)
{
	return Config();
}

//TODO: Nick: Implement queryLot
// Queries db for Lot from id
Lot DatabaseController::queryLot(int id)
{
	return Lot();
}

//TODO: Nick: Implement querySchedule
// Queries db for Schedule from id
Schedule DatabaseController::querySchedule(int id)
{
	return Schedule();
}

//TODO: Nick: Implement queryStub
// Queries db for Stub from id
Stub DatabaseController::queryStub(int id)
{
	return Stub();
}

//TODO: Nick: Implement querySpot
// Queries db for Spot from id
Spot DatabaseController::querySpot(int id)
{
	return Spot();
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

//TODO: Nick: Implement DatabaseController::notify
// Does stuff
int DatabaseController::notify(Subject subject)
{
	return RC_ERR;
}

