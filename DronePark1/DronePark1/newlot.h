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

private:
	Ui::NewLot ui;

public slots:
	void newLotOk();

signals:
	void newLotOkSig(int numSpots, int rows, int col);
};

#endif // !NEWLOT_H
