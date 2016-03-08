#include "dronepark1.h"
#include "SpotButton.h"
#include "MainController.h"
#include "ParkingLot.h"

DronePark1::DronePark1(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	loadConfigAct = new QAction(tr("&Open Config"), this);
	loadConfigAct->setShortcuts(QKeySequence::Open);
	loadConfigAct->setStatusTip(tr("Load a New Configuration"));
	connect(loadConfigAct, SIGNAL(triggered()), this, SLOT(loadConfigSlot()));

	newLotAct = new QAction(tr("&New Lot"), this);
	newLotAct->setShortcuts(QKeySequence::New);
	newLotAct->setStatusTip(tr("Create a New Lot Configuration"));
	connect(newLotAct, SIGNAL(triggered()), this, SLOT(newLotSlot()));

	ui.menuFile->addAction(newLotAct);
	ui.menuFile->addAction(loadConfigAct);

}

DronePark1::~DronePark1()
{

}


//Return the ui object. This is probably bad practice...
Ui::DronePark1Class DronePark1::returnUI()
{
	return ui;
}

//Build and add the layout that will hold the spot graphics
int DronePark1::buildLotGui(Lot* lot)
{

	QWidget * widget = new QWidget();
	QGridLayout * layout = new QGridLayout(widget);

	lotGridLayout = new ParkingLotLayout(lot->getRow(), lot->getCol());

	ui.parkingLotLayout->addLayout(lotGridLayout);

	ui.parkingLotLayout->setAlignment(Qt::AlignLeft);
	ui.buttonLayout->setAlignment(Qt::AlignRight);

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

void DronePark1::loadConfigSlot()
{
	loadConfigWin = new LoadConfig();
	connect(loadConfigWin, SIGNAL(finished(int)), this, SLOT(loadConfigClose()));

	loadConfigWin->setModal(true);
	loadConfigWin->show();
}

void DronePark1::loadConfigClose()
{
	if (loadConfigWin != NULL)
	{
		delete loadConfigWin;
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

	//Do a one time explicit update of the widget
	spotButton->updateEmpty(spot->getId(), spot->getEmpty());
	spotButton->updateTicketed(spot->getId(), spot->getTicketed());
	spotButton->updateIllegal(spot->getId(), spot->getIllegal());
}
