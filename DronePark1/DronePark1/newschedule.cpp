#include "newschedule.h"

NewSchedule::NewSchedule(Schedule* schedule, QWidget *parent) : QDialog(parent)
{
	ui.setupUi(this);

	ui.fromTimeEdit->setTime(*(schedule->getStartTime()));
	ui.toTimeEdit->setTime(*(schedule->getEndTime()));
	ui.intervalSpinBox->setValue(schedule->getInterval());
}

void NewSchedule::accept()
{
	QTime* startTime = new QTime(ui.fromTimeEdit->time());
	QTime* endTime = new QTime(ui.toTimeEdit->time());
	int interval = ui.intervalSpinBox->value();

	emit acceptedSchedule( startTime,  endTime,  interval);

	//Call superclass accept
	QDialog::accept();
}

NewSchedule::~NewSchedule()
{

}