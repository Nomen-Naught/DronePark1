#include "ImageProcessor.h"

ImageProcessor::ImageProcessor(QMutex* mutex)
{
	imageBufferMutex = mutex;

	decoder.setDecoder(decoder.DecoderFormat_QR_CODE);
}

QString ImageProcessor::getQRCode(QImage* image)
{
	QString code = decoder.decodeImage(*image);

	// Try sharpening if the first try doesn't find a QR code
	if (code.length() <= 1)
	{
		qDebug() << "No QR code found. Attempting to improve image...";
		QImage *newImage = enhanceImage(image);
		code = decoder.decodeImage(*newImage);

		delete newImage;
	}

	return code;
}

QImage* ImageProcessor::enhanceImage(QImage* preImage)
{
	Magick::Image *mImage = toImage(preImage);

	mImage->sharpen(2.0, 1.0);
	QImage *newImage = toQImage(mImage);

	return newImage;
}

QImage* ImageProcessor::toQImage(Magick::Image *mImage)
{
	QImage *newQImage = new QImage(mImage->columns(), mImage->rows(), QImage::Format_RGB32);
	const Magick::PixelPacket *pixels;
	Magick::ColorRGB rgb;
	for (int y = 0; y < newQImage->height(); y++) {
		pixels = mImage->getConstPixels(0, y, newQImage->width(), 1);
		for (int x = 0; x < newQImage->width(); x++) {
			rgb = (*(pixels + x));
			newQImage->setPixel(x, y, QColor((int)(255 * rgb.red())
				, (int)(255 * rgb.green())
				, (int)(255 * rgb.blue())).rgb());
		}
	}
	return newQImage;
}

Magick::Image* ImageProcessor::toImage(QImage *qImage)
{
	Magick::Image *mImage = new Magick::Image(Magick::Geometry(qImage->width(),
				qImage->height()), Magick::ColorRGB(0.0, 0.0, 0.0));

	double scale = 1 / 256.0;
	mImage->modifyImage();

	Magick::PixelPacket *pixels;
	Magick::ColorRGB mgc;
	for (int y = 0; y < qImage->height(); y++)
	{
		pixels = mImage->setPixels(0, y, mImage->columns(), 1);
		for (int x = 0; x < qImage->width(); x++)
		{
			QColor pix(qImage->pixel(x, y));

			mgc.red(scale * pix.red());
			mgc.green(scale *pix.green());
			mgc.blue(scale *pix.blue());

			*pixels++ = mgc;
		}
		mImage->syncPixels();
	}

	return mImage;
}

void ImageProcessor::handleImage(QImage* capturedImage)
{
	QString code = getQRCode(capturedImage);

	emit qrCodeReady(code);

	delete capturedImage;
}

ImageProcessor::~ImageProcessor()
{

}