#include "ImageCapture.h"
#include <Windows.h>
#include "qimage.h"

ImageCapture::ImageCapture()
{
	videoInput *vi = new videoInput();
	int device = 0;

	vi->setupDevice(device, 720, 480, 0);

	int width = vi->getWidth(device);
	int height = vi->getHeight(device);
	int size = vi->getSize(device);

	unsigned char * buffer = new unsigned char[size];

	// TODO: remove this stuff from the constructor
	// grab a few frames, otherwise this won't work
	for (int i = 0; i < 5; i++)
	{
		if (vi->isFrameNew(device)) {
			vi->getPixels(device, buffer, false, false);
		}

		Sleep(500);
	}

	// The buffer is reversed, so image must go from bottom
	//  to top and the byte order is now BGR.
	// Could reverse the whole array and proceed normally, but
	//  that would take twice as long.
	QImage frame(width, height, QImage::Format_RGB888);
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			int i = (x + (y*width)) * 3;
			QRgb value = qRgb(buffer[i + 2], buffer[i + 1], buffer[i]); // reversed byte order
			frame.setPixel(x, height - y - 1, value); // bottom to top
		}
	}
	
	bool rc = frame.save("C:/Users/Ben/Documents/frame.png");

	if (!rc)
		qDebug() << "Error saving file";

	vi->stopDevice(device);
}