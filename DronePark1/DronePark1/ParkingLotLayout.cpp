#include "ParkingLotLayout.h"
#include <QMessageBox>

ParkingLotLayout::ParkingLotLayout(int _row, int _col, QWidget *parent)
	: QGridLayout(parent)
{
	row = _row;
	col = _col;
}

ParkingLotLayout::~ParkingLotLayout()
{

}

void ParkingLotLayout::add_widget(QWidget* p_widget)
{
	int current_row = 0;
	int current_column = 0;

	while (itemAtPosition(current_row, current_column) != 0)
	{
		if (current_column == (col - 1))
		{
			current_column = 0;
			++current_row;
		}
		else
		{
			++current_column;
		}
	}

	QGridLayout::addWidget(p_widget, current_row, current_column);
}