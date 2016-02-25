#ifndef IMAGEPROCESSOR_H
#define IMAGEPROCESSOR_H

#include "QZXing.h"
#include "Magick++.h"
#include <qmutex.h>

class ImageProcessor : public QObject
{
	Q_OBJECT

public:
	ImageProcessor(QMutex* mutex);

	QString getQRCode(QImage *image);

	~ImageProcessor();

public slots:
	void handleImage(QImage* capturedImage);

signals:
	void qrCodeReady(QString code);

private:
	QZXing decoder;
	QMutex* imageBufferMutex;

	QImage* enhanceImage(QImage* preImage);

	Magick::Image* toImage(QImage *qImage);
	QImage* toQImage(Magick::Image *mImage);
};


#endif // IMAGEPROCESSOR_H