#include "Schedule.h"

//TODO: Nick: Schedule Constructor maybe should do something?
// Constructor, should call DbObject constructor
Schedule::Schedule() : DbObject()
{

}

void Schedule::setStartTime(QTime* _newStartTime)
{
	startTime = _newStartTime;

	return;
}

void Schedule::setEndTime(QTime* _newEndTime)
{
	endTime = _newEndTime;

	return;
}

void Schedule::setInterval(int _newInterval)
{
	interval = _newInterval;
	return;
}

QTime* Schedule::getStartTime()
{
	return startTime;
}

QTime* Schedule::getEndTime()
{
	return endTime;
}

int Schedule::getInterval()
{
	return interval;
	{

	};
}

