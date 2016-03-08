#pragma once
#include <QLabel>
#include "StatusLabel.h"

StatusLabel::StatusLabel(QWidget *parent)
	: QLabel(parent)
{

}

StatusLabel::~StatusLabel()
{

}

void StatusLabel::updateStatus(bool _newStatus)
{
	if (_newStatus == true)
	{
		setText("IN THE AIR");

		setStyleSheet("QLabel { background-color : red; color : white; }");



	}
	else
	{
		setText("NOT FLYING");
		setStyleSheet("QLabel { background-color : rgb(53, 53, 53); color : red; }");
	}
	
}