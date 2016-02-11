#include "ParkingLotLayout.h"
#include <QMessageBox>

ParkingLotLayout::ParkingLotLayout(int _row, int _col, QWidget *parent)
	: QGridLayout(parent)
{
	row = _row;
	col = _col;

	// Load the spot graphic
	spotImage.load("parkingSpot.png");

	// Store spot graphic and mirrored version in QPixmap
	spotPixMap = QPixmap::fromImage(spotImage);
	spotPixMap_mirror = QPixmap::fromImage(spotImage.mirrored(true, false));

	// Scale the image to a better size
	spotPixMap = spotPixMap.scaled(QSize(100, 200), Qt::KeepAspectRatio);
	spotPixMap_mirror = spotPixMap_mirror.scaled(QSize(100, 200), Qt::KeepAspectRatio);

	// Store the images in an icon format
	spotIcon = new QIcon(spotPixMap);
	spotIcon_mirror = new QIcon(spotPixMap_mirror);
}

ParkingLotLayout::~ParkingLotLayout()
{

}

void ParkingLotLayout::add_spot(SpotButton* spotButton)
{
	// Declaractions
	int current_row = 0;
	int current_column = 0;

	// Constructs the spot matrix based on the rows and columns
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


	// Set the icon for the spot, set a mirrored one if it's an odd column
	if (current_column % 2 != 0)
	{
		spotButton->setIcon(*spotIcon_mirror);
		spotButton->setIconSize(spotPixMap_mirror.rect().size());
		spotButton->setFixedSize(QSize(spotPixMap_mirror.rect().width() * (1.2), spotPixMap_mirror.rect().height() * (1.4)));
	}
	else
	{
		spotButton->setIcon(*spotIcon);
		spotButton->setIconSize(spotPixMap.rect().size());
		spotButton->setFixedSize(QSize(spotPixMap.rect().width() * (1.2), spotPixMap.rect().height() * (1.4)));
	}

	QGridLayout::addWidget(spotButton, current_row, current_column);
}