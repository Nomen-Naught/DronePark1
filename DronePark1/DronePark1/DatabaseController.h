#pragma once
#include "DBObject.h"
#include "ParkingLot.h"
#include "Schedule.h"
#include "Config.h"
#include "ParkingStub.h"
#include <QObject>

//Database stuff
#define OTL_ODBC_MYSQL // Compile OTL 4.0/ODBC/MySQL
#include <otlv4.h> // include the OTL 4.0 header file

class DatabaseController : public QObject
{
	Q_OBJECT

private:

	//Flag indicating whether we are connected to a db or not
	bool dbConnected;

	// connect object
	otl_connect* db;

public:
	// Constructor, should call Observer constructor
	DatabaseController();

	// Destructor
	~DatabaseController();

	// Connect to db using specified connectionString, repeated calls should be no-op/return error
	int connectToDb(QString connectionString);

	// Inserts lot into db, returns rc
	int insertLot(Lot newLot);

	// Inserts schedule into db, returns rc
	int insertSchedule(Schedule newSchedule);

	// Inserts stub into db, returns rc
	int insertStub(Stub newStub);

	// Queries db for config from id
	int queryConfig(int id, Config** config);

	//TODO: Nick: Implement queryLot
	// Queries db for Lot from id
	Lot* queryLot(int id);

	// Queries db for Schedule from id
	Schedule* querySchedule(int id);

	// Queries db for Stub from id
	Stub* queryStub(int id);

	// Queries db for Spot from id
	Spot* querySpot(int id);

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

public slots:
	void updateSpotTicketed(int id, bool ticketed);
	void updateSpotEmpty(int id, bool empty);

};