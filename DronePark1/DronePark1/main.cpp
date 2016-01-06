#include "dronepark1.h"
#include <QtWidgets/QApplication>

//Hello World

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	DronePark1 w;
	w.show();
	return a.exec();
}
