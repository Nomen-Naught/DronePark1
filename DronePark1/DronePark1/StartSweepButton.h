#pragma once
#include <QtWidgets/QPushButton>
#include "ParkingLot.h"

class StartSweepButton : public QPushButton
{
	Q_OBJECT

public:
	StartSweepButton(QWidget *parent = 0);
	~StartSweepButton();

	public slots:
	//Click handler, 
	void handleClick();

signals:
	//Start sweep signal
	void startSweep();
};