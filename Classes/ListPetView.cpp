#include "ListPetView.h"
USING_NS_CC;
using namespace std;
using namespace std::placeholders;


ListPetView *ListPetView::create(const CCSize &size)
{
	ListPetView *view = new ListPetView(size);
	view->init();
	view->autorelease();
	return view;
}


ListPetView::ListPetView(const CCSize &size)
: ListView(size)
, m_curItem(0)
{
	scheduleUpdate();
}

bool ListPetView::init()
{
	ListView::init();
	return true;
}

//新加入的node会在listview的底部
int ListPetView::addNode(cocos2d::CCNode *node)
{
	auto containerSize = m_container->getContentSize();
	
	float lastNodeHeight = 0;
	if (!m_nodes.empty())
	{
		lastNodeHeight = m_nodes.back()->getContentSize().height;
	}
	
	float offsetY = m_size.height + (node->getContentSize().height - lastNodeHeight) * 0.5f;

	for (auto iter = m_nodes.rbegin(); iter != m_nodes.rend(); ++iter)
	{
		auto curNode = *iter;
		auto curNodeHeight = curNode->getPositionY();
		curNode->setPositionY(curNodeHeight + offsetY);
	}
	
	m_container->setContentSize(ccp(containerSize.width, containerSize.height + offsetY));
	m_container->addChild(node);
	m_nodes.push_back(node);
	
	m_curItem = m_nodes.size() - 1;
	return m_nodes.size() - 1;
}

void ListPetView::update(float dt)
{
}

void ListPetView::onTouchBegan(cocos2d::CCTouch *pTouch)
{
	
}

void ListPetView::onTouchMoved(cocos2d::CCTouch *pTouch)
{

}

void ListPetView::onTouchEnded(cocos2d::CCTouch *pTouch)
{
	static float minYOffset = 100;
	auto startPos = pTouch->getStartLocation();
	auto curPos = pTouch->getLocation();

	auto offset = ccpSub(curPos, startPos);
	if (fabs(offset.y) < minYOffset)
	{
		setCurItem(m_curItem);
	}
	else
	{
		int nextIndex = offset.y > 0 ? m_curItem + 1 : m_curItem - 1;
		setCurItem(nextIndex);
	}
}

void ListPetView::dragBack()
{

}

void ListPetView::nextItem()
{

}

void ListPetView::moveItemToCenter(int index)
{
	auto node = getNode(index);
	if (!node) return;
	
	auto nodeSize = node->getContentSize();

	float targetY = (m_size.height - nodeSize.height) * 0.5f - node->getPositionY();
	m_container->setPosition(ccp(0, targetY));

	//m_container->runAction(CCEaseBounceInOut::create(CCMoveTo::create(0.2f, ccp(0, targetY))));
	if (m_itemSelectedHandle)
	{
		m_itemSelectedHandle(m_curItem);
	}
}

void ListPetView::setCurItem(int index)
{
	if (getNode(index))
	{
		m_curItem = index;
	}
	//如果list为空 那么初始化的m_cureItem也没有对应节点
	if (getNode(m_curItem))
	{
		moveItemToCenter(m_curItem);
	}
}

void ListPetView::clear()
{
	ListView::clear();
	m_curItem = 0;
}
