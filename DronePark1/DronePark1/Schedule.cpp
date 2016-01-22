#include "Schedule.h"

//TODO: Nick: Schedule Constructor maybe should do something?
// Constructor, should call DbObject constructor
Schedule::Schedule() : DbObject()
{

}

//TODO: Nick: Implement addTime
// Adds another time to the sweep list, returns RC
int Schedule::addTime(QDateTime newTime)
{
	return RC_ERR;
}

//TODO: Nick: Implement advanceSchedule
// Sets the nextTime to the next time in the sweeplist, returns RC
int Schedule::advanceSchedule()
{
	return RC_ERR;
}

//TODO: Nick: Implement removeTime
// Removes a time from the sweepTimes list, returns RC
int Schedule::removeTime(QDateTime removeTime)
{
	return RC_ERR;
}

// Returns sweepTimes
std::list<QDateTime> Schedule::getSweepTimes()
{
	return sweepTimes;
}

// Returns nextTime
QDateTime Schedule::getNextTime()
{
	return nextTime;
}