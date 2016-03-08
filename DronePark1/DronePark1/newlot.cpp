#include "newlot.h"

NewLot::NewLot(QWidget *parent) : QDialog(parent)
{
	ui.setupUi(this);

	connect(ui.buttonBox, SIGNAL(accepted()), this, SLOT(newLotOk()));

}

void NewLot::newLotOk()
{
	emit newLotOkSig(1, 1, 1);
}

NewLot::~NewLot()
{

}