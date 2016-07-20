#ifndef __SCALEDIALOG_H__
#define __SCALEDIALOG_H__
#include "cocos2d.h"

class ScaleDialog
	: public cocos2d::CCNode
	, public cocos2d::CCTouchDelegate
{
public:
	virtual ~ScaleDialog(){}
	void setRunScale(bool isRunScale){ m_isRunScale = isRunScale; }
	void addMaskLayer();
protected:
	ScaleDialog();
	void runScale();
private:
	virtual void onEnter();
	virtual void onExit();
	void closeCallback(cocos2d::CCObject *pSender);
	virtual bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
protected:
	int m_touchPriority;
private:
	bool m_isRunScale;
};
#endif