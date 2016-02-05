#include "ImageProcessor.h"

ImageProcessor::ImageProcessor()
{
	decoder.setDecoder(decoder.DecoderFormat_QR_CODE);
}

QString ImageProcessor::getQRCode(QImage *image)
{
	QTime timer;

	timer.start();
	Magick::Image *mImage = toImage(image);
	int ms = timer.elapsed();
	qDebug() << "Convert to Image took " << ms << " milliseconds";
	
	timer.restart();
	mImage->sharpen(2.0, 1.0);
	ms = timer.elapsed();
	qDebug() << "Sharpen took " << ms << " milliseconds";

	timer.restart();
	QImage *newImage = toQImage(mImage);
	ms = timer.elapsed();
	qDebug() << "Convert to QImage took " << ms << " milliseconds";

	timer.restart();
	QString code = decoder.decodeImage(*newImage);
	ms = timer.elapsed();
	qDebug() << "QR decode took " << ms << " milliseconds";

	return code;
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

ImageProcessor::~ImageProcessor()
{

}