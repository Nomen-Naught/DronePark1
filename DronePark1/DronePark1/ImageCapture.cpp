#include "ImageCapture.h"
#include <Windows.h>
#include "qimage.h"

ImageCapture::ImageCapture(QMutex* mutex)
{
	imageBufferMutex = mutex;

}

void ImageCapture::asyncCaptureStart()
{
	videoInput* vi = new videoInput();

	// Device index -- should be 0 if the digitizer is the only device
	int device = 0;

	vi->setupDevice(device, 720, 480, 0);

	int width = vi->getWidth(device);
	int height = vi->getHeight(device);
	int size = vi->getSize(device);

	unsigned char* buffer = new unsigned char[size];

	// grab a few frames, otherwise this won't work
	for (int i = 0; i < 5; i++)
	{
		if (vi->isFrameNew(device)) {
			vi->getPixels(device, buffer, false, false);
		}

		Sleep(500);
	}

	// TODO: Ben: provide an exit condition
	while (1)
	{
		while (!vi->isFrameNew(device))
		{
			Sleep(10); // spin until new frame
		}

		vi->getPixels(device, buffer, false, false);

		QImage* frame = convertToQImage(width, height, buffer);

		emit imageReady(frame);

	}

	vi->stopDevice(device);
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