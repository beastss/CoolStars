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
		kStateWon, //�ϴ�ʤ��
		kStateFirstFail,//�ϴε�һ����
		kStateFailTimes,//�Ѿ����˺ܶ��
	};
private:
	int m_curFailState;
	
};

#endif