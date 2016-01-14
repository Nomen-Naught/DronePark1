#include "dronepark1.h"
#include <QtWidgets/QApplication>
#include "Model.h"


int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	DronePark1 w;
	w.show();

	return a.exec();
}
