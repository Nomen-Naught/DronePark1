#pragma once
#include <QGridLayout>
#ifndef MY_GRID_LAYOUT_H
#define MY_GRID_LAYOUT_H
#endif
#include "ParkingLot.h"

class ParkingLotLayout : public QGridLayout
{
	Q_OBJECT

public:
	ParkingLotLayout(int _row, int _col, QWidget *parent = 0);
	~ParkingLotLayout();

	void add_widget(QWidget* p_widget);

private:
	int row;
	int col;

};