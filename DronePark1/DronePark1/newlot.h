#pragma once
#ifndef NEWLOT_H
#define NEWLOT_H

#include <QtWidgets/QDialog>
#include "ui_newlot.h"

class NewLot : public QDialog
{
	Q_OBJECT

public:
	NewLot(QWidget *parent = 0);
	~NewLot();

	//Gets called when you accept the form
	void accept();

signals:
	void newLotOkSig(int numSpots, int rows, int col, QString lotName, QString _city);

private:
	Ui::NewLot ui;


};

#endif // !NEWLOT_H
