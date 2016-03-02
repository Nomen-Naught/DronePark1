#include "DecideController.h"
#include "ReturnCodes.h"

//TODO: Nick: implement getCurrentTime
//A helper method which returns the current time.
QDateTime DecideSpotController::getCurrentTime()
{
	return QDateTime();
}

//TODO: Nick: implement asyncDecideStubValidity
/*Dispatch a new thread to decide the validity of the stub
being examined.The parameter passed in is the id
recovered from the image processing, to be matched to
its corresponding Stub object.Returns a Boolean which
is the value of the validity of the stub*/
void DecideSpotController::decideStubValidity(int id)
{
	return;
}