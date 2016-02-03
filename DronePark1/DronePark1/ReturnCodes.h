#pragma once
#include <qstring.h>
#include <map>

#define DP_ASSERT2(condition, _what, _why) Q_ASSERT_X(condition == RC_OK, _what, _why);
#define DP_ASSERT(condition, _what) Q_ASSERT_X(condition == RC_OK, _what, "generic");
enum ReturnCode
{
	RC_OK = 0,
	RC_ERR,
	RC_DB_CONN_FAIL,
	RC_2DB_CONN_ATTEMPTS
};

const std::map<int, QString> RC_MAP = 
{ 
	{ RC_OK, "RC_OK" },
	{ RC_ERR, "RC_ERR" },
	{ RC_DB_CONN_FAIL, "RC_DB_CONN_FAIL" },
	{ RC_2DB_CONN_ATTEMPTS, "RC_2DB_CONN_ATTEMPTS" }
};