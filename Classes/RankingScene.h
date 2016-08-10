#ifndef __RANKINGSCENE_H__
#define __RANKINGSCENE_H__ 
#include "cocos2d.h"
#include "BasePanel.h"

class UiLayout;
class ActionRunner;
class ListSlideView;

class RankingScene 
	: public BasePanel
{
public:
	CREATE_FUNC(RankingScene);
	virtual bool init();
private:
	RankingScene();
	~RankingScene();
	void initLayout();
	void initBottomLayout();
	void onGetReward(cocos2d::CCObject* pSender);
	virtual void onBackKeyTouched();
	void initWaitingLayer();
	void addRankingNodes();
private:
	UiLayout *m_layout;
	UiLayout *m_bottomlayout;
	ActionRunner *m_runner;
	cocos2d::CCNode *m_waitingLayer;
	ListSlideView *m_rankList;
};
#endif