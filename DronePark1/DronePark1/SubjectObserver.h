#pragma once
#include <list>
#include "ReturnCodes.h"

// Forward delcarations
class Subject;
class Observer;

class Observer
{
public:
	// A virtual method which is intended to inform the Observer of a change in a subject object.
	virtual int notify(Subject subject);
};

class Subject
{
public:

	// List of Observers associated with the subject
	std::list<Observer> observers;

	//TODO: Nick: Implement addObserver
	// Adds an Observer object to the observer list 
	int addObserver(Observer)
	{
		return RC_ERR;
	}

	//TODO: Nick: Implement removeObserver
	// Removes an Observer object from the observer list
	int removeObserver(Observer)
	{
		return RC_ERR;
	}

	//TODO: Nick: Implement notifyObservers
	// Informs the Observers of a change in a subject
	int notifyObservers()
	{
		return RC_ERR;
	}

	// TODO: Nick: idk how constructors work (Subject object)
	// Subject Constructor
	Subject()
	{
		observers = std::list<Observer>();
	}
};

class DbObject : Subject
{
protected:
	// Unique identifier, generated and set by Database!
	// -1 means a proper ID has not been set yet.
	int id;

public:
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
