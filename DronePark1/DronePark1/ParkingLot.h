#pragma once
#include "SubjectObserver.h"
#include "ReturnCodes.h"
#include <list>

class Spot : DbObject
{
private:
	// Boolean flag set if spot is empty
	bool empty;

	// Boolean flag set if a vehicle has received a ticket
	bool ticketed;

public:
	// Constructor, should call DbObject constructor
	Spot() : DbObject()
	{
		empty = false;
		ticketed = false;
	}

	// Returns the ticketed flag
	bool getTicketed()
	{
		return ticketed;
	}

	// Returns the empty flag
	bool getEmpty()
	{
		return empty;
	}

	// Sets the Ticketed flag, returns RC
	int setTicketed(bool _ticketed)
	{
		ticketed = _ticketed;
		return RC_OK;
	}

	// Sets the Empty flag, returns RC
	int setEmpty(bool _empty)
	{
		empty = _empty;
		return RC_OK;
	}

};

class Lot : DbObject
{
private:
	// List of spots held in the lot
	std::list<Spot> spots;

public:

	//TODO: Nick: Lot Constructor maybe should do something?
	// Constructor, should call DbObject constructor
	Lot(): DbObject()
	{
		
	}

	// Returns the Spot list
	std::list<Spot> getSpots()
	{
		return spots;
	}

	// Sets the spot list, returns RC
	int setSpots(std::list<Spot> _spots)
	{
		spots = _spots;
		return RC_OK;
	}
};

