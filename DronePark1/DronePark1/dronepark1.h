#ifndef DRONEPARK1_H
#define DRONEPARK1_H

#include <QtWidgets/QMainWindow>
#include "ui_dronepark1.h"
#include "ParkingLot.h"

class DronePark1 : public QMainWindow
{
	Q_OBJECT

public:
	DronePark1(QWidget *parent = 0);
	~DronePark1();

	//Connect a new spot object to the gui and add a widget for it
	void connectNewSpot(Spot* spot);

	//Return the ui object. This is probably bad practice...
	Ui::DronePark1Class returnUI();

private:
	Ui::DronePark1Class ui;
};

#endif // DRONEPARK1_H
