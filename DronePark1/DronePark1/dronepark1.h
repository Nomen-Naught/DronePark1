#ifndef DRONEPARK1_H
#define DRONEPARK1_H

#include <QtWidgets/QMainWindow>
#include "ui_dronepark1.h"
#include "ParkingLot.h"
#include "ParkingLotLayout.h"
#include <QPixmap>
#include <QIcon>
#include <QImage>

class DronePark1 : public QMainWindow
{
	Q_OBJECT

public:
	DronePark1(QWidget *parent = 0);
	~DronePark1();


	int buildLotGui(Lot* lot);

	//Connect a new spot object to the gui and add a widget for it
	void connectNewSpot(Spot* spot);

	//Return the ui object. This is probably bad practice...
	Ui::DronePark1Class returnUI();

private:
	Ui::DronePark1Class ui;

	// Custom grid layout for drawing parking spots
	ParkingLotLayout* lotGridLayout;

};

#endif // DRONEPARK1_H
