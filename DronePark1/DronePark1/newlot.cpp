#include "newlot.h"
#include "DatabaseController.h"

NewLot::NewLot(QWidget *parent) : QDialog(parent)
{
	ui.setupUi(this);

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

NewLot::~NewLot()
{

}