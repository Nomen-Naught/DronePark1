#include "newlot.h"
#include "DatabaseController.h"
#include "QMessageBox.h"

NewLot::NewLot(QWidget *parent) : QDialog(parent)
{
	ui.setupUi(this);
	connect(ui.numCol, SIGNAL(valueChanged(int)), this, SLOT(setColSlideValue(int)));
	connect(ui.numRow, SIGNAL(valueChanged(int)), this, SLOT(setRowSlideValue(int)));
}

void NewLot::accept()
{
	//grab values from the new lot ui
	int row = ui.numRow->value();
	int col = ui.numCol->value();
	int numSpot = row * col;
	QString lotName = ui.lotText->text();
	lotName = lotName.trimmed();
	QString city = ui.cityText->text();
	city = city.trimmed();
	
	// make sure city and lot input are not blank
	if (lotName.isEmpty() || city.isEmpty())
	{
		QMessageBox msgBox;
		msgBox.setText("City and Lot Name Cannot be left blank.");
		msgBox.exec();
	}
	else
	{
		//emit signal to insert to db
		emit newLotOkSig(numSpot, row, col, lotName, city);

		QDialog::accept();
	}
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