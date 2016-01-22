#include "ParkingLot.h"

// Constructor, should call DbObject constructor
Spot::Spot() : DbObject()
{
	empty = false;
	ticketed = false;
}

// Returns the ticketed flag
bool Spot::getTicketed()
{
	return ticketed;
}

// Returns the empty flag
bool Spot::getEmpty()
{
	return empty;
}

// Sets the Ticketed flag, returns RC
int Spot::setTicketed(bool _ticketed)
{
	ticketed = _ticketed;
	return RC_OK;
}

// Sets the Empty flag, returns RC
int Spot::setEmpty(bool _empty)
{
	empty = _empty;
	return RC_OK;
}

//TODO: Nick: Lot Constructor maybe should do something?
// Constructor, should call DbObject constructor
Lot::Lot() : DbObject()
{

}

// Returns the Spot list
std::list<Spot> Lot::getSpots()
{
	return spots;
}

// Sets the spot list, returns RC
int Lot::setSpots(std::list<Spot> _spots)
{
	spots = _spots;
	return RC_OK;
}