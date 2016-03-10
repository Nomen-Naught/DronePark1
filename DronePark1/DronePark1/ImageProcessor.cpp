#include "ImageProcessor.h"
#include "zxing\Exception.h"

ImageProcessor::ImageProcessor(QMutex* _mutex)
{
	mutex = _mutex;
	decoder.setDecoder(decoder.DecoderFormat_QR_CODE);
}

QString ImageProcessor::getQRCode(QImage* image)
{
	QString code = "";

	if (TRY_BEFORE_ENHANCE)
	{
		try
		{
			code = decoder.decodeImage(*image);
		}
		catch (zxing::Exception e)
		{
			code = "";
		}

	}

	if (code.length() <= 1 && TRY_ENHANCE)
	{
		QImage* newImage = enhanceImage(image);
		
		try
		{
			code = decoder.decodeImage(*newImage);
		}
		catch (zxing::Exception e)
		{
			code = "";
		}

		delete newImage;
	}

	return code;
}

QImage* ImageProcessor::enhanceImage(QImage* preImage)
{
	Magick::Image* mImage = toImage(preImage);

	/////////////////////  De-interlace / Downsample  ///////////////////////
	//                                                                     //
	// The raw frames are badly interlaced when there is motion.           //
	// At full res there isn't much detail so little to no detail is lost. //
	// Test performance is WAY better after downsampling.                  //
	/////////////////////////////////////////////////////////////////////////

	//mImage->sample(Magick::Geometry("50%x50%"));

	//////////////////////////////  Sharpen  ////////////////////////////////
	//                                                                     //
	// Sharpens soft edges in the image into harder edges.                 //
	// This is particularly important for QR codes, especially since       //
	// QZXing does not remotely like soft edges.                           //
	/////////////////////////////////////////////////////////////////////////

	mImage->sharpen(2.0, 2.0);

	/////////////////////////////  Threshold  ///////////////////////////////
	//                                                                     //
	// Binarize the image so that each pixel is either black or white.     //
	// Some testing shows that this might not actually be beneficial,      //
	// since it can make the QR edges jagged.                              //
	/////////////////////////////////////////////////////////////////////////

	//mImage->threshold(160.0);

	QImage* newImage = toQImage(mImage);

	delete mImage;

	return newImage;
}

/* Convert from Magick::Image to QImage */
QImage* ImageProcessor::toQImage(Magick::Image *mImage)
{
	QImage* newQImage = new QImage(mImage->columns(), mImage->rows(), QImage::Format_RGB32);
	const Magick::PixelPacket* pixels;
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

/* Convert from QImage to Magick::Image */
Magick::Image* ImageProcessor::toImage(QImage *qImage)
{
	Magick::Image* mImage = new Magick::Image(Magick::Geometry(qImage->width(),
				qImage->height()), Magick::ColorRGB(0.0, 0.0, 0.0));

	double scale = 1 / 256.0;

	// Not sure why this is necessary but it seems important
	mImage->modifyImage();

	Magick::PixelPacket *pixels;
	Magick::ColorRGB mgc;

	// Taking the values pixel by pixel... sounds slow but it works
	for (int y = 0; y < qImage->height(); y++)
	{
		pixels = mImage->setPixels(0, y, mImage->columns(), 1);

		for (int x = 0; x < qImage->width(); x++)
		{
			QColor pix(qImage->pixel(x, y));

			mgc.red(scale * pix.red());
			mgc.green(scale * pix.green());
			mgc.blue(scale * pix.blue());

			*pixels++ = mgc;
		}

		mImage->syncPixels();
	}

	return mImage;
}

/* Take a pointer to a QImage potentially containing a QR code *
 * and extract it.                                             */
void ImageProcessor::handleImage(QImage* capturedImage)
{
	// This should prevent a processing queue from forming
	// A queue would be bad because we can only process in real time
	if (processing)
	{
		return;
	}
	processing = true;

	mutex->lock();

	QString code = getQRCode(capturedImage);

	emit qrCodeReady(code);

	delete capturedImage;
	processing = false;

	mutex->unlock();
}

void ImageProcessor::stopProcess()
{

}

ImageProcessor::~ImageProcessor()
{

}