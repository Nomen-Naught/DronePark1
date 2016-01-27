#pragma once
#include "DBObject.h"
#include "ReturnCodes.h"
#include <list>
#include <QObject>

class Spot : public DbObject 
{
	Q_OBJECT
private:
	// Boolean flag set if spot is empty
	bool empty;

	// Boolean flag set if a vehicle has received a ticket
	bool ticketed;

public:
	// Constructor, should call DbObject constructor
	Spot();

	// Returns the ticketed flag
	bool getTicketed();

	// Returns the empty flag
	bool getEmpty();

	// Sets the Ticketed flag, returns RC
	int setTicketed(bool _ticketed);

	// Sets the Empty flag, returns RC
	int setEmpty(bool _empty);

signals:
	void spotTicketedChanged(int id, bool newValue);
	void spotEmptyChanged(int id, bool newValue);

};

class Lot : public DbObject
{
private:
	// List of spots held in the lot
	std::list<Spot*>* spots;

public:

	// Constructor, should call DbObject constructor
	Lot();

	// Returns the Spot list
	std::list<Spot*>* getSpots();

	// Sets the spot list, returns RC
	int setSpots(std::list<Spot*>* _spots);
};

