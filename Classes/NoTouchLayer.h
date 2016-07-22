#ifndef __NOTOUCHLAYER_H__
#define __NOTOUCHLAYER_H__
#include "cocos2d.h"

class NoTouchLayer
	: public cocos2d::CCNode
	, public cocos2d::CCTouchDelegate
{
public:
	CREATE_FUNC(NoTouchLayer);
	void setCanTouch(bool canTouch, int tag = 0);
protected:
	virtual void onEnter();
	virtual void onExit();
private:
	NoTouchLayer(): m_canTouch(true){}
	virtual bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
private:
	int m_touchPriority;
	bool m_canTouch;
	std::vector<int> m_tags;
};
#endif