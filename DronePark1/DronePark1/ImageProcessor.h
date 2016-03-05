#ifndef IMAGEPROCESSOR_H
#define IMAGEPROCESSOR_H

#include "QZXing.h"
#include "Magick++.h"
#include <qmutex.h>

class ImageProcessor : public QObject
{
	Q_OBJECT

public:
	ImageProcessor();

	QString getQRCode(QImage *image);

	~ImageProcessor();

public slots:
	void handleImage(QImage* capturedImage);

signals:
	void qrCodeReady(QString code);

private:
	const bool TRY_ENHANCE = true;
	const bool TRY_BEFORE_ENHANCE = false;
	bool processing = false;

	QZXing decoder;

	QImage* enhanceImage(QImage* preImage);

	Magick::Image* toImage(QImage *qImage);
	QImage* toQImage(Magick::Image *mImage);
};


#endif // IMAGEPROCESSOR_H