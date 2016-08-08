#ifndef __TITLEPANEL_H__
#define __TITLEPANEL_H__
#include "cocos2d.h"
#include "UserInfo.h"
#include "MsgNotifier.h"

class UiLayout;

enum TitlePanelUi
{
	kTitlePanelStrength,
	kTitlePanelFood,
	kTitlePanelDiamond,
	kTitlePanelBackHome,
};

class TitlePanel
	: public cocos2d::CCNode
	, public IUserInfoView
	, public cocos2d::CCTouchDelegate
	, public IMsgWatcherView
{
public:
	static TitlePanel *create(int touchPriority);
	virtual bool init();
	void setUiVisible(int who, bool isVisible);
	void setTopThief(int type);
	void setBottomThief(int type);
	cocos2d::CCPoint getWidgetPos(int type);
private:
	virtual void onEnter();
	virtual void onExit();

	TitlePanel(int touchPriority) : m_touchPriority(touchPriority), m_thiefNode(NULL){}
	void initTopLayout();
	void initBottomLayout();

	void onAddStrengthBtnClicked(cocos2d::CCObject* pSender);
	void onAddDiamondBtnClicked(cocos2d::CCObject* pSender);

	void onBackHomeBtnClicked(cocos2d::CCObject* pSender);
	virtual bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
private:
	virtual void onDiamondChanged();
	virtual void onFoodChanged();
	virtual void onStrengthChanged();
	virtual void onThiefShowUp();
	virtual void onStrengthLeftTimeChanged(int leftSecs);
	virtual void onThiefDisappear();
	cocos2d::CCAction *getScaleAction();
private:
	
	UiLayout *m_topLayout;
	UiLayout *m_bottomLayout;
	int m_touchPriority;
	cocos2d::CCNode *m_thiefNode;
	int m_thiefType;
};
#endif