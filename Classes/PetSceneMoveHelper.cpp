#include "PetSceneMoveHelper.h"
#include "CCFunctionAction.h"
USING_NS_CC;
using namespace std;

const float PetSceneMoveHelper::kFirstMoveDuation = 0.2f;
const float PetSceneMoveHelper::kSecondMoveDuation = 0.5f;

PetSceneMoveHelper::PetSceneMoveHelper()
: m_curNode(NULL)
{

}

void PetSceneMoveHelper::init(const cocos2d::CCPoint &leftmost, const cocos2d::CCPoint &center, const cocos2d::CCPoint &rightmost)
{
	m_leftmostPos = leftmost;
	m_centerPos = center;
	m_rightmostPos = rightmost;
}

void PetSceneMoveHelper::setCenterNode(cocos2d::CCNode *centerNode)
{
	centerNode->setAnchorPoint(ccp(0.5f, 0.5f));
	centerNode->setPosition(m_centerPos);
	m_curNode = centerNode;
}

void PetSceneMoveHelper::moveLeft(cocos2d::CCNode *newNode)
{
	if (m_curNode)
	{
		auto removeFunc = CCFunctionAction::create(bind(&PetSceneMoveHelper::removeNode, this, m_curNode));
		m_curNode->runAction(CCSequence::create(CCMoveTo::create(kFirstMoveDuation, m_leftmostPos), removeFunc, NULL));
		m_curNode = NULL;
	}

	newNode->setAnchorPoint(ccp(0.5f, 0.5f));
	newNode->setPosition(m_rightmostPos);
	m_curNode = newNode;
	newNode->runAction(CCEaseBackInOut::create(CCMoveTo::create(kSecondMoveDuation, m_centerPos)));
}

void PetSceneMoveHelper::moveRight(cocos2d::CCNode *newNode)
{
	if (m_curNode)
	{
		auto removeFunc = CCFunctionAction::create(bind(&PetSceneMoveHelper::removeNode, this, m_curNode));
		m_curNode->runAction(CCSequence::create(CCMoveTo::create(kFirstMoveDuation, m_rightmostPos), removeFunc, NULL));
		m_curNode = NULL;
	}

	newNode->setAnchorPoint(ccp(0.5f, 0.5f));
	newNode->setPosition(m_leftmostPos);
	m_curNode = newNode;
	newNode->runAction(CCEaseBackInOut::create(CCMoveTo::create(kSecondMoveDuation, m_centerPos)));
}

void PetSceneMoveHelper::removeNode(CCNode *node)
{
	node->removeFromParent();
}

void PetSceneMoveHelper::clearNodes()
{
	if (m_curNode)
	{
		m_curNode->removeFromParentAndCleanup(true);
		m_curNode = NULL;
	}
}

