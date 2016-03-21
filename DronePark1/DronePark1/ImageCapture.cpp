#include "ImageCapture.h"
#include <Windows.h>
#include "qimage.h"
#include "qdebug.h"

#define DEVICE 1

ImageCapture::ImageCapture(QMutex* _mutex, bool* _captureLoop)
{
	mutex = _mutex;
	captureLoop = _captureLoop;
}

void ImageCapture::asyncCaptureStart()
{


	videoInput* vi = new videoInput();


	int numDevices = vi->listDevices();

	vi->setupDevice(DEVICE, 720, 480, 0);

	int width = vi->getWidth(DEVICE);
	int height = vi->getHeight(DEVICE);
	int size = vi->getSize(DEVICE);

	unsigned char* buffer = new unsigned char[size];

	// grab a few frames, otherwise this won't work
	for (int i = 0; i < 5; i++)
	{

		if (vi->isFrameNew(DEVICE)) {
			vi->getPixels(DEVICE, buffer, false, false);
		}

		Sleep(500);
	}
	
	// This will stay true until set false by the slot stopCapture()
	//captureLoop = true;

	while (*captureLoop)
	{
		
		while (!vi->isFrameNew(DEVICE))
		{
			Sleep(10); // spin until new frame
		}

		vi->getPixels(DEVICE, buffer, false, false);

		QImage* frame = convertToQImage(width, height, buffer);

		mutex->lock();
		emit imageReady(frame);
		mutex->unlock();

	}

	vi->stopDevice(DEVICE);
}

void ImageCapture::stopCapture()
{
	//captureLoop = false;
	qDebug() << "captureLoop value now:" << captureLoop;
}

QImage* ImageCapture::convertToQImage(int width, int height, unsigned char* buffer)
{

	// The buffer is reversed, so image must go from bottom
	//    to top and the byte order is now BGR.
	// Could reverse the whole array and proceed normally, but
	//    that would take twice as long.
	QImage* frame = new QImage(width, height, QImage::Format_RGB888);

	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			int i = (x + (y*width)) * 3;
			QRgb value = qRgb(buffer[i + 2], buffer[i + 1], buffer[i]); // reversed byte order
			frame->setPixel(x, height - y - 1, value); // bottom to top
		}
	}

	return frame;
}