#pragma once
#include "DBObject.h"
#include "ReturnCodes.h"
#include "ParkingLot.h"
#include <QDateTime>

class Schedule : public DbObject
{
private:
	// Holdes the start time for the schedule
	QTime* startTime = NULL;

	//Holds the end time for the schedule
	QTime* endTime = NULL;

	//The interval between sweeps in minutes
	int interval;

public:

	// Constructor, should call DbObject constructor
	Schedule();

	void setStartTime(QTime*);
	void setEndTime(QTime*);
	void setInterval(int);

	QTime* getStartTime();
	QTime* getEndTime();
	int getInterval();
};