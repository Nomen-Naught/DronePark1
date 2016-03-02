#pragma once
#include <QDateTime>
#include <QObject>

class DecideSpotController : public QObject
{
private:
	//A helper method which returns the current time.
	QDateTime getCurrentTime();

public slots:
	void decideStubValidity(int id);

signals:
	void spotValid(int spot_id, int stub_id);
	void spotInvalid(int spot_id, int stub_id);
	void spotUnknown(int spot_id);
};