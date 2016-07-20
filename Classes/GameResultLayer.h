#ifndef __GAMERESULTLAYER_H__
#define __GAMERESULTLAYER_H__
#include "ScaleDialog.h"
#include "BasePanel.h"
#include "StageDataMgr.h"

class UiLayout;

class GameResultLayer
	: public BasePanel
	, public cocos2d::CCTouchDelegate
{
public:
	bool init(std::string xmlFile);
protected:
	virtual void addGameResultReward(){}

private:
	virtual bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	void initLayout();
	void initRewardData();
	void onConfirmBtnClicked(CCObject *pSender);
	virtual void onEnter();
	virtual void onExit();

	void getRuneStoneReward();
	void addFood(int value);
	void addDiamond(int value);
	void addKey(int value);
	void saveRewards();
protected:
	UiLayout *m_layout;
	GameResultReward m_reward;
};

class GameWinLayer : public GameResultLayer
{
public:
	CREATE_FUNC(GameWinLayer);
	bool init();
private:
	void initPanel();
	void addGameResultReward();
private:
};


class GameFailLayer : public GameResultLayer
{
public:
	CREATE_FUNC(GameFailLayer);
	bool init();
private:
	void initPanel();
	void onBuyPetBtnClicked(CCObject *pSender);
	void addGameResultReward();
private:

};
#endif