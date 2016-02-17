#ifndef IMAGECAPTURE_H
#define IMAGECAPTURE_H

#include "opencv\cv.h"
#include "opencv2\opencv.hpp"
#include "opencv2\core\core.hpp"
#include "VideoInput\videoInput.h"
#include "qdebug.h"
#include <fstream>
#include <iostream>
using std::ofstream;

class ImageCapture
{
public:
	ImageCapture();
	CvCapture* getCapture();
	CvCapture *capture;
	IplImage *frame;
private:
	

};

#endif // IMAGECAPTURE_H