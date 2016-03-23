#include "newschedule.h"

NewSchedule::NewSchedule(Schedule* schedule, QWidget *parent) : QDialog(parent)
{
	ui.setupUi(this);

	ui.fromTimeEdit->setTime(*(schedule->getStartTime()));
	ui.toTimeEdit->setTime(*(schedule->getEndTime()));
	ui.intervalSpinBox->setValue(schedule->getInterval());
}

NewSchedule::~NewSchedule()
{

}