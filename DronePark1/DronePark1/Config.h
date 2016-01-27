#pragma once
#include "SubjectObserver.h"
#include "ReturnCodes.h"
#include "ParkingLot.h"
#include "Schedule.h"

class Config : public DbObject
{
private:
	// Holds the currently associated lot
	Lot* currentLot;

	// Holds the currently associated schedule;
	Schedule* currentSchedule;

	// Flag indicating if Drone will automatically fly according to associated schedule
	bool useSchedule;

public:

	Config();

	// Returns currentLot
	Lot* getCurrentLot();

	// Returns currentSchedule
	Schedule* getCurrentSchedule();

	// Returns useSchedule
	bool getUseSchedule();

	// Sets currentLot and returns RC
	int setCurrentLot(Lot* _currentLot);

	// Sets currentSchedule and returns RC
	int setCurrentSchedule(Schedule* _currentSchedule);

	// Sets useSchedule and returns RC
	int setUseSchedule(bool _useSchedule);
	
};