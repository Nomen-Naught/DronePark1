#ifndef IMAGECAPTURE_H
#define IMAGECAPTURE_H

//#include "opencv\cv.h"
//#include "opencv2\opencv.hpp"
//#include "opencv2\core\core.hpp"
#include "VideoInput\videoInput.h"
#include "qdebug.h"
#include <fstream>
#include <iostream>
#include <qmutex.h>

using std::ofstream;

class ImageCapture : public QObject
{
	Q_OBJECT

public:
	ImageCapture();

public slots:
	void asyncCaptureStart();
	void stopCapture();

signals:
	void imageReady(QImage* capturedImage);

private:
	bool captureLoop = false;
	//CvCapture* capture;
	//IplImage* frame;

	QImage* convertToQImage(int width, int height, unsigned char* buffer);
};

#endif // IMAGECAPTURE_H