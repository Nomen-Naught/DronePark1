#pragma once
#include <QObject>


//This class is designed to communicate between the hosted python script and the Qt application
class ControlInterface : public QObject {
	Q_OBJECT

public:
	ControlInterface();
	~ControlInterface();

signals:
	//Test signal for python calling
	void sigTest();

	// A signal saying connection to the drone failed
	void connFail();

	// A signal saying the drone is at the next spot
	void atNextSpot();
};