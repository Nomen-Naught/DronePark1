#pragma once
#include <string>

#define DP_ASSERT(condition, _what, _why) Q_ASSERT_X(condition, _what, _why);
#define DP_ASSERT(condition, _what) Q_ASSERT_X(condition, _what, "generic");

enum ReturnCode
{
	RC_OK,
	RC_ERR,
	RC_2DB_CONN_ATTEPTS
};