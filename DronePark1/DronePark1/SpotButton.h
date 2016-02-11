#pragma once
#include <QtWidgets/QPushButton>
#include <QToolButton>
#include "ParkingLot.h"

class SpotButton : public QToolButton
{
	Q_OBJECT

public:
	SpotButton(QWidget *parent = 0);
	~SpotButton();

	//Sets the spot member to associate the widget with an actual spot object
	void setSpot(Spot* spot);

public slots:
	//Update the ticketed field and call updateText
	void updateTicketed(int id, bool value);

	//Update the empty field and call updateText
	void updateEmpty(int id, bool value);

	//Click handler, currently sets ticketed to true
	void handleClick();

private:
	bool currentTicketed;
	bool currentEmpty;

	Spot* associatedSpot;

	//Update the text on the widget
	void updateText();
};