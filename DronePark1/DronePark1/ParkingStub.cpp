#include "ParkingStub.h"

// Constructor, should call DbObject constructor
Stub::Stub() : DbObject()
{
	spot = NULL;
}

// Returns the expireTime
QDateTime Stub::getExpireTime()
{
	return expireTime;
}

// Returns the valid flags
bool Stub::getValid()
{
	return valid;
}

// Returns the purchaseTime
QDateTime Stub::getPurchaseTime()
{
	return purchaseTime;
}

// Sets the expireTime, returns RC
int Stub::setExpireTime(QDateTime _expireTime)
{
	expireTime = _expireTime;
	return RC_OK;
}

// Sets the valid flag, returns RC
int Stub::setValid(bool _valid)
{
	valid = _valid;
	return RC_OK;
}

// Sets the purchaseTime, returns RC
int Stub::setPurchaseTime(QDateTime _purchaseTime)
{
	purchaseTime = _purchaseTime;
	return RC_OK;
}