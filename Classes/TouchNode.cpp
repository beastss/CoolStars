#include "TouchNode.h"
using namespace std;
USING_NS_CC; 
//子类设置了contentsize 之后调用即可
bool TouchNode::init()
{
	return true;
}

void TouchNode::onEnter()
{
	CCNode::onEnter();
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, m_touchPriority, true);
}

void TouchNode::onExit()
{
	CCNode::onExit();
	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
}

bool TouchNode::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
	auto pos = convertToNodeSpace(pTouch->getLocation());
	auto size = getContentSize();
	CCRect rect(0, 0, size.width, size.height);
	bool isInside = rect.containsPoint(pos);
	return onTouchBegan(pTouch->getLocation(), isInside);
}

void TouchNode::runScale()
{
	CCScaleTo *scaleLarge = CCScaleTo::create(0.15f, 1.3f);
	CCScaleTo *scaleSmall = CCScaleTo::create(0.15f, 0.8f);
	CCScaleTo *scaleNormal = CCScaleTo::create(0.1f, 1.0f);
	CCSequence *seq = CCSequence::create(scaleLarge, scaleSmall, scaleNormal, NULL);

	runAction(seq);
}