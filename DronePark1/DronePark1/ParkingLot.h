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

	// Boolean flag set if vehicle is parked illegaly
	bool illegal;

public:
	// Constructor, should call DbObject constructor
	Spot();
	Spot(int, int, int, int);

	// Returns the ticketed flag
	bool getTicketed();

	// Returns the empty flag
	bool getEmpty();

	// Returns the illegal flag
	bool getIllegal();

	// Sets the Ticketed flag, returns RC
	int setTicketed(bool _ticketed);

	// Sets the Empty flag, returns RC
	int setEmpty(bool _empty);

	// Sets the illegal flag
	int setIllegal(bool _illegal);

signals:
	void spotTicketedChanged(int id, bool newValue);
	void spotEmptyChanged(int id, bool newValue);
	void spotIllegalChanged(int id, bool newValue);

};

class Lot : public DbObject
{
private:
	// List of spots held in the lot, searlized, organization will be determined by column and row count
	std::list<Spot*>* spots;

	//Column and row count
	int col;
	int row;

	QString name;
	QString city;

public:

	//Default constructor and destructor
	Lot();
	~Lot();

	// Constructor, should call DbObject constructor
	Lot(int, int, QString _name, QString _city);

	// Returns the Spot list
	std::list<Spot*>* getSpots();

	// Sets the spot list, returns RC
	int setSpots(std::list<Spot*>* _spots);

	//getters
	int getCol();
	int getRow();
	QString getName();
	QString getCity();
};

