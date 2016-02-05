#ifndef IMAGEPROCESSOR_H
#define IMAGEPROCESSOR_H

#include "QZXing.h"
#include "Magick++.h"

class ImageProcessor
{
public:
	ImageProcessor();

	QString getQRCode(QImage *image);

	~ImageProcessor();

private:
	QImage enhanceImage(QImage preImage);

	QZXing decoder;

	Magick::Image* toImage(QImage *qImage);
	QImage* toQImage(Magick::Image *mImage);
};


#endif // IMAGEPROCESSOR_H