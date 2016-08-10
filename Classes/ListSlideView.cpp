#include "ListSlideView.h"
USING_NS_CC;
using namespace std;
using namespace std::placeholders;


ListSlideView *ListSlideView::create(const CCSize &size)
{
	ListSlideView *view = new ListSlideView(size);
	view->init();
	view->autorelease();
	return view;
}


ListSlideView::ListSlideView(const CCSize &size)
: ListView(size)
, m_speed(0)
, m_scrollTime(0)
, m_curIndex(0)
{
}

bool ListSlideView::init()
{
	ListView::init();
	scheduleUpdate();
	return true;
}

void ListSlideView::update(float dt)
{
	static const float speedPrec = 1.0f;

	if (m_isTouching)
	{
		m_scrollTime += dt;
	}
	else if (isOutOfRange())
	{
		m_speed = 0;
	}
	else if (fabs(m_speed) > speedPrec)
	{
		m_speed -= dt * m_speed * 2;
		CCPoint offset = ccp(0, m_speed * dt);
		doMove(offset);
	}
}

void ListSlideView::onTouchBegan(cocos2d::CCTouch *pTouch)
{
	m_speed = 0;
	m_scrollTime = 0;
}

void ListSlideView::onTouchMoved(cocos2d::CCTouch *pTouch)
{

}

void ListSlideView::onTouchEnded(cocos2d::CCTouch *pTouch)
{
	static float maxSpeed = 1000;
	static float minSpeed = 100;

	auto startPos = pTouch->getStartLocation();
	auto curPos = pTouch->getLocation();

	m_speed = (curPos.y - startPos.y) / m_scrollTime;
	if (fabs(m_speed) > maxSpeed)
	{
		m_speed = m_speed > 0 ? maxSpeed : -maxSpeed;
	}
	if (fabs(m_speed) < minSpeed)
	{
		m_speed = 0;
	}
	
	//list node的点选回调
	static const float maxMoveDistance = 10;
	if (fabs(curPos.y - startPos.y) <= maxMoveDistance)
	{
		auto pos = m_container->convertToNodeSpace(curPos);
		for (size_t i = 0; i < m_nodes.size(); ++i)
		{
			if (m_nodes[i]->boundingBox().containsPoint(pos) && m_handle)
			{
				m_curIndex = i;
				m_handle(i);
				break;
			}
		}
	}
}

void ListSlideView::toNode(int index, bool withAction)
{
	auto node = getNode(index);
	if (!node) return;
	float posY = m_size.height - node->getPosition().y - node->getContentSize().height;
	
	auto containerSize = m_container->getContentSize();

	float maxY = 0;
	float minY = 0;
	if (containerSize.height <= m_size.height)
	{
		maxY = m_size.height - containerSize.height;
		minY = maxY;
	}
	else
	{
		minY = m_size.height - containerSize.height;
		maxY = 0;
	}
	float newY = min(max(posY, minY), maxY);
	if (withAction)
	{
		float duration = (count() - index) * 0.01f;
		CCPoint targetPos = m_container->getPosition();
		targetPos.y = newY;
		auto move = CCMoveTo::create(duration, targetPos);
		m_container->runAction(move);
	}
	else
	{
		m_container->setPositionY(newY);
	}

}
