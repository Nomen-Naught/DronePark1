#pragma once
#include "DBObject.h"
#include "ReturnCodes.h"
#include "ParkingLot.h"
#include <QDateTime>

class Schedule : DbObject
{
private:
	// Holdes the next time the Drone will fly
	QDateTime nextTime;
	
	// List of all times that a Drone will fly automatically
	std::list<QDateTime> sweepTimes;

public:

	//TODO: Nick: Schedule Constructor maybe should do something?
	// Constructor, should call DbObject constructor
	Schedule();

	//TODO: Nick: Implement addTime
	// Adds another time to the sweep list, returns RC
	int addTime(QDateTime newTime);

	//TODO: Nick: Implement advanceSchedule
	// Sets the nextTime to the next time in the sweeplist, returns RC
	int advanceSchedule();

	//TODO: Nick: Implement removeTime
	// Removes a time from the sweepTimes list, returns RC
	int removeTime(QDateTime removeTime);

	// Returns sweepTimes
	std::list<QDateTime> getSweepTimes();

	// Returns nextTime
	QDateTime getNextTime();
};