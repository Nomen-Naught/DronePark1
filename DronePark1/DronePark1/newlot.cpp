#include "newlot.h"
#include "DatabaseController.h"

NewLot::NewLot(QWidget *parent) : QDialog(parent)
{
	ui.setupUi(this);
	connect(ui.numCol, SIGNAL(valueChanged(int)), this, SLOT(setColSlideValue(int)));
	connect(ui.numRow, SIGNAL(valueChanged(int)), this, SLOT(setColSlideValue(int)));
}

void NewLot::accept()
{
	
	int row = ui.numRow->value();
	int col = ui.numCol->value();
	int numSpot = row * col;
	QString lotName = ui.lotText->text();
	QString city = ui.cityText->text();
	emit newLotOkSig(numSpot, row, col,lotName, city);

	QDialog::accept();

	return;
}

void NewLot::setColSlideValue(int value)
{
	ui.numColLabel->setText(QString::number(value));
}

void NewLot::setRowSlideValue(int value)
{
	ui.numRowLabel->setText(QString::number(value));
}

NewLot::~NewLot()
{

}