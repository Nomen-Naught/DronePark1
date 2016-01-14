#pragma once
#include "SubjectObserver.h"
#include "ReturnCodes.h"
#include "ParkingLot.h"

class Schedule : DbObject
{
private:
	// Holdes the next time the Drone will fly
	QDateTime nextTime;
	
	// List of all times that a Drone will fly automatically
	std::list<QDateTime> sweepTimes;

public:
	//TODO: Nick: Implement addTime
	// Adds another time to the sweep list, returns RC
	int addTime(QDateTime newTime)
	{
		return RC_ERR;
	}

	//TODO: Nick: Implement advanceSchedule
	// Sets the nextTime to the next time in the sweeplist, returns RC
	int advanceSchedule()
	{
		return RC_ERR;
	}

	//TODO: Nick: Implement removeTime
	// Removes a time from the sweepTimes list, returns RC
	int removeTime(QDateTime removeTime)
	{
		return RC_ERR;
	}

	// Returns sweepTimes
	std::list<QDateTime> getSweepTimes()
	{
		return sweepTimes;
	}

	// Returns nextTime
	QDateTime getNextTime()
	{
		return nextTime;
	}
};