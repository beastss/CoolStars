#include "StageTargetPanel.h"
#include "UiLayout.h"
#include "CommonMacros.h"
#include "StarsController.h"
#include "StageTargetView.h"
#include "EmptyBox.h"
#include "CCFunctionAction.h"
#include "GuideMgr.h"
#include "StageLayersMgr.h"
#include "CommonUtil.h"
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
	auto vPos = CommonUtil::getSameDistancePos(m_layout->getChildById(4)->getPosition(), m_layout->getChildById(7)->getPosition(), leftTarget.size() + 1);

	auto layout = UiLayout::create("layout/stage_target_panel_node2.xml");
	CCLabelAtlas *score = dynamic_cast<CCLabelAtlas *>(layout->getChildById(3));
	score->setString(CommonUtil::intToStr(target->getTargetScore()));
	auto scoreBox = dynamic_cast<EmptyBox *>((m_layout->getChildById(4)));
	scoreBox->setNode(layout);
	scoreBox->setPosition(vPos[0]);
	m_icons.push_back(score);

	for (size_t i = 0; i < leftTarget.size(); ++i)
	{
		auto layout = UiLayout::create("layout/stage_target_panel_node.xml");
		StageTargetView *view = StageTargetView::create(leftTarget[i]);
		auto box = dynamic_cast<EmptyBox *>((layout->getChildById(3)));
		box->setNode(view);
		m_icons.push_back(view);

		auto node = dynamic_cast<EmptyBox *>((m_layout->getChildById(5 + i)));
		node->setNode(layout);
		node->setPosition(vPos[i + 1]);
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
	m_layout->runAction(CCSequence::create(CCDelayTime::create(1.0), CCEaseBackInOut::create(moveTo), CCDelayTime::create(2.0f), func, NULL));
}

void StageTargetPanel::runActionSeq()
{
	//int widgetIds[] = { 10, 5, 6, 7 };
	for (size_t i = 0; i < m_icons.size(); ++i)
	{
		auto targetPos = m_icons[i]->getParent()->convertToNodeSpace(m_pos[i]);
		m_icons[i]->runAction(CCEaseExponentialInOut::create(CCMoveTo::create(1.0f, targetPos)));
	}

	auto func = CCFunctionAction::create([=]()
	{
		StageLayersMgr::theMgr()->targetPanelDone();
		GuideMgr::theMgr()->startGuide(kGuideStart_stage_showTargetFinished);
		removeFromParent();
	});
	runAction(CCSequence::create(CCDelayTime::create(1.0f), func, NULL));
}