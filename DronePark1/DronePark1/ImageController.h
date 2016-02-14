#pragma once

class ImageCommsController
{
private:

	//Generic Connection object used to store connection information relating to the camera
	int connection;

public:

	//Dispatch a new thread to receive an image from the camera. Returns the image received.
	int asyncRecieveImage();

	//Initialize the camera connection
	int connectToCamera();

};

class ImageProcessController
{
public:

	//Given an image, attempt to identify a stub and return its id.
	int processImage();
};