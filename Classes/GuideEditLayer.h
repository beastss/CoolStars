#ifndef __GUIDEEDITORLAYER_H__
#define __GUIDEEDITORLAYER_H__
#include "cocos2d.h"
class UiLayout;
class GuideEditLayer
	: public cocos2d::CCNode
	, public cocos2d::CCTouchDelegate
{
public:
	static GuideEditLayer *create();
private:
	GuideEditLayer(){}
	virtual bool init();
	void initPanel();
	void addMaskLayer();

	virtual void onEnter();
	virtual void onExit();
	virtual bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual void ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual void ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	
	void onCloseBtnClicked(cocos2d::CCObject* pSender);
	void onSaveBtnClicked(cocos2d::CCObject* pSender);
private:
	static const int kGuideEditTouchPriority = -129;
	static const int kHighLightNodeTag = -100;
	UiLayout *m_layout;
};

#endif