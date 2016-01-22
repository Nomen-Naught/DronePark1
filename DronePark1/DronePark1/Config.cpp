#include "Config.h"

// Returns currentLot
Lot Config::getCurrentLot()
{
	return currentLot;
}

// Returns currentSchedule
Schedule Config::getCurrentSchedule()
{
	return currentSchedule;
}

// Returns useSchedule
bool Config::getUseSchedule()
{
	return useSchedule;
}

// Sets currentLot and returns RC
int Config::setCurrentLot(Lot _currentLot)
{
	currentLot = _currentLot;
	return RC_OK;
}

// Sets currentSchedule and returns RC
int Config::setCurrentSchedule(Schedule _currentSchedule)
{
	currentSchedule = _currentSchedule;
	return RC_OK;
}

// Sets useSchedule and returns RC
int Config::setUseSchedule(bool _useSchedule)
{
	useSchedule = _useSchedule;
	return RC_OK;
}