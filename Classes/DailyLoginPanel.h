#ifndef _DAILYLOGINPANEL_H_
#define _DAILYLOGINPANEL_H_
#include "ScaleDialog.h"
#include "ScaleDialog.h"
#include "MsgNotifier.h"

class UiLayout;

class DailyLoginPetHint
	: public ScaleDialog
{
public:
	CREATE_FUNC(DailyLoginPetHint);
private:
	bool init();
	DailyLoginPetHint(){}
	void onOkBtnClicked(cocos2d::CCObject* pSender);
};

class DailyLoginNode
	: public cocos2d::CCNode
{
public:
	static DailyLoginNode *create(int day);
private:
	DailyLoginNode(int day);
	virtual bool init();
	void highLightRewards();
private:
	int m_day;
	UiLayout *m_layout;
};

class DailyLoginPanel
	: public ScaleDialog
	, public IMsgWatcherView
{
public:
	CREATE_FUNC(DailyLoginPanel);
private:
	bool init();
	void onCloseBtnClicked(cocos2d::CCObject* pSender);
	void onGetRewardBtnClicked(cocos2d::CCObject* pSender);
	void refreshUi();
private:
	virtual void onEnter();
	virtual void onExit();
	virtual void onDailyLoginPetReward(int petId, bool success);
private:
	UiLayout *m_layout;
	std::vector<DailyLoginNode *>m_nodes;
};
#endif