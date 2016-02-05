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
	QZXing decoder;

	QImage* enhanceImage(QImage* preImage);

	Magick::Image* toImage(QImage *qImage);
	QImage* toQImage(Magick::Image *mImage);
};


#endif // IMAGEPROCESSOR_H