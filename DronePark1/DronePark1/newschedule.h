#pragma once
#ifndef NEWSCHEDULE_H
#define NEWSCHEDULE_H

#include <QtWidgets/QDialog>
#include "ui_newschedule.h"
#include "Schedule.h"

class NewSchedule : public QDialog
{
	Q_OBJECT

public:
	NewSchedule(Schedule* schedule, QWidget *parent = 0);
	~NewSchedule();


private:
	Ui::NewSchedule ui;

};

#endif // LOADCONFIG_H