#include "newlot.h"
#include "DatabaseController.h"
#include "QMessageBox.h"

//create ui for new lot 
NewLot::NewLot(QWidget *parent) : QDialog(parent)
{
	ui.setupUi(this);

	//connect the sliders to signals that update labels based on slider position
	connect(ui.numCol, SIGNAL(valueChanged(int)), this, SLOT(setColSlideValue(int)));
	connect(ui.numRow, SIGNAL(valueChanged(int)), this, SLOT(setRowSlideValue(int)));
}

//overwrite interfaces accept() function to grab ui values and emit signal if ok
void NewLot::accept()
{
	//grab values from the new lot ui
	int row = ui.numRow->value();
	int col = ui.numCol->value();
	int numSpot = row * col;
	QString lotName = ui.lotText->text();
	QString city = ui.cityText->text();

	//Remove any leading or trailing whitespace
	lotName = lotName.trimmed();
	city = city.trimmed();
	
	// make sure city and lot input are not blank
	if (lotName.isEmpty() || city.isEmpty())
	{
		//if either are blank display error message and do not submit to db
		QMessageBox msgBox;
		msgBox.setText("City and Lot Name Cannot be left blank.");
		msgBox.exec();
	}
	else
	{
		//emit signal to insert to db
		emit newLotOkSig(numSpot, row, col, lotName, city);
		
		//call super for accept()
		QDialog::accept();
	}
	return;
}

//slot that updates column label based on virticle slider
void NewLot::setColSlideValue(int value)
{
	ui.numColLabel->setText(QString::number(value));
}

//function that updates Row label based on horizontal slider
void NewLot::setRowSlideValue(int value)
{
	ui.numRowLabel->setText(QString::number(value));
}

NewLot::~NewLot()
{

}