#pragma once
#include "SubjectObserver.h"
#include "ParkingLot.h"
#include "Schedule.h"
#include "Config.h"
#include "ParkingStub.h"

class DatabaseController : Observer
{

public:
	// Constructor, should call Observer constructor
	DatabaseController();

	// Inserts lot into db, returns rc
	int insertLot(Lot newLot);

	// Inserts schedule into db, returns rc
	int insertSchedule(Schedule newSchedule);

	// Inserts stub into db, returns rc
	int insertStub(Stub newStub);

	// Queries db for config from id
	Config queryConfig(int id);

	//TODO: Nick: Implement queryLot
	// Queries db for Lot from id
	Lot queryLot(int id);

	// Queries db for Schedule from id
	Schedule querySchedule(int id);

	// Queries db for Stub from id
	Stub queryStub(int id);

	// Queries db for Spot from id
	Spot querySpot(int id);

	// Removes Lot from db by id, returns rc
	int removeLot(int id);

	// Removes Spot from db by id, returns rc
	int removeSpot(int id);

	// Removes Schedule from db by id, returns rc
	int removeSchedule(int id);

	//TODO: Nick: Implement removeStub
	// Removes Stub from db by id, returns rc
	int removeStub(int id);

	// Updates config from db by id, returns rc
	int updateConfig(Config newConfig, int id);

	// Updates Lot from db by id, returns rc
	int updateLot(Lot newLot, int id);

	// Updates Schedule from db by id, returns rc
	int updateSchedule(Schedule newScheudle, int id);

	// Updates Stub from db by id, returns rc
	int updateStub(Stub newStub, int id);

	// Updates Spot from db by id, returns rc
	int updateSpot(Spot newSpot, int id);

	int notify(Subject subject);

};