#ifndef __TREASURESTAGETARGETPANEL_H__
#define __TREASURESTAGETARGETPANEL_H__
#include "cocos2d.h"

class UiLayout;
class TreasureStageTargetPanel 
	: public cocos2d::CCNode
	, public cocos2d::CCTouchDelegate
{
public:
	CREATE_FUNC(TreasureStageTargetPanel);
private:
	virtual bool init();
	virtual void onEnter();
	virtual void onExit();
	void initPanel();
	virtual bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	void runBeganAction();
	void runActionSeq();
private:
	UiLayout *m_layout;
};
#endif