#include "SettingBtnsHelper.h"
#include "UiLayout.h"
#include "CCFunctionAction.h"
USING_NS_CC;
using namespace std;

void SettingBtnsHelper::toggle()
{
	const float kDuration = 0.2f;
	if (m_isVisible)
	{
		for (auto iter = m_nodes.begin(); iter != m_nodes.end(); ++iter)
		{
			auto moveTo = CCMoveTo::create(kDuration, m_targetPos);
			auto func = [=]()
			{
				iter->first->setVisible(false);
			};
			iter->first->runAction(CCSequence::create(CCEaseBackInOut::create(moveTo), CCFunctionAction::create(func), NULL));
		}
	}
	else
	{
		for (auto iter = m_nodes.begin(); iter != m_nodes.end(); ++iter)
		{
			iter->first->setVisible(true);
			auto moveTo = CCMoveTo::create(kDuration, iter->second);
			iter->first->runAction(CCEaseBackInOut::create(moveTo));
		}
	}

	m_isVisible = !m_isVisible;
}

void SettingBtnsHelper::addPopupNodes(cocos2d::CCNode *node)
{
	if (m_nodes.find(node) == m_nodes.end())
	{
		m_nodes.insert(make_pair(node, node->getPosition()));
	}
}

void SettingBtnsHelper::removePopupNodes(cocos2d::CCNode *node)
{
	auto iter = m_nodes.find(node);
	if (iter != m_nodes.end())
	{
		m_nodes.erase(iter);
	}
}

void SettingBtnsHelper::init(cocos2d::CCPoint targetPos)
{ 
	m_targetPos = targetPos;

	for (auto iter = m_nodes.begin(); iter != m_nodes.end(); ++iter)
	{
		iter->first->setPosition(targetPos);
		iter->first->setVisible(false);
	}
}
