#ifndef __TOUCHNODE_H__
#define __TOUCHNODE_H__
#include "cocos2d.h"

class TouchNode
	: public cocos2d::CCNode
	, public cocos2d::CCTouchDelegate
{
public:
	virtual ~TouchNode(){}
	virtual bool init();
	void setTouchPriority(int touchPriority){ m_touchPriority = touchPriority; }
protected:
	TouchNode(int touchPriority = 0): m_touchPriority(touchPriority){}
	virtual bool onTouchBegan(cocos2d::CCPoint pt, bool isInside){ return true; }
	void runScale();
	virtual void onEnter();
	virtual void onExit();
private:

	virtual bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
private:
	int m_touchPriority;
};
#endif