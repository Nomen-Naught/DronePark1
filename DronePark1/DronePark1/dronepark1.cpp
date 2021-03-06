#include "dronepark1.h"
#include "SpotButton.h"
#include "ParkingLot.h"
#include <QTableWidgetItem>

#define DUMMY_HISTORY_DATA true

DronePark1::DronePark1(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	//Create menu bar

	//Open Config
	loadConfigAct = new QAction(tr("&Open Config..."), this);
	loadConfigAct->setShortcuts(QKeySequence::Open);
	loadConfigAct->setStatusTip(tr("Load a New Configuration"));

	//New Schedule
	newScheduleAct = new QAction(tr("Edit Schedule..."), this);
	newScheduleAct->setStatusTip(tr("Edit the Schedule"));

	//Exit
	exitAction = new QAction(tr("Exit"), this);
	exitAction->setShortcut(QKeySequence(tr("Alt+F4")));
	exitAction->setStatusTip(tr("Exit DronePark"));


	//File
	newLotAct = new QAction(tr("&New Lot..."), this);
	newLotAct->setShortcuts(QKeySequence::New);
	newLotAct->setStatusTip(tr("Create a New Lot Configuration"));

	ui.menuFile->addAction(newLotAct);
	ui.menuFile->addAction(loadConfigAct);


	//Add new Actions above!
	ui.menuFile->addSeparator();
	ui.menuFile->addAction(exitAction);

	//Schedule
	ui.menuSchedule->addAction(newScheduleAct);

	//radio button group for graphs
	QObject::connect(ui.illSpotButton, SIGNAL(toggled(bool)), this, SLOT(updateGraphSlot()));
	QObject::connect(ui.validSpotButton, SIGNAL(toggled(bool)), this, SLOT(updateGraphSlot()));
	QObject::connect(ui.emptySpotButton, SIGNAL(toggled(bool)), this, SLOT(updateGraphSlot()));

	//Create lot info history table
	QStringList headers;
	headers << "Time" << "Empty" << "Occupied" << "Illegal";

	ui.historyTable->setHorizontalHeaderLabels(headers);

	if (DUMMY_HISTORY_DATA)
	{

		//Grab the current time and format it
		QDateTime time = QDateTime::currentDateTime();
		QString timeString;

		QTableWidgetItem *newDate;
		QTableWidgetItem *newEmpty;
		QTableWidgetItem *newOccupied;
		QTableWidgetItem *newIllegal;

		time = time.addSecs(60 * 60 * 24 * -1);

		for (int i = 0; i < 24; i++)
		{

			
			ui.historyTable->insertRow(0);

			newDate = new QTableWidgetItem(time.toString("MM/dd/yyyy hh:mm"));
			newDate->setFlags(newDate->flags() ^ Qt::ItemIsEditable);

			newEmpty = new QTableWidgetItem(QString::number(qrand() % 10));
			newEmpty->setFlags(newEmpty->flags() ^ Qt::ItemIsEditable);

			newOccupied = new QTableWidgetItem(QString::number(qrand() % 10));
			newOccupied->setFlags(newOccupied->flags() ^ Qt::ItemIsEditable);

			newIllegal = new QTableWidgetItem(QString::number(qrand() % 10));
			newIllegal->setFlags(newIllegal->flags() ^ Qt::ItemIsEditable);

			ui.historyTable->setItem(0, 0, newDate);
			ui.historyTable->setItem(0, 1, newEmpty);
			ui.historyTable->setItem(0, 2, newOccupied);
			ui.historyTable->setItem(0, 3, newIllegal);

			time = time.addSecs(60 * 60);
		}

		updateGraph();
	}

	//Style tweaks
	ui.tabWidget->setStyleSheet("QTabBar::tab { height: 35px; width: 100px; }");
	ui.menuFile->setStyleSheet("QMenu::separator{ height: 1px; background: white;margin-left: 10px; margin-right: 5px; }");
}

void DronePark1::connectQuit(QApplication* qapp)
{
	connect(exitAction, SIGNAL(triggered()), qapp, SLOT(closeAllWindows()));
}

DronePark1::~DronePark1()
{

}


//Return the ui object. This is probably bad practice...
Ui::DronePark1Class DronePark1::returnUI()
{
	return ui;
}

int DronePark1::replaceLotGui(Lot* lot)
{
	if (lotGridLayout != NULL)
	{
		QLayoutItem* item;
		while ((item = lotGridLayout->takeAt(0)) != NULL)
		{
			delete item->widget();
			delete item;
		}
		delete lotGridLayout;
	}

	lotGridLayout = new ParkingLotLayout(lot->getRow(), lot->getCol());

	ui.parkingLotLayout->addLayout(lotGridLayout);

	ui.parkingLotLayout->setAlignment(Qt::AlignLeft);
	ui.rightButtonLayout->setAlignment(Qt::AlignRight);

	// Set Lot title
	ui.lotInfo->setText(lot->getCity() + ": " + lot->getName());

	if (DUMMY_HISTORY_DATA)
	{

		//Grab the current time and format it
		QDateTime time = QDateTime::currentDateTime();
		QString timeString;

		QTableWidgetItem *newDate;
		QTableWidgetItem *newEmpty;
		QTableWidgetItem *newOccupied;
		QTableWidgetItem *newIllegal;

		time = time.addSecs(60 * 60 * 24 * -1);

		for (int i = 0; i < 24; i++)
		{


			ui.historyTable->insertRow(0);

			newDate = new QTableWidgetItem(time.toString("MM/dd/yyyy hh:mm"));
			newDate->setFlags(newDate->flags() ^ Qt::ItemIsEditable);

			newEmpty = new QTableWidgetItem(QString::number(qrand() % 10));
			newEmpty->setFlags(newEmpty->flags() ^ Qt::ItemIsEditable);

			newOccupied = new QTableWidgetItem(QString::number(qrand() % 10));
			newOccupied->setFlags(newOccupied->flags() ^ Qt::ItemIsEditable);

			newIllegal = new QTableWidgetItem(QString::number(qrand() % 10));
			newIllegal->setFlags(newIllegal->flags() ^ Qt::ItemIsEditable);

			ui.historyTable->setItem(0, 0, newDate);
			ui.historyTable->setItem(0, 1, newEmpty);
			ui.historyTable->setItem(0, 2, newOccupied);
			ui.historyTable->setItem(0, 3, newIllegal);

			time = time.addSecs(60 * 60);
		}
	}

	updateGraph();

	return RC_OK;
}

//Build and add the layout that will hold the spot graphics
int DronePark1::buildLotGui(Lot* lot)
{
	
	QWidget * widget;
	QGridLayout * layout;

	widget = new QWidget();

	layout = new QGridLayout(widget);


	lotGridLayout = new ParkingLotLayout(lot->getRow(), lot->getCol());

	ui.parkingLotLayout->addLayout(lotGridLayout);

	ui.parkingLotLayout->setAlignment(Qt::AlignLeft);
	ui.rightButtonLayout->setAlignment(Qt::AlignRight);

	timer = new QTimer(this);

	currentDateTime = new QLabel();
	statusLabel = new QLabel();

	QFont font;
	font.setPointSize(10);
	font.setBold(true);

	QFont font2;
	font2.setPointSize(10);

	currentDateTime->setFont(font);
	statusLabel->setFont(font2);

	connect(timer, SIGNAL(timeout()), this, SLOT(showcurrentTime()));

	currentDateTime->setText(QDateTime::currentDateTime().toString("dd/MM/yyyy hh:mm") + "   ");

	timer->start(60000);

	statusLabel->setText(" DronePark");

	//Throw the status label in a gridlayout to right align it
	layout->addWidget(statusLabel, 0, 0, 1, 1, Qt::AlignVCenter | Qt::AlignLeft);
	layout->setMargin(0);

	//Build status bar
	ui.statusBar->addWidget(widget, 1);
	ui.statusBar->addPermanentWidget(currentDateTime);

	// Set Lot title
	ui.lotInfo->setText(lot->getCity() + ": " + lot->getName());

	return RC_OK;
}

void DronePark1::showcurrentTime()
{
	QDateTime dateTime = dateTime.currentDateTime();
	currentDateTime->setText(QDateTime::currentDateTime().toString("dd/MM/yyyy hh:mm") + "   ");
}

void DronePark1::newLotSlot()
{
	newLotWin = new NewLot();

	emit newLotOpen(newLotWin);

	newLotWin->setModal(true);
	newLotWin->show();
}

void DronePark1::loadSchedSlot(Schedule* _sched)
{
	newSchedWin = new NewSchedule(_sched);

	emit newScheduleOpen(newSchedWin);

	connect(newSchedWin, SIGNAL(acceptedSchedule(QTime* , QTime* , int )), this, SIGNAL(acceptSchedPass(QTime* , QTime* , int)));

	newSchedWin->setModal(true);
	newSchedWin->show();
}

void DronePark1::loadConfigSlot(std::list<Config*>* configs)
{
	loadConfigWin = new LoadConfig(configs);
	connect(loadConfigWin, SIGNAL(finished(int)), this, SLOT(loadConfigClose()));
	connect(loadConfigWin, SIGNAL(acceptedConfig(int)), this, SIGNAL(acceptConfigPass(int)));

	loadConfigWin->setModal(true);
	loadConfigWin->show();
}

void DronePark1::loadConfigClose()
{
	if (loadConfigWin != NULL)
	{
		//This is causing weird race conditions... but I think it leaks otherwise!
		//delete loadConfigWin;
	}
}

//Connect a new spot object to the gui and add a widget for it
void DronePark1::connectNewSpot(Spot* spot)
{
	//Create a new widget
	SpotButton* spotButton = new SpotButton();

	//Associate the spot with the widget
	spotButton->setSpot(spot);

	//Add the new widget to the layout
	lotGridLayout->add_spot(spotButton);

	//Connect the widget to the passed in spot object
	QObject::connect(spot, SIGNAL(spotTicketedChanged(int, bool)),
					 spotButton, SLOT(updateTicketed(int, bool)));

	QObject::connect(spot, SIGNAL(spotEmptyChanged(int, bool)),
					 spotButton, SLOT(updateEmpty(int, bool)));

	QObject::connect(spot, SIGNAL(spotIllegalChanged(int, bool)),
					 spotButton, SLOT(updateIllegal(int, bool)));

	// So we can set if the drone is overhead right now
	QObject::connect(spot, SIGNAL(overheadChanged(bool)),
		spotButton, SLOT(setOverhead(bool)));

	//Do a one time explicit update of the widget
	spotButton->updateEmpty(spot->getId(), spot->getEmpty());
	spotButton->updateTicketed(spot->getId(), spot->getTicketed());
	spotButton->updateIllegal(spot->getId(), spot->getIllegal());
}

void DronePark1::keyPressEvent(QKeyEvent* event)
{
	//qDebug() << event->text();
	if (event->key() == Qt::Key_N)
	{
		emit enterPressed();
	}
}

void DronePark1::showSchedule(Schedule* _schedule)
{
	bool nextSweep = true;

	QLabel* newLabel;

	QDateTime scheduledTime (QDate::currentDate(), *(_schedule->getStartTime()));

	QDateTime endTime(QDate::currentDate(), *(_schedule->getEndTime()));

	QFont f("Arial", 10, QFont::Bold);

	int interval = _schedule->getInterval();

	while (scheduledTime < endTime)
	{

		scheduledTime = scheduledTime.addSecs(interval * 60);

		QString schedTimeStr = scheduledTime.toString();
		QString currentTime = QDateTime::currentDateTime().toString();
		QString endTimeStr = endTime.toString();

		if (scheduledTime > QDateTime::currentDateTime())
		{

			if (nextSweep == true)
			{
				ui.nextSweep->setText(scheduledTime.toString("hh:mm ddd MMMM d"));
				nextSweep = false;
			}

			newLabel = new QLabel();
			newLabel->setText(scheduledTime.toString("hh:mm ddd MMMM d"));
			newLabel->setFont(f);
			ui.scheduleTimeLayout->addWidget(newLabel);
		}
	}
}

void DronePark1::clearSchedule()
{
	while(ui.scheduleTimeLayout->count() != 0)
	{
		QWidget *widget = ui.scheduleTimeLayout->itemAt(0)->widget();
		if (widget != NULL)
		{
			delete widget;
		}
	}

	ui.nextSweep->setText("Never");

}

void DronePark1::flightSuccessSlot(int empty, int occupied, int illegal)
{
	//Grab the current time and format it
	QDateTime now = QDateTime::currentDateTime();
	QString time = now.toString("MM/dd/yyyy hh:mm");

	ui.lastSweep->setText(now.toString());

	ui.historyTable->insertRow(0);

	QTableWidgetItem *newDate = new QTableWidgetItem(time);
	newDate->setFlags(newDate->flags() ^ Qt::ItemIsEditable);

	QTableWidgetItem *newEmpty = new QTableWidgetItem(QString::number(empty));
	newEmpty->setFlags(newEmpty->flags() ^ Qt::ItemIsEditable);

	QTableWidgetItem *newOccupied = new QTableWidgetItem(QString::number(occupied));
	newOccupied->setFlags(newOccupied->flags() ^ Qt::ItemIsEditable);

	QTableWidgetItem *newIllegal = new QTableWidgetItem(QString::number(illegal));
	newIllegal->setFlags(newIllegal->flags() ^ Qt::ItemIsEditable);

	ui.historyTable->setItem(0, 0, newDate);
	ui.historyTable->setItem(0, 1, newEmpty);
	ui.historyTable->setItem(0, 2, newOccupied);
	ui.historyTable->setItem(0, 3, newIllegal);
	updateGraph();

}

void DronePark1::updateGraph()
{
	//Get total number of rows in table
	int rowCount;
	int max  = 0;
	rowCount = ui.historyTable->rowCount();

	//initialize values
	QVector<double> time(rowCount), value(rowCount);
	for (int i = 0; i < rowCount; i++)
	{
		QString d = ui.historyTable->item(i, 0)->text();
		QDateTime tempDate = QDateTime::fromString(d, "MM/dd/yyyy hh:mm");
		double temp = tempDate.toTime_t();
		time[i] = temp;
		if (ui.illSpotButton->isChecked())
		{
			value[i] = ui.historyTable->item(i, 3)->text().toInt();
		}
		if (ui.validSpotButton->isChecked())
		{
			value[i] = ui.historyTable->item(i, 2)->text().toInt();
		}
		if (ui.emptySpotButton->isChecked())
		{
			value[i] = ui.historyTable->item(i, 1)->text().toInt();
		}

		if (value[i] > max)
		{
			max = value[i];
		}
	}

	// create graph and assign data to it:
	ui.customPlot->addGraph();
	if (ui.illSpotButton->isChecked())
	{
		QPen pen;
		pen.setColor(QColor(255, 0, 0, 200));
		ui.customPlot->graph(0)->setBrush(QBrush(QColor(255, 50, 50, 50)));
		ui.customPlot->graph(0)->setPen(pen);
		ui.customPlot->yAxis->setLabel("Illegal Spots");

	}
	if (ui.validSpotButton->isChecked())
	{
		QPen pen;
		pen.setColor(QColor(0, 255, 0, 200));
		ui.customPlot->graph(0)->setBrush(QBrush(QColor(50, 255, 50, 50)));
		ui.customPlot->graph(0)->setPen(pen);
		ui.customPlot->yAxis->setLabel("Valid Spots");

	}
	if (ui.emptySpotButton->isChecked())
	{
		QPen pen;
		pen.setColor(QColor(0, 0, 255, 200));
		ui.customPlot->graph(0)->setBrush(QBrush(QColor(50, 50, 255, 50)));
		ui.customPlot->graph(0)->setPen(pen);
		ui.customPlot->yAxis->setLabel("Empty Spots");

	}
	ui.customPlot->graph()->setLineStyle(QCPGraph::lsLine);
	ui.customPlot->graph()->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 5));
	ui.customPlot->xAxis->setTickLabelType(QCPAxis::ltDateTime);
	ui.customPlot->xAxis->setDateTimeFormat("hh:mm");
	ui.customPlot->graph(0)->setData(time, value);
	// give the axes some labels:
	ui.customPlot->xAxis->setLabel("Time");
	// set axes ranges, so we see all data:
	double now = QDateTime::currentDateTime().toTime_t();
	ui.customPlot->xAxis->setRange(now-60*60*24,now);
	ui.customPlot->yAxis->setRange(0, max+1);
	ui.customPlot->replot();
	
}

void DronePark1::updateGraphSlot()
{
	updateGraph();
}