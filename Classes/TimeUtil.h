#ifndef __TIMEUTILE_H
#define __TIMEUTILE_H
#include "cocos2d.h"
#include <time.h>
#include <wchar.h>

namespace time_util
{
	int getCurTime();
	struct tm *getDate(time_t time);
}

#endif