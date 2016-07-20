#include "StageTargetPanel.h"
#include "UiLayout.h"
#include "CommonMacros.h"
#include "StarsController.h"
#include "StageTargetView.h"
#include "EmptyBox.h"
#include "CCFunctionAction.h"
#include "GuideMgr.h"
USING_NS_CC;

void StageTargetPanel::onEnter()
{
	CCNode::onEnter();
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, kStageTargetPanelPriority, true);
}

void StageTargetPanel::onExit()
{
	CCNode::onExit();
	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
}

bool StageTargetPanel::init()
{
	m_layout = UiLayout::create("layout/stage_target_panel.xml");
	auto size = m_layout->getContentSize();
	setContentSize(size);
	initPanel();
	addChild(m_layout);

	runBeganAction();
	return true;
}

void StageTargetPanel::initPanel()
{
	CCLayerColor *mask = CCLayerColor::create(ccc4(0, 0, 0, 175));
	auto size = getContentSize();
	mask->setContentSize(size);
	m_layout->addChild(mask, -1);

	auto target = StarsController::theModel()->getStageTarget();
	auto leftTarget = target->getEraseStarsLeft();
	assert(leftTarget.size() <= 3);
	for (int i = 0; i < 3; ++i)
	{
		m_layout->getChildById(2 + i)->setVisible(false);
	}

	for (size_t i = 0; i < leftTarget.size(); ++i)
	{
		StageTargetView *view = StageTargetView::create(leftTarget[i]);
		auto node = dynamic_cast<EmptyBox *>((m_layout->getChildById(5 + i)));
		node->setNode(view);

		m_layout->getChildById(2 + i)->setVisible(true);
	}
}

bool StageTargetPanel::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
	return true;
}

void StageTargetPanel::runBeganAction()
{
	auto winSize = CCDirector::sharedDirector()->getWinSize();
	m_layout->setPosition(ccp(winSize.width, 0));
	auto moveTo = CCMoveTo::create(0.5f, ccp(0, 0));
	auto func = CCFunctionAction::create(bind(&StageTargetPanel::runActionSeq, this));
	m_layout->runAction(CCSequence::create(CCEaseBackInOut::create(moveTo), CCDelayTime::create(1.5f), func, NULL));
}

void StageTargetPanel::runActionSeq()
{
	for (size_t i = 0; i < 3; ++i)
	{
		auto targetPos = convertToNodeSpace(m_pos[i]);
		m_layout->getChildById(5 + i)->runAction(CCEaseExponentialInOut::create(CCMoveTo::create(1.0f, targetPos)));
	}

	auto func = CCFunctionAction::create([=]()
	{
		GuideMgr::theMgr()->startGuide(kGuideStart_stage_showTargetFinished);
		removeFromParent();
	});
	runAction(CCSequence::create(CCDelayTime::create(1.0f), func, NULL));
}