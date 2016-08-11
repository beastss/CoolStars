#ifndef __FAILSTAGERECORD_H__
#define __FAILSTAGERECORD_H__
#include "cocos2d.h"
class FailStateRecord
{
public:
	FailStateRecord();
	bool isFirstFail();
	void recordFailState(bool isWon);
private:
	enum LastGameoverState
	{
		kStateWon, //上次胜利
		kStateFirstFail,//上次第一次输
		kStateFailTimes,//已经输了很多次
	};
private:
	int m_curFailState;
	
};

#endif