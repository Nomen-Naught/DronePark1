#include "SpotButton.h"

SpotButton::SpotButton(QWidget *parent)
	: QPushButton(parent)
{
	currentTicketed = false;
	currentEmpty = false;
}

SpotButton::~SpotButton()
{

}

void SpotButton::updateTicketed(int id, bool value)
{
	currentTicketed = value;
	updateText();
}

void SpotButton::updateEmpty(int id, bool value)
{
	currentEmpty = value;
	updateText();
}

void SpotButton::updateText()
{
	QString text = "Empty: " + QString::number(currentEmpty) + " Ticket: " + QString::number(currentTicketed);
	setText(text);
}