#include "ParkingLot.h"

// Constructor, should call DbObject constructor
Spot::Spot() : DbObject()
{
	empty = false;
	ticketed = false;

	overhead = false;
}

Spot::Spot(int _spot_id, int _empty, int _is_ticketed, int _illegal, int _stub_id ) : DbObject()
{
	id = _spot_id;
	empty = _empty;
	ticketed = _is_ticketed;
	illegal = _illegal;
	stub_id = _stub_id;

	overhead = false;

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

// Returns the illegal flag
bool Spot::getIllegal()
{
	return illegal;
}

bool Spot::getNewIllegal()
{
	return newIllegal;
}

// Returns the stub id
int Spot::getStubId()
{
	return stub_id;
}


// Sets the Ticketed flag, returns RC
int Spot::setTicketed(bool _ticketed)
{

	if (ticketed != _ticketed)
	{
		ticketed = _ticketed;
		//Emit Qt signal to update listeners
		emit spotTicketedChanged(id, _ticketed);
	}

	return RC_OK;
}

int Spot::setNewIllegal(bool _newIllegal)
{
	if (newIllegal != _newIllegal)
	{
		newIllegal = _newIllegal;
	}

	return RC_OK;
}

// Sets the Empty flag, returns RC
int Spot::setEmpty(bool _empty)
{
	if (empty != _empty)
	{
		empty = _empty;
		//Emit Qt signal to update listeners
		emit spotEmptyChanged(id, _empty);

	}
	return RC_OK;
}

// Sets the Illegal flag, returns RC
int Spot::setIllegal(bool _illegal)
{
	if (illegal != _illegal)
	{
		illegal = _illegal;
		//Emit Qt signal to update listeners
		emit spotIllegalChanged(id, _illegal);

	}
	return RC_OK;
}

// Sets the stub_id, returns RC
int Spot::setStubId(int _stub_id)
{
	if (stub_id != _stub_id)
	{
		stub_id = _stub_id;
		//Emit Qt signal to update listeners
		emit spotStubChanged(id, _stub_id);
	}
	return RC_OK;
}

int Spot::setOverhead(bool _overhead)
{
	if (overhead != _overhead)
	{
		overhead = _overhead;
		emit overheadChanged(_overhead);
	}
	return RC_OK;
}


//Lot Constructor
Lot::Lot()
{

}

//Lot Destructor
Lot::~Lot()
{
	//de-allocate spots if we got it
	if (spots != NULL)
	{
		spots->clear();
		delete spots;
	}
}

// Constructor, should call DbObject constructor
Lot::Lot(int _row, int _col, QString _name, QString _city) : DbObject()
{
	row = _row;
	col = _col;
	name = _name;
	city = _city;
}


// Returns the Spot list
std::list<Spot*>* Lot::getSpots()
{
	return spots;
}

// Sets the spot list, returns RC
int Lot::setSpots(std::list<Spot*>* _spots)
{
	spots = _spots;
	return RC_OK;
}

// gets column count
int Lot::getCol()
{
	return col;
}

// gets row count
int Lot::getRow()
{
	return row;
}

// gets row count
QString Lot::getName()
{
	return name;
}

// gets row count
QString Lot::getCity()
{
	return city;
}

