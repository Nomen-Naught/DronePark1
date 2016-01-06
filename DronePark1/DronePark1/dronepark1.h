#ifndef DRONEPARK1_H
#define DRONEPARK1_H

#include <QtWidgets/QMainWindow>
#include "ui_dronepark1.h"

class DronePark1 : public QMainWindow
{
	Q_OBJECT

public:
	DronePark1(QWidget *parent = 0);
	~DronePark1();

private:
	Ui::DronePark1Class ui;
};

#endif // DRONEPARK1_H
