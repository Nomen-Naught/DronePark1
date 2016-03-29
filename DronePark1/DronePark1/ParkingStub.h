#pragma once
#include "DBObject.h"
#include "ReturnCodes.h"
#include "ParkingLot.h"
#include <list>
#include <QDateTime>

class Stub : public DbObject
{
private:
	// Time at which a stub expires
	QDateTime* expireTime;

	// Time that Stub was purchased
	QDateTime* purchaseTime;

	// Associated pointer to Spot object
	Spot* spot;

	// Should be set to false if current time is past expireTime, True otherwise
	bool valid;

public:
	// Constructor, should call DbObject constructor
	Stub();

	// Returns the expireTime
	QDateTime* getExpireTime();

	// Returns the valid flags
	bool getValid();

	// Returns the purchaseTime
	QDateTime* getPurchaseTime();

	// Sets the expireTime, returns RC
	int setExpireTime(QDateTime* _expireTime);

	// Sets the valid flag, returns RC
	int setValid(bool _valid);

	// Sets the purchaseTime, returns RC
	int setPurchaseTime(QDateTime* _purchaseTime);

};