#include "TimeUtil.h"
USING_NS_CC;
using namespace std;

int time_util::getCurTime()
{
	time_t timep;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	time(&timep);
#else
	struct cc_timeval now;
	CCTime::gettimeofdayCocos2d(&now, NULL);
	timep = now.tv_sec;
#endif
	return int(timep);
}

tm *time_util::getDate(time_t time)
{
	tm *t = NULL;
	t = localtime(&time);
	t->tm_year = t->tm_year + 1900;
	t->tm_mon = t->tm_mon + 1;
	return t;
}