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
		std::list<Spot*> spots;

		for (int i = 0; i < 10; i++)
		{
			spots.push_front(new Spot());
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
};