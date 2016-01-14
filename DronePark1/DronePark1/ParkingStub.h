#pragma once
#include "SubjectObserver.h"
#include "ReturnCodes.h"
#include "ParkingLot.h"
#include <list>
#include <QDateTime>

class Stub : DbObject
{
private:
	// Time at which a stub expires
	QDateTime expireTime;

	// Time that Stub was purchased
	QDateTime purchaseTime;

	// Associated pointer to Spot object
	Spot* spot;

	// Should be set to false if current time is past expireTime, True otherwise
	bool valid;

public:
	// Constructor, should call DbObject constructor
	Stub() : DbObject()
	{
		spot = NULL;
	}

	// Returns the expireTime
	QDateTime getExpireTime()
	{
		return expireTime;
	}

	// Returns the valid flags
	bool getValid()
	{
		return valid;
	}

	// Returns the purchaseTime
	QDateTime getPurchaseTime()
	{
		return purchaseTime;
	}

	// Sets the expireTime, returns RC
	int setExpireTime(QDateTime _expireTime)
	{
		expireTime = _expireTime;
		return RC_OK;
	}

	// Sets the valid flag, returns RC
	int setValid(bool _valid)
	{
		valid = _valid;
		return RC_OK;
	}

	// Sets the purchaseTime, returns RC
	int setPurchaseTime(QDateTime _purchaseTime)
	{
		purchaseTime = _purchaseTime;
		return RC_OK;
	}

};