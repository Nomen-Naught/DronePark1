#include "StartSweepButton.h"

StartSweepButton::StartSweepButton(QWidget *parent)
	: QPushButton(parent)
{
	//Connect our own clicked signal to our own click handler
	QObject::connect(this, SIGNAL(clicked()), this, SLOT(handleClick()));
}

StartSweepButton::~StartSweepButton()
{

}

//Click handler, emit start sweep signal
void StartSweepButton::handleClick()
{
	//Do stuff
	emit startSweep();
}