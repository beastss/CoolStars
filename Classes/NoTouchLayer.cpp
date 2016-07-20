#include "NoTouchLayer.h"
#include "CommonMacros.h"
using namespace std;
USING_NS_CC; 

void NoTouchLayer::onEnter()
{
	CCNode::onEnter();
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, kStageNoTouchPriority, true);
}

void NoTouchLayer::onExit()
{
	CCNode::onExit();
	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
}

bool NoTouchLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
	return !m_canTouch;
}

