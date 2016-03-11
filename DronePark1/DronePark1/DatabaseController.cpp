#include "DatabaseController.h"
#include "Tests_Misc.h"
#include <QMessageBox>
#include <string>
#include <QListWidgetItem>


#define UID "root"
#define PWD "123456"
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

		//tester.generateDB(db);
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

// Inserts lot into db, returns rc
int DatabaseController::insertLot(int _numspot,int _row,int _col,QString _lotname,QString _city)
{
	int rc = RC_OK;

	//store parameters into temp variables
	int lot_id;
	char create_date[50];
	int num_spots = _numspot;
	int row = _row;
	int col = _col;

	//for lot name and city QString must be converted to a char array
	char *lot_name; 
	QByteArray bl = _lotname.toLatin1();
	lot_name = bl.data();
	char *city; 
	QByteArray bc = _city.toLatin1();
	city = bc.data();

	

	try {

		//Create the stream object for Lot query
		otl_stream j(1, // buffer size
			"insert INTO Lot (num_spots, row, col, lot_name, city) VALUES (:num_spots<int>, :row<int>, :col<int>, :lot_name<char[20]>, :city<char[20]>)",
			// insert statement
			*db // connect object
			);

		//execute insert statement
		j << num_spots << row << col << lot_name << city;
		
	}
	catch (otl_exception& p) // intercept OTL exceptions
	{
		rc = RC_ERR;
		goto exit;
	}

	
exit:
	return rc;
}

int DatabaseController::insertConfig(int lot_id, int schedule_id)
{

	int rc = RC_OK;

	try {

		//Create the stream object for Lot query
		otl_stream j(1, // buffer size
			"insert INTO config (current_lot, current_schedule) VALUES (:lot_id<int>, :schedule_id<int>)",
			// insert statement
			*db // connect object
			);

		//execute insert statement
		j << lot_id << schedule_id;

	}
	catch (otl_exception& p) // intercept OTL exceptions
	{
		rc = RC_ERR;
		goto exit;
	}

exit:
	return rc;
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

//Query a list of configs and their lots (does not get spots!)
//Used in load config/lot
int DatabaseController::queryConfigs(std::list<Config*>** newConfigs)
{
	int rc = RC_OK;

	std::list<Config*>* configs;

	Lot* tempLot;
	Config* tempConfig;

	int config_id;
	int lot_id;
	int current_schedule;

	try
	{
		//Create the stream object for Config query
		otl_stream k(50, // buffer size
			"select * from config",
			// SELECT statement
			*db // connect object
			);

		//Write variables into query
		//otl_write_row(k);

		configs = new std::list<Config*>;

		//Loop through results
		for (auto& it : k) {

			otl_read_row(it, config_id, lot_id, current_schedule);

			// Query the Lot object next
			rc = queryLot(lot_id, &tempLot);
			DP_ASSERT(rc, "queryLot");

			tempConfig = new Config(config_id);
			tempConfig->setCurrentLot(tempLot);


			configs->push_front(tempConfig);
		}
	}
	catch (otl_exception& p) // intercept OTL exceptions
	{
		rc = RC_ERR;
		goto exit;
	}

	*newConfigs = configs;

exit:
	return rc;
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

	Config* newConfig;
	Lot* newLot;
	std::list<Spot*>* newSpots;

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

		// Query the Lot object next
		rc = queryLot(config_lot, &newLot);
		DP_ASSERT(rc, "queryLot");

		// Query the spots for the Lot
		rc = querySpots(newLot->getId(), &newSpots);
		DP_ASSERT(rc, "querySpots");

		//Set up Config structure with our newly queried data!
		newLot->setSpots(newSpots);
		newConfig->setCurrentLot(newLot);

		//TODO: Query stub info

	}
	catch (otl_exception& p) // intercept OTL exceptions
	{
		rc = RC_ERR;
		goto exit;
	}

	*config = newConfig;

exit:
	return rc;
}

// Queries db for Lot from id
int DatabaseController::queryLot(int id, Lot** lot)
{

	int rc = RC_OK;

	//Lot temp variables
	int lot_id;
	char create_date[50];
	int num_spots;
	int row;
	int col;
	char lot_name[20];
	char city[20];

	Lot* newLot;

	try {

		//Create the stream object for Lot query
		otl_stream j(1, // buffer size
			"select * from Lot where lot_id=:lot_id<int>",
			// SELECT statement
			*db // connect object
			);

		//Write variables into query
		otl_write_row(j, id);

		//Loop through results
		for (auto& it : j) {
			otl_read_row(it, lot_id, num_spots, row, col, lot_name, city);
		}
	}
	catch (otl_exception& p) // intercept OTL exceptions
	{
		rc = RC_ERR;
		goto exit;
	}

	// We have enough data to construct the Lot object
	newLot = new Lot(row, col, lot_name, city);
	newLot->setId(lot_id);

	*lot = newLot;

exit:
	return rc;
}

//TODO: Nick: Implement querySchedule
// Queries db for Schedule from id
Schedule* DatabaseController::querySchedule(int id)
{
	return new Schedule();
}

// Queries db for Stub from id
int DatabaseController::queryStub(int id, Stub** stub)
{
	// Declarations
	int rc = RC_OK;

	Stub* newStub;

	QDateTime* purchaseDateTime;
	QDateTime* expireDateTime;

	TIMESTAMP_STRUCT purchase_date;
	TIMESTAMP_STRUCT expire_date;

	try {

		//Create the stream object for Lot query
		otl_stream j(1, // buffer size
			"select purchase_date, expire_date from Stub where stub_id=:stub_id<int>",
			// SELECT statement
			*db // connect object
			);

		//Write variables into query
		otl_write_row(j, id);

		//Loop through results
		for (auto& it : j) {
			otl_read_row(it, purchase_date, expire_date);
		}
	}
	catch (otl_exception& p) // intercept OTL exceptions
	{
		rc = RC_ERR;
		goto error_exit;
	}

	// Construct the struct object with the data we got from the query
	newStub = new Stub();

	purchaseDateTime = new QDateTime(QDate(purchase_date.year, purchase_date.month, purchase_date.day), QTime(purchase_date.hour, purchase_date.minute, purchase_date.second));
	expireDateTime = new QDateTime(QDate(expire_date.year, expire_date.month, expire_date.day), QTime(expire_date.hour, expire_date.minute, expire_date.second));

	rc |= newStub->setPurchaseTime(purchaseDateTime);
	rc |= newStub->setExpireTime(expireDateTime);

	*stub = newStub;

exit:
	return rc;

error_exit:
	*stub = NULL;
	goto exit;
}

// Queries db for spots list from lot_id
int DatabaseController::querySpots(int _lot_id, std::list<Spot*>** spots)
{
	int rc = RC_OK;

	//Spot temp variables
	int spot_id;
	int stub_id;
	int lot_id;
	int is_empty;
	int is_illegal;
	int is_ticketed;
	int state;

	std::list<Spot*>* newSpots;

	try 
	{
		//Create the stream object for Spot query
		otl_stream k(50, // buffer size
			"select * from Spot where lot_id=:lot_id<int>",
			// SELECT statement
			*db // connect object
			);

		//Write variables into query
		otl_write_row(k, _lot_id);

		newSpots = new std::list<Spot*>;

		//Loop through results
		for (auto& it : k) {
			otl_read_row(it, spot_id, lot_id, stub_id, is_empty, is_illegal, is_ticketed, state);
			newSpots->push_front(new Spot(spot_id, is_empty, is_ticketed, is_illegal, stub_id));
		}
	}
	catch (otl_exception& p) // intercept OTL exceptions
	{
		rc = RC_ERR;
		goto exit;
	}

	*spots = newSpots;

exit:
	return rc;
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

int DatabaseController::queryLastLotId(int* lot_id)
{

	int rc = RC_OK;

	int newLotLocal = -1;

	try
	{
		//Create the stream object for Config query
		otl_stream k(50, // buffer size
			"SELECT LAST_INSERT_ID() FROM lot",
			// SELECT statement
			*db // connect object
			);

		//Loop through results
		for (auto& it : k) {

			otl_read_row(it, newLotLocal);

		}
	}
	catch (otl_exception& p) // intercept OTL exceptions
	{
		rc = RC_ERR;
		goto exit;
	}

	*lot_id = newLotLocal;
	
exit:
	return rc;

}

int DatabaseController::queryLastConfigId(int* config_id)
{

	int rc = RC_OK;

	int newConfigLocal = -1;

	try
	{
		//Create the stream object for Config query
		otl_stream k(50, // buffer size
			"SELECT LAST_INSERT_ID() FROM config",
			// SELECT statement
			*db // connect object
			);

		//Loop through results
		for (auto& it : k) {

			otl_read_row(it, newConfigLocal);

		}
	}
	catch (otl_exception& p) // intercept OTL exceptions
	{
		rc = RC_ERR;
		goto exit;
	}

	*config_id = newConfigLocal;

exit:
	return rc;

}

//Inserts num_spots new blank spots into lot with lot_id
int DatabaseController::insertNewSpots(int lot_id, int num_spots)
{
	int rc = RC_OK;

	int index = 0;

	while (index < num_spots)
	{
		try
		{
			//Create the stream object for Spot query
			otl_stream k(50, // buffer size
				"INSERT INTO `DRONEPARK`.`Spot`"
				"(`lot_id`,"
				"`stub_id`,"
				"`is_empty`,"
				"`is_illegal`,"
				"`is_ticketed`,"
				"`state`)"
				"VALUES"
				"(:lot_id<int>, NULL, 0, 0, 0, NULL)",
				// SELECT statement
				*db // connect object
				);

			//Write variables into query
			otl_write_row(k, lot_id);
		}
		catch (otl_exception& p) // intercept OTL exceptions
		{
			rc = RC_ERR;
			goto exit;
		}

		index++;
	}

exit:
	return rc;
}

//TODO: updateSpotTicketed has no error handling at all
//Slot for updating a spot object
void DatabaseController::updateSpotTicketed(int id, bool ticketed) 
{
	//Declarations
	int rc = RC_OK;
	int _ticketed = ticketed; //Convert to int for the db

	try {
		otl_stream o(1, // buffer size should be == 1 always on UPDATE
			"UPDATE Spot "
			"   SET is_ticketed=:is_ticketed<int> "
			" WHERE spot_id=:spot_id<int>",
			// UPDATE statement
			*db // connect object
			);

		//Write variables into query
		otl_write_row(o, _ticketed, id);
	}
	catch (otl_exception& p) // intercept OTL exceptions
	{
		rc = RC_ERR;
		goto exit;
	}

exit:
	return;
	//return rc;

}

//TODO: Implement updateSpotIllegal
//Slot for updating a spot object
void DatabaseController::updateSpotIllegal(int id, bool illegal)
{
	//Leaving this empty for now intentionally, easier to develop when database isn't constantly changing
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
