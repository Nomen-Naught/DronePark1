#ifndef DRONEPARK1_H
#define DRONEPARK1_H

#include <QtWidgets/QMainWindow>
#include "ui_dronepark1.h"
#include "loadconfig.h"
#include "newlot.h"
#include "newschedule.h"
#include "ParkingLot.h"
#include "ParkingLotLayout.h"
#include <QPixmap>
#include <QIcon>
#include <QImage>
#include <QTimer>
#include <QDateTime>
#include <QKeyEvent>

class DronePark1 : public QMainWindow
{
	Q_OBJECT

public:
	DronePark1(QWidget *parent = 0);
	~DronePark1();

	QTimer* timer;

	QLabel* currentDateTime;
	QLabel* statusLabel;

	//Actions
	QAction* loadConfigAct;
	QAction* newScheduleAct;
	QAction* exitAction;
	QAction* newLotAct;

	//Other Windows
	LoadConfig* loadConfigWin;
	NewLot* newLotWin;
	NewSchedule* newSchedWin;


	int buildLotGui(Lot* lot);

	int replaceLotGui(Lot* lot);

	//Connect a new spot object to the gui and add a widget for it
	void connectNewSpot(Spot* spot);

	void connectQuit(QApplication* qapp);

	//Shows schedule
	void showSchedule(Schedule* schedule);

	//Clears scheudle
	void clearSchedule();

	//Update Graph
	void updateGraph();

	//Return the ui object. This is probably bad practice...
	Ui::DronePark1Class returnUI();

public slots:
	void showcurrentTime();
	void loadConfigSlot(std::list<Config*>*);
	void loadSchedSlot(Schedule*);
	void loadConfigClose();
	void flightSuccessSlot(int empty, int occupied, int illegal);
	void newLotSlot();
	void updateGraphSlot();

signals:
	void enterPressed();
	void acceptConfigPass(int);
	void acceptSchedPass(QTime*, QTime*, int);

	void newLotOpen(NewLot* LotDialog);
	void newScheduleOpen(NewSchedule* SchedDialog);

protected:

	void keyPressEvent(QKeyEvent* event);


private:
	Ui::DronePark1Class ui;

	// Custom grid layout for drawing parking spots
	ParkingLotLayout* lotGridLayout;

};

#endif // DRONEPARK1_H
