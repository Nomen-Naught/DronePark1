#pragma once
#include "ParkingLot.h"
#include "Config.h"

class Tester
{
public:

	//Generates a Lot with spots in it
	Lot* generateLot()
	{
		Lot* _lot = new Lot(3, 3, "test", "test");
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
				" (1,1), (2,2) ;"
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
				"  `row` int(11) DEFAULT NULL,"
				"  `col` int(11) DEFAULT NULL,"
				"  `lot_name` varchar(30) DEFAULT NULL,"
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
				"`row`,"
				"`col`,"
				"`lot_name`,"
				"`city`)"
				"VALUES"
				"( 6, 5, 4, 'ImPark', 'London'),"
				"( 10, 2, 5, 'Rogers Center', 'Toronto');"
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
				"  `is_illegal` int(11) NOT NULL DEFAULT '0' COMMENT 'No bool in MySQL, 0 means is_ticketed = false, 1 means is_ticketed = true',"
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
				"`is_illegal`,"
				"`is_ticketed`,"
				"`state`)"
				"VALUES"
				"(1, NULL, 1, 0, 0, NULL), (1, NULL, 0, 0, 0, NULL), (1, NULL, 0, 0, 0, NULL), (1, NULL, 0, 0, 0, NULL), (1, NULL, 0, 0, 0, NULL), (1, NULL, 1, 1, 0, NULL),"
				"(1, NULL, 1, 1, 0, NULL), (1, NULL, 0, 0, 0, NULL), (1, NULL, 1, 1, 0, NULL), (1, NULL, 0, 0, 0, NULL), (1, NULL, 1, 0, 0, NULL), (1, NULL, 1, 1, 0, NULL),"
				"(1, NULL, 1, 1, 0, NULL), (1, NULL, 0, 0, 0, NULL), (1, NULL, 1, 0, 0, NULL), (1, NULL, 0, 0, 0, NULL), (1, NULL, 1, 1, 0, NULL), (1, NULL, 1, 0, 0, NULL), (1, NULL, 0, 0, 0, NULL), (1, NULL, 1, 0, 0, NULL),"
				"(2, NULL, 1, 1, 0, NULL), (2, NULL, 0, 0, 0, NULL), (2, NULL, 1, 0, 0, NULL), (2, NULL, 0, 0, 0, NULL), (2, NULL, 1, 0, 0, NULL), (2, NULL, 1, 0, 1, NULL), (2, NULL, 1, 0, 0, NULL), (2, NULL, 0, 0, 0, NULL), (2, NULL, 1, 0, 0, NULL), (2, NULL, 1, 0, 0, NULL)"
				);  // insert some rows

		// Stub table
		otl_cursor::direct_exec
			(
				*db,
				"drop table Stub",
				otl_exception::disabled // disable OTL exceptions
				); // drop table

		otl_cursor::direct_exec
			(
				*db,
				"CREATE TABLE `Stub` ("
				"  `stub_id` int(11) NOT NULL AUTO_INCREMENT,"
				"  `spot_id` int(11) DEFAULT NULL,"
				"  `purchase_date` datetime DEFAULT NULL,"
				"  `expire_date` datetime DEFAULT NULL,"
				"  PRIMARY KEY (`stub_id`),"
				"  UNIQUE KEY `stub_id_UNIQUE` (`stub_id`)"
				") ENGINE=InnoDB DEFAULT CHARSET=latin1;"
				);  // create table

			otl_cursor::direct_exec
				(
					*db,
					"INSERT INTO `DRONEPARK`.`Stub`"
					"(stub_id,"
					"`spot_id`,"
					"`purchase_date`,"
					"`expire_date`)"
					"VALUES"
					"(1234, 1, '1000-01-01 00:00:00', '1000-01-01 00:00:01'), (1235, 2, '1000-01-01 00:00:00', '9999-12-31 23:59:59'), (1, 6, '1000-01-01 00:00:00','9999-12-31 23:59:59');"
					);  // insert some rows

			//Schedule table
			otl_cursor::direct_exec
				(
					*db,
					"drop table schedule",
					otl_exception::disabled // disable OTL exceptions
					); // drop table

			otl_cursor::direct_exec
				(
					*db,
					"CREATE TABLE `schedule` ("
					"  `schedule_id` int(11) NOT NULL AUTO_INCREMENT,"
					"  `lot_id` int(11) NOT NULL,"
					"  `start_time` datetime NOT NULL,"
					"  `end_time` datetime NOT NULL,"
					"  `interv` int(11) NOT NULL,"
					"  UNIQUE KEY `schedule_id_UNIQUE` (`schedule_id`)"
					") ENGINE=InnoDB DEFAULT CHARSET=utf8;"
					);  // create table

			otl_cursor::direct_exec
				(
					*db,
					"INSERT INTO `dronepark`.`schedule`"
					"(`lot_id`,"
					"`start_time`,"
					"`end_time`,"
					"`interv`)"
					"VALUES"
					"(1, '1000-01-01 09:00:00', '1000-01-01 22:00:00', 60), (2, '1000-01-01 09:00:00', '1000-01-01 14:00:00', 60);"
					);  // insert some rows

	}
};