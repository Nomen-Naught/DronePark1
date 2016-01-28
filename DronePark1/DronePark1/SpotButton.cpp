#include "SpotButton.h"
#include <QMessageBox>

SpotButton::SpotButton(QWidget *parent)
	: QPushButton(parent)
{
	currentTicketed = false;
	currentEmpty = false;

	//Connect our own clicked signal to our own click handler
	QObject::connect(this, SIGNAL(clicked()), this, SLOT(handleClick()));
}

SpotButton::~SpotButton()
{

}

//Sets the spot member to associate the widget with an actual spot object
void SpotButton::setSpot(Spot* _spot)
{
	associatedSpot = _spot;
}

//Update the ticketed field and call updateText
void SpotButton::updateTicketed(int id, bool value)
{
	currentTicketed = value;
	updateText();
}

//Update the empty field and call updateText
void SpotButton::updateEmpty(int id, bool value)
{
	currentEmpty = value;
	updateText();
}

//Update the text on the widget
void SpotButton::updateText()
{
	QString text = "Empty: " + QString::number(currentEmpty) + " Ticket: " + QString::number(currentTicketed);
	setText(text);
}

//Click handler, currently sets ticketed to true
void SpotButton::handleClick()
{
	//We should have a spot object to mark
	if (associatedSpot != NULL)
	{
		//It doesn't make sense to be able to ticket an empty spot
		if (currentEmpty != false)
		{
			associatedSpot->setTicketed(true);
			return;
		}
	}
	else
	{
		DP_ASSERT(false, "handleClick", "no associated spot");
		return;
	}
}