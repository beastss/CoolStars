#ifndef __TESTSCENE_H__
#define __TESTSCENE_H__

#include "cocos2d.h"

class InputPad;

class TestScene : public cocos2d::CCLayer
{
public:
	virtual bool init();
	static cocos2d::CCScene* scene();
	void menuCloseCallback(CCObject* pSender);
	CREATE_FUNC(TestScene);
private:
	void closeCallback(cocos2d::CCObject* pSender);
	void testCallback(cocos2d::CCObject* pSender);
	void addClippingNode();
	void initPanel();
	void imageNumtest();
	void animationTest();
	void animationTest2();
	void listViewTest();
	void printDBInfos();
	void progressTest();
	void blinkAction();
	void test();
	void testSameDistancePos();
	void sensitiveWordsTest();
	void inputPadTest();
private:
	cocos2d::CCClippingNode *m_clippingNode;
	cocos2d::CCSprite *m_spr;
	InputPad *m_inputPad;
};


#endif
