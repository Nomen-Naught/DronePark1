#include "dronepark1.h"
#include <QtWidgets/QApplication>
#include "Model.h"
#include "Controllers.h"
#include "Tests_Misc.h"
#include <QStyleFactory>

int main(int argc, char *argv[])
{
	//Declarations--------------

	int rc = RC_OK;								//Return code

	DroneParkController droneParkController;	//Main Program Controller, stays in scope of main function for now... might need to be moved to somewhere more available to Qt events

	//End Delcarations----------

	// Launches Qt window
	QApplication a(argc, argv);
	a.setStyle(QStyleFactory::create("Fusion"));

	QPalette darkPalette;
	darkPalette.setColor(QPalette::Window, QColor(53, 53, 53));
	darkPalette.setColor(QPalette::WindowText, Qt::white);
	darkPalette.setColor(QPalette::Base, QColor(25, 25, 25));
	darkPalette.setColor(QPalette::AlternateBase, QColor(53, 53, 53));
	darkPalette.setColor(QPalette::ToolTipBase, Qt::white);
	darkPalette.setColor(QPalette::ToolTipText, Qt::white);
	darkPalette.setColor(QPalette::Text, Qt::white);
	darkPalette.setColor(QPalette::Button, QColor(53, 53, 53));
	darkPalette.setColor(QPalette::ButtonText, Qt::white);
	darkPalette.setColor(QPalette::BrightText, Qt::red);
	darkPalette.setColor(QPalette::Link, QColor(42, 130, 218));

	darkPalette.setColor(QPalette::Highlight, QColor(42, 130, 218));
	darkPalette.setColor(QPalette::HighlightedText, Qt::black);

	a.setPalette(darkPalette);

	a.setStyleSheet("QToolTip { color: #ffffff; background-color: #2a82da; border: 1px solid white; }");

	DronePark1 gui;
	gui.show();

	// Program Start use-case BEGIN***

	//Should instantiate a database controller for this session AND do everything needed to load first page!!
	rc = droneParkController.initialize(&gui);
	DP_ASSERT(rc, "droneParkController.initialize()");


	// Program Start use-case END***

	return a.exec();
}
