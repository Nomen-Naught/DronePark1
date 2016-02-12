#include "dronepark1.h"
#include "SpotButton.h"
#include "MainController.h"
#include "ParkingLot.h"

DronePark1::DronePark1(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
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
	lotGridLayout = new ParkingLotLayout(lot->getRow(), lot->getCol());

	ui.parkingLotLayout->addLayout(lotGridLayout);

	// Set Lot title
	ui.lotInfo->setText(lot->getCity() + ": " + lot->getName());

	return RC_OK;
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
