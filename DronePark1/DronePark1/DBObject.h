#pragma once
#include <QObject>
#include "ReturnCodes.h"

class DbObject : public QObject
{
	Q_OBJECT
protected:


public:

	// Unique identifier, generated and set by Database!
	// -1 means a proper ID has not been set yet.
	int id;

	// Constructor
	DbObject()
	{
		id = -1;
	}

	// Returns the id value for the Spot object
	int getId()
	{
		return id;
	}

	// Sets the id, returns RC
	int setId(int _id)
	{
		id = _id;
		return RC_OK;
	}
};
