#include "BackgroundLayer.h"
#include "CCFunctionAction.h"
#include "UiLayout.h"
#include "cocos-ext.h"
#include "CocoStudio/Armature/CCArmature.h"
USING_NS_CC;
using namespace std;
USING_NS_CC_EXT;

bool BackgroundLayer::init()
{
	m_bkLayout = UiLayout::create("layout/common_background.xml");
	m_bkLayout->setAnchorPoint(ccp(0.5f, 0.5f));
	initLayout();
	auto size = m_bkLayout->getContentSize();
	m_bkLayout->setPosition(ccpMult(size, 0.5f));
	addChild(m_bkLayout);

	setContentSize(size);
	setAnchorPoint(ccp(0.5, 0.5));
	showSimpleBk(true);
	return true;
}

void BackgroundLayer::showSimpleBk(bool simpleBk)
{
	m_bkLayout->getChildById(1)->setVisible(simpleBk);
	m_bkLayout->getChildById(2)->setVisible(!simpleBk);
}

void BackgroundLayer::doMoveAction(CCNode *node, float speed, CCPoint sourcePos, CCPoint targetPos)
{
	node->setPosition(sourcePos);

	float duration = fabs((targetPos.x - sourcePos.x) / speed);
	CCMoveTo *moveTo = CCMoveTo::create(duration, targetPos);

	function<void()> func = bind(&BackgroundLayer::doMoveAction, this, node, speed, sourcePos, targetPos);
	node->runAction(CCSequence::create(moveTo, CCFunctionAction::create(func), NULL));
}

void BackgroundLayer::initLayout()
{
	auto winSize = CCDirector::sharedDirector()->getWinSize();
	CCPoint sourcePos;
	CCPoint targetPos;
	float speed = 0;

	auto clound1 = dynamic_cast<CCArmature *>(m_bkLayout->getChildById(5));
	sourcePos = ccp(winSize.width * 1.3f, clound1->getPositionY());
	targetPos = ccp(winSize.width * -0.3f, clound1->getPositionY());
	speed = (int)(CCRANDOM_0_1() * 100) + 50;
	clound1->getAnimation()->setSpeedScale(speed / 500);
	doMoveAction(clound1, speed, sourcePos, targetPos);

	auto clound2 = dynamic_cast<CCArmature *>(m_bkLayout->getChildById(6));
	sourcePos = ccp(winSize.width * -0.3f, clound2->getPositionY());
	targetPos = ccp(winSize.width * 1.3f, clound2->getPositionY());
	speed = (int)(CCRANDOM_0_1() * 100) + 50;
	clound2->getAnimation()->setSpeedScale(speed / 500);
	doMoveAction(clound2, speed, sourcePos, targetPos);

	auto clound3 = dynamic_cast<CCArmature *>(m_bkLayout->getChildById(7));
	sourcePos = ccp(winSize.width * -0.3f, clound3->getPositionY());
	targetPos = ccp(winSize.width * 1.3f, clound3->getPositionY());
	speed = (int)(CCRANDOM_0_1() * 100) + 50;
	clound3->getAnimation()->setSpeedScale(speed / 500);
	doMoveAction(clound3, speed, sourcePos, targetPos);
}