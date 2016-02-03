#pragma once
#include "ParkingLot.h"
#include "Config.h"

class Tester
{
public:

	//Generates a Lot with spots in it
	Lot* generateLot()
	{
		Lot* _lot = new Lot();
		std::list<Spot*>* spots = new std::list<Spot*>;

		for (int i = 0; i < 5; i++)
		{
			spots->push_front(new Spot());
		}

		_lot->setSpots(spots);

		_lot->setId(-2);

		return _lot;
	}

	//Generates a Config ptr with a lot with spots in it
	void generateConfigPtr(Config** config)
	{
		Config* _config = new Config();

		_config->setCurrentLot(generateLot());

		_config->setId(-2);

		*config =  _config;
	}

	// WARNING, WIPES ALL DB DATA!!!
	// re-generates a schema and puts some data in it
	void generateDB(otl_connect* db)
	{
		//Config table
		otl_cursor::direct_exec
			(
				*db,
				"drop table Config",
				otl_exception::disabled // disable OTL exceptions
			); // drop table

		otl_cursor::direct_exec
			(
				*db,
				"CREATE TABLE `Config` ("
				"`config_id` int(11) NOT NULL AUTO_INCREMENT,"
				"`current_lot` int(11) DEFAULT NULL,"
				"`current_schedule` int(11) DEFAULT NULL,"
				"PRIMARY KEY (`config_id`),"
				"UNIQUE KEY `config_id_UNIQUE` (`config_id`)"
				") ENGINE=InnoDB DEFAULT CHARSET=latin1;"
			);  // create table

		otl_cursor::direct_exec
			(
				*db,
				"INSERT INTO `DRONEPARK`.`Config`"
				"(`current_lot`,"
				"`current_schedule`)"
				"VALUES"
				" (1,1), (2,2), (3,3) ;"
			);  // insert some rows

		//Lot Table
		otl_cursor::direct_exec
			(
				*db,
				"drop table Lot",
				otl_exception::disabled // disable OTL exceptions
				); // drop table

		otl_cursor::direct_exec
			(
				*db,
				"CREATE TABLE `Lot` ("
				"  `lot_id` int(11) NOT NULL AUTO_INCREMENT,"
				"  `num_spots` int(11) DEFAULT NULL,"
				"  `lot_name` varchar(20) DEFAULT NULL,"
				"  `city` varchar(20) DEFAULT NULL,"
				"  PRIMARY KEY (`lot_id`),"
				"  UNIQUE KEY `lot_id_UNIQUE` (`lot_id`)"
				") ENGINE=InnoDB DEFAULT CHARSET=latin1;"
				);  // create table

		otl_cursor::direct_exec
			(
				*db,
				"INSERT INTO `DRONEPARK`.`Lot`"
				"(`num_spots`,"
				"`lot_name`,"
				"`city`)"
				"VALUES"
				"( 6, 'ImPark', 'London'),"
				"( 10, 'Rogers Center', 'Toronto');"
				);  // insert some rows

		//Spot Table
		otl_cursor::direct_exec
			(
				*db,
				"drop table Spot",
				otl_exception::disabled // disable OTL exceptions
				); // drop table

		otl_cursor::direct_exec
			(
				*db,
				"CREATE TABLE `Spot` ("
				"  `spot_id` int(11) NOT NULL AUTO_INCREMENT,"
				"  `lot_id` int(11) NOT NULL,"
				"  `stub_id` int(11) DEFAULT NULL,"
				"  `is_empty` int(11) NOT NULL DEFAULT '0' COMMENT 'No bool in MySQL, 0 means is_ticketed = false, 1 means is_ticketed = true',"
				"  `is_ticketed` int(11) NOT NULL DEFAULT '0' COMMENT 'No bool in MySQL, 0 means is_ticketed = false, 1 means is_ticketed = true',"
				"  `state` int(11) DEFAULT NULL,"
				"  PRIMARY KEY (`spot_id`),"
				"  UNIQUE KEY `spot_id_UNIQUE` (`spot_id`)"
				") ENGINE=InnoDB DEFAULT CHARSET=latin1;"

				);  // create table

		otl_cursor::direct_exec
			(
				*db,
				"INSERT INTO `DRONEPARK`.`Spot`"
				"(`lot_id`,"
				"`stub_id`,"
				"`is_empty`,"
				"`is_ticketed`,"
				"`state`)"
				"VALUES"
				"(1, NULL, 1, 0, NULL), (1, NULL, 0, 0, NULL), (1, NULL, 1, 0, NULL), (1, NULL, 0, 0, NULL), (1, NULL, 1, 0, NULL), (1, NULL, 1, 0, NULL);"
				);  // insert some rows
	}
};