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

	//Set/get button to mirrored for gui
	void setMirrored();
	bool getMirrored();

	//set our icons
	static void setIcons(QIcon*, QIcon*, QIcon*, QIcon*, QIcon*, QIcon*);

	//icons
	static QIcon* empty;
	static QIcon* empty_mirror;
	static QIcon* occupied;
	static QIcon* occupied_mirror;
	static QIcon* illegal;
	static QIcon* illegal_mirror;

public slots:
	//Update the ticketed field and call updateText
	void updateTicketed(int id, bool value);

	//Update the empty field and call updateText
	void updateEmpty(int id, bool value);

	//Update the illegal field
	void updateIllegal(int id, bool value);

	//Click handler, currently sets ticketed to true
	void handleClick();

private:
	bool currentTicketed;
	bool currentEmpty;
	bool currentIllegal;
	bool mirror = false;

	Spot* associatedSpot;

	//Update the text on the widget
	void updateText();


};