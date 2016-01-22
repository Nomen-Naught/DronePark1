#include "ImageController.h"
#include "ReturnCodes.h"

//TODO: Nick: implement asyncRecieveImage
//Dispatch a new thread to receive an image from the camera. Returns the image received.
int ImageCommsController::asyncRecieveImage()
{
	return RC_ERR;
}

//TODO: Nick: implement connectToCamera
//Initialize the camera connection
int ImageCommsController::connectToCamera()
{
	return RC_ERR;
}

//TODO: Nick: implement processImage
//Given an image, attempt to identify a stub and return its id.
int ImageProcessController::processImage()
{
	return RC_ERR;
}