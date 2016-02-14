#pragma once
#include <QDateTime>

class DecideSpotController
{
private:
	//A helper method which returns the current time.
	QDateTime getCurrentTime();

public:
	/*Dispatch a new thread to decide the validity of the stub
		being examined.The parameter passed in is the id
		recovered from the image processing, to be matched to
		its corresponding Stub object.Returns a Boolean which
		is the value of the validity of the stub*/
	bool asyncDecideStubValidity(int id);
};