#ifndef __MSGNOTIFIER_H__
#define __MSGNOTIFIER_H__
#include "cocos2d.h"
#include "CommonMacros.h"
//��������ڼ���Сģ�����Ϣ֪ͨ

struct IMsgWatcherView
{
	virtual void onThiefShowUp(){}
	virtual void onThiefDisappear(){}
	virtual void onPetPackageBuy(){}
	virtual void onDailyLoginPetReward(int petId, bool success){}
};

class MsgNotifier
{
public:
	static MsgNotifier *theModel();
	void addView(IMsgWatcherView *view);
	void removeView(IMsgWatcherView *view);
public:
	void onThiefShowUp(){ NOTIFY_VIEWS(onThiefShowUp); }
	void onThiefDisappear(){ NOTIFY_VIEWS(onThiefDisappear); }
	void onPetPackageBuy(){ NOTIFY_VIEWS(onPetPackageBuy); }
	void onDailyLoginPetReward(int petId, bool success){ NOTIFY_VIEWS(onDailyLoginPetReward, petId, success); }
private:
	std::vector<IMsgWatcherView *>m_views;
};
#endif
