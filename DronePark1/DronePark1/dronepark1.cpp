#include "dronepark1.h"
#include "SpotButton.h"
#include "MainController.h"
#include "ParkingLot.h"

DronePark1::DronePark1(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	spotPixMap.load("parkingSpot.png");
	spotPixMap = spotPixMap.scaled(QSize(100, 200), Qt::KeepAspectRatio);
	spotIcon = new QIcon(spotPixMap);
}

DronePark1::~DronePark1()
{

}

//Return the ui object. This is probably bad practice...
Ui::DronePark1Class DronePark1::returnUI()
{
	return ui;
}

int DronePark1::buildLotGui(Lot* lot)
{
	lotGridLayout = new ParkingLotLayout(lot->getRow(), lot->getCol());

	ui.parkingLotLayout->addLayout(lotGridLayout);

	return RC_OK;
}

//Connect a new spot object to the gui and add a widget for it
void DronePark1::connectNewSpot(Spot* spot)
{
	//Create a new widget
	SpotButton* spotButton = new SpotButton();

	//Associate the spot with the widget
	spotButton->setSpot(spot);

	spotButton->setIcon(*spotIcon);
	spotButton->setIconSize(spotPixMap.rect().size());
	spotButton->setFixedSize(QSize(spotPixMap.rect().width() * (2.2), spotPixMap.rect().height() * (1.4)));

	//Add the new widget to the layout
	lotGridLayout->add_widget(spotButton);

	//Connect the widget to the passed in spot object
	QObject::connect(spot, SIGNAL(spotTicketedChanged(int, bool)),
					 spotButton, SLOT(updateTicketed(int, bool)));

	QObject::connect(spot, SIGNAL(spotEmptyChanged(int, bool)),
					 spotButton, SLOT(updateEmpty(int, bool)));

	//Do a one time explicit update of the widget
	spotButton->updateEmpty(spot->getId(), spot->getEmpty());
	spotButton->updateTicketed(spot->getId(), spot->getTicketed());
}
