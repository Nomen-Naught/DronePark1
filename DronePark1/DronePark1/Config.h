#pragma once
#include "SubjectObserver.h"
#include "ReturnCodes.h"
#include "ParkingLot.h"
#include "Schedule.h"

class Config : DbObject
{
private:
	// Holds the currently associated lot
	Lot currentLot;

	// Holds the currently associated schedule;
	Schedule currentSchedule;

	// Flag indicating if Drone will automatically fly according to associated schedule
	bool useSchedule;

public:
	// Returns currentLot
	Lot getCurrentLot()
	{
		return currentLot;
	}

	// Returns currentSchedule
	Schedule getCurrentSchedule()
	{
		return currentSchedule;
	}

	// Returns useSchedule
	bool getUseSchedule()
	{
		return useSchedule;
	}

	// Sets currentLot and returns RC
	int setCurrentLot(Lot _currentLot)
	{
		currentLot = _currentLot;
		return RC_OK;
	}

	// Sets currentSchedule and returns RC
	int setCurrentSchedule(Schedule _currentSchedule)
	{
		currentSchedule = _currentSchedule;
		return RC_OK;
	}

	// Sets useSchedule and returns RC
	int setUseSchedule(bool _useSchedule)
	{
		useSchedule = _useSchedule;
		return RC_OK;
	}
	
};