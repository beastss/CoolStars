#ifndef __LOTTERYSCENE_H__
#define __LOTTERYSCENE_H__ 
#include "cocos2d.h"
#include "BasePanel.h"
#include "TouchNode.h"
#include <functional>
#include "UserInfo.h"

class UiLayout;
class TitlePanel;
class LotteryScene;
class LotteryNode
	: public TouchNode
{
public:
	static LotteryNode *create(int touchPriority, LotteryScene *panel);
	void setHandle(std::function<void()> handle){ m_handle = handle; }
	bool isOpened();
private:
	virtual bool init();
	LotteryNode(int touchPriority, LotteryScene *panel);
	void openReward();
	void handleTouch();
	virtual bool onTouchBegan(cocos2d::CCPoint pt, bool isInside);
	cocos2d::CCAction *getRewardOutAction(int num);
private:
	UiLayout *m_layout;
	UiLayout *m_goodsLayout;
	std::function<void()> m_handle;
	bool m_isOpened;
	LotteryScene *m_panel;
};

enum LotterySceneUsage
{
	kLotterySceneFromMenuScene,
	kLotterySceneFromStageScene,
};

class LotteryScene 
	: public BasePanel
	, public IUserInfoView
{
public:
	static LotteryScene *create(int usage);
	virtual bool init();
	void runKeyMoveAction(cocos2d::CCPoint target, std::function<void()>callback);
private:
	virtual void onEnter();
	virtual void onExit();
	LotteryScene(int usage);
	void initLayout();
	void initBottomLayout();
	void refreshUi();
	void toPetScene(cocos2d::CCObject* pSender);
	void onStartBtnClicked(cocos2d::CCObject* pSender);
	void onOpenReward();
	virtual void onKeyChanged();
	void hideBottomBtns();

private:
	UiLayout *m_layout;
	UiLayout *m_bottomLayout;
	TitlePanel *m_titlePanel;
	static const int kRewardBoxNum = 9;
	static const int kMinOpenBoxNum = 3;
	int m_openedBoxNum;
};
#endif