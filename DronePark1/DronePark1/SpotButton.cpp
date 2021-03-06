#include "SpotButton.h"
#include <QMessageBox>
#include <QPainter>


QIcon* SpotButton::empty;
QIcon* SpotButton::empty_mirror;
QIcon* SpotButton::occupied;
QIcon* SpotButton::occupied_mirror;
QIcon* SpotButton::illegal;
QIcon* SpotButton::illegal_mirror;

QRect SpotButton::rect1;
QRect SpotButton::rect2;
QRect SpotButton::rect3;
QRect SpotButton::rect4;


SpotButton::SpotButton(QWidget *parent)
	: QToolButton(parent)
{
	currentTicketed = false;
	currentEmpty = false;

	setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

	//Connect our own clicked signal to our own click handler
	QObject::connect(this, SIGNAL(clicked()), this, SLOT(handleClick()));
}

SpotButton::~SpotButton()
{

}

//Set our static spot images
void SpotButton::setIcons(QIcon* _empty, QIcon* _empty_mirror, QIcon* _occupied, QIcon* _occupied_mirror, QIcon* _illegal, QIcon* _illegal_mirror)
{
	empty = _empty;
	empty_mirror = _empty_mirror;
	occupied = _occupied;
	occupied_mirror = _occupied_mirror;
	illegal = _illegal;
	illegal_mirror = _illegal_mirror;
}

void SpotButton::setRect(QRect _rect1, QRect _rect2, QRect _rect3, QRect _rect4)
{
	rect1 = _rect1;
	rect2 = _rect2;
	rect3 = _rect3;
	rect4 = _rect4;
}

//Sets the spot member to associate the widget with an actual spot object
void SpotButton::setSpot(Spot* _spot)
{
	associatedSpot = _spot;
}

//Sets the spot member to associate the widget with an actual spot object
void SpotButton::setMirrored()
{
	mirror = true;
}

void SpotButton::setOverhead(bool _overhead)
{
	overhead = _overhead;
	this->repaint();
}

//Sets the spot member to associate the widget with an actual spot object
bool SpotButton::getMirrored()
{
	return mirror;
}

//Update the ticketed field and call updateText
void SpotButton::updateTicketed(int id, bool value)
{
	currentTicketed = value;


	updateText();
}

//Update the illegal field and call updateText
//Should only be called if there's a change in state
void SpotButton::updateIllegal(int id, bool value)
{
	currentIllegal = value;

	// Change the icon to correspond with the upate
	if (value == true)
	{
		//Handle mirrored spots
		if (mirror)
		{
			setIcon(*illegal_mirror);
		}
		else
		{
			setIcon(*illegal);
		}
	}
	// Not sure if this is needed, I would imagine we would from illegal state -> empty state
	else if(currentEmpty == true)
	{
		if (mirror)
		{
			setIcon(*occupied_mirror);
		}
		else
		{
			setIcon(*occupied);
		}
	}
}

//Update the empty field and call updateText
//Should only be called if there's a change in state
void SpotButton::updateEmpty(int id, bool value)
{
	currentEmpty = value;

	// Change the icon to correspond with the upate
	if (value == true)
	{
		//Handle mirrored spots
		if (mirror)
		{
			setIcon(*occupied_mirror);
		}
		else
		{
			setIcon(*occupied);
		}
	}
	else
	{
		if (mirror)
		{
			setIcon(*empty_mirror);
		}
		else
		{
			setIcon(*empty);
		}
	}

	updateText();
}

//Update the text on the widget
void SpotButton::updateText()
{
	QString text;
	QFont f("Arial", 12, QFont::Bold);

	if (currentTicketed)
	{
		text = "TICKETED";
	}

	setText(text);
	setFont(f);
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
			//Currently toggles, up for discussion ( database write could slow things down?? )
			associatedSpot->setTicketed(!currentTicketed);
			return;
		}
	}
	else
	{
		DP_ASSERT2(false, "handleClick", "no associated spot");
		return;
	}
}

void SpotButton::paintEvent(QPaintEvent *Event)
{
	QToolButton::paintEvent(Event);


	if (overhead)
	{
		QPainter painter(this);

		painter.setBrush(QBrush(Qt::red, Qt::SolidPattern));

		painter.drawRect(rect1);
		painter.drawRect(rect2);
		painter.drawRect(rect3);
		painter.drawRect(rect4);
	}

}