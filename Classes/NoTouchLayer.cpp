#include "NoTouchLayer.h"
#include "CommonMacros.h"
#include <algorithm>
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

void NoTouchLayer::setCanTouch(bool canTouch, int tag)
{
	auto iter = find(m_tags.begin(), m_tags.end(), tag);

	if (canTouch)
	{
		//“∆≥˝∆•≈‰œÓ
		if (iter != m_tags.end())
		{
			m_tags.erase(iter);
		}
		m_canTouch = m_tags.empty();
	}
	else
	{
		if (iter == m_tags.end())
		{
			m_tags.push_back(tag);
		}
		m_canTouch = false;
	}
}
