#include "ParkingLotLayout.h"
#include <QMessageBox>
#include "SpotButton.h"

ParkingLotLayout::ParkingLotLayout(int _row, int _col, QWidget *parent)
	: QGridLayout(parent)
{
	row = _row;
	col = _col;

	// Holds image for empty spots
	QImage emptyImage;
	QPixmap emptyPixMap;
	QPixmap emptyPixMap_mirror;
	QIcon* emptyIcon;
	QIcon* emptyIcon_mirror;

	// Holds image for occupied spots
	QImage occupiedImage;
	QPixmap occupiedPixMap;
	QPixmap occupiedPixMap_mirror;
	QIcon* occupiedIcon;
	QIcon* occupiedIcon_mirror;

	// Holds image for illegal spots
	QImage illegalImage;
	QPixmap illegalPixMap;
	QPixmap illegalPixMap_mirror;
	QIcon* illegalIcon;
	QIcon* illegalIcon_mirror;


	//EMPTY---------------------------------------------------------------------
	// Load the spot graphic
	emptyImage.load("parkingSpot.png");

	// Store spot graphic and mirrored version in QPixmap
	emptyPixMap = QPixmap::fromImage(emptyImage);
	emptyPixMap_mirror = QPixmap::fromImage(emptyImage.mirrored(true, false));

	// Scale the image to a better size
	emptyPixMap = emptyPixMap.scaled(QSize(100, 200), Qt::KeepAspectRatio);
	emptyPixMap_mirror = emptyPixMap_mirror.scaled(QSize(100, 200), Qt::KeepAspectRatio);

	// Store the images in an icon format
	emptyIcon = new QIcon(emptyPixMap);
	emptyIcon_mirror = new QIcon(emptyPixMap_mirror);

	//OCCUPIED---------------------------------------------------------------------
	// Load the spot graphic
	occupiedImage.load("parkingSpot_occupied.png");

	// Store spot graphic and mirrored version in QPixmap
	occupiedPixMap = QPixmap::fromImage(occupiedImage);
	occupiedPixMap_mirror = QPixmap::fromImage(occupiedImage.mirrored(true, false));

	// Scale the image to a better size
	occupiedPixMap = occupiedPixMap.scaled(QSize(100, 200), Qt::KeepAspectRatio);
	occupiedPixMap_mirror = occupiedPixMap_mirror.scaled(QSize(100, 200), Qt::KeepAspectRatio);

	// Store the images in an icon format
	occupiedIcon = new QIcon(occupiedPixMap);
	occupiedIcon_mirror = new QIcon(occupiedPixMap_mirror);

	//ILLEGAL---------------------------------------------------------------------
	// Load the spot graphic
	illegalImage.load("parkingSpot_illegal.png");

	// Store spot graphic and mirrored version in QPixmap
	illegalPixMap = QPixmap::fromImage(illegalImage);
	illegalPixMap_mirror = QPixmap::fromImage(illegalImage.mirrored(true, false));

	// Scale the image to a better size
	illegalPixMap = illegalPixMap.scaled(QSize(100, 200), Qt::KeepAspectRatio);
	illegalPixMap_mirror = illegalPixMap_mirror.scaled(QSize(100, 200), Qt::KeepAspectRatio);

	// Store the images in an icon format
	illegalIcon = new QIcon(illegalPixMap);
	illegalIcon_mirror = new QIcon(illegalPixMap_mirror);

	SpotButton::setIcons(emptyIcon, emptyIcon_mirror, occupiedIcon, occupiedIcon_mirror, illegalIcon, illegalIcon_mirror);

	//Grab our icon size
	iconSize = new QSize(emptyPixMap.rect().size());

	//Figure out how big we wanna draw rectablges in buttons, for denoting which spot the copter should be over
	int thickness = 5;

	int b_width = iconSize->width() * (1.2) - 1;
	int b_height = iconSize->height() * (1.4) - 1;

	QRect rect1(0, 0, thickness, b_height);
	QRect rect2(0, 0, b_width, thickness);
	QRect rect3(b_width - thickness, 0, thickness, b_height);
	QRect rect4(0, b_height - thickness, b_width, thickness);

	SpotButton::setRect(rect1, rect2, rect3, rect4);

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
	// This constructs a rectangular lot with indexes incrementing in an S shape.
	// The S shape makes iterating over them more like how a flight path would actually be
	while (itemAtPosition(current_row, current_column) != 0)
	{
		if (current_column % 2 == 0)
		{
			if (current_row == (row - 1))
			{
				++current_column;
			}
			else
			{
				++current_row;
			}
		}
		else
		{
			if (current_row == 0)
			{
				++current_column;

			}
			else
			{
				--current_row;
			}
		}
	}


	// Set the icon for the spot, set a mirrored one if it's an odd column
	if (current_column % 2 != 0)
	{
		spotButton->setIcon(*SpotButton::empty_mirror);
		spotButton->setIconSize(*iconSize);
		spotButton->setFixedSize(QSize(iconSize->width() * (1.2), iconSize->height() * (1.4)));
		spotButton->setMirrored();
	}
	else
	{
		spotButton->setIcon(*SpotButton::empty);
		spotButton->setIconSize(*iconSize);
		spotButton->setFixedSize(QSize(iconSize->width() * (1.2), iconSize->height() * (1.4)));
	}

	QGridLayout::addWidget(spotButton, current_row, current_column);
}