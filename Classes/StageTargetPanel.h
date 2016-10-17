#ifndef __STAGETARGETPANEL_H__
#define __STAGETARGETPANEL_H__
#include "cocos2d.h"

class UiLayout;
class StageTargetPanel 
	: public cocos2d::CCNode
	, public cocos2d::CCTouchDelegate
{
public:
	CREATE_FUNC(StageTargetPanel);
	void loadTargetPos(const std::vector<cocos2d::CCPoint> &pos){ m_pos = pos; }
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
	std::vector<cocos2d::CCPoint> m_pos;
	std::vector<cocos2d::CCNode *> m_icons;
};
#endif