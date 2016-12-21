#include "TreasureStageTargetPanel.h"
#include "UiLayout.h"
#include "CommonMacros.h"
#include "EmptyBox.h"
#include "CCFunctionAction.h"
#include "CommonUtil.h"
#include "TreasureStageMacro.h"
#include "TreasureStageState.h"
USING_NS_CC;

void TreasureStageTargetPanel::onEnter()
{
	CCNode::onEnter();
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, kTreasureStageTargetPanelPriority, true);
}

void TreasureStageTargetPanel::onExit()
{
	CCNode::onExit();
	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
}

bool TreasureStageTargetPanel::init()
{
	m_layout = UiLayout::create("layout/treasure_stage_target_panel.xml");
	auto size = m_layout->getContentSize();
	setContentSize(size);
	initPanel();
	addChild(m_layout);

	runBeganAction();
	return true;
}

void TreasureStageTargetPanel::initPanel()
{
	CCLabelAtlas *curStage = dynamic_cast<CCLabelAtlas *>(m_layout->getChildById(14));
	curStage->setString(CommonUtil::intToStr(TreasureStageState::theState()->getCurStage()));
	CCLabelAtlas *score = dynamic_cast<CCLabelAtlas *>(m_layout->getChildById(5));
	score->setString(CommonUtil::intToStr(TreasureStageState::theState()->getTargeScore()));
	CCLabelAtlas *foodReward = dynamic_cast<CCLabelAtlas *>(m_layout->getChildById(8));
	foodReward->setString(CommonUtil::intToStr(TreasureStageState::theState()->getStageFoodReward()));
	CCLabelAtlas *diamondReward = dynamic_cast<CCLabelAtlas *>(m_layout->getChildById(11));
	diamondReward->setString(CommonUtil::intToStr(TreasureStageState::theState()->getStageDiamondReward()));
}

bool TreasureStageTargetPanel::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
	return true;
}

void TreasureStageTargetPanel::runBeganAction()
{
	auto winSize = CCDirector::sharedDirector()->getWinSize();
	m_layout->setPosition(ccp(winSize.width, 0));
	auto moveTo = CCMoveTo::create(0.5f, ccp(0, 0));
	//auto func = CCFunctionAction::create(bind(&TreasureStageTargetPanel::runActionSeq, this));
	auto func = CCFunctionAction::create([=]()
	{
		removeFromParent();
	});
	m_layout->runAction(CCSequence::create(CCDelayTime::create(1.0), CCEaseBackInOut::create(moveTo), CCDelayTime::create(3.0f), func, NULL));
}

void TreasureStageTargetPanel::runActionSeq()
{
	auto func = CCFunctionAction::create([=]()
	{
		removeFromParent();
	});
	runAction(CCSequence::create(CCDelayTime::create(1.0f), func, NULL));
}