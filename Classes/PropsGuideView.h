#ifndef __PROPSGUIDEVIEW_H__
#define __PROPSGUIDEVIEW_H__
#include "cocos2d.h"

class PropsGuideView
	: public cocos2d::CCNode
	, public cocos2d::CCTouchDelegate
{
public:
	CREATE_FUNC(PropsGuideView);
	void loadPropsPos(const std::vector<cocos2d::CCPoint> &pos){ m_pos = pos; }
	void setFinishHandle(std::function<void()> handle){ m_handle = handle; }
private:
	virtual void onEnter();
	virtual void onExit();
	virtual bool init();
	void runScaleAction();
	void toPropsItem();
	virtual bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){ return true; }
private:
	std::vector<cocos2d::CCPoint> m_pos;
	std::function<void()> m_handle;
};

#endif 