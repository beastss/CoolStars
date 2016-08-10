#include "RankingScene.h"
#include "UiLayout.h"
#include "TitlePanel.h"
#include "ListView.h"
#include "ListSlideView.h"
#include "RankingPanelUtil.h"
#include "CommonMacros.h"
#include "RankingModel.h"
#include "DataManager.h"
#include "ThiefModel.h"
#include "GuideMgr.h"
#include "SoundMgr.h"
#include "MainScene.h"
#include "ActionRunner.h"
#include "NoTouchLayer.h"

USING_NS_CC;
using namespace std;

RankingScene::RankingScene()
{
	m_runner = ActionRunner::create();
	m_runner->retain();
}

RankingScene::~RankingScene()
{
	m_runner->clear();
	m_runner->release();
}

bool RankingScene::init()
{
	setPanelId(kRankingPanel);

	auto winSize = CCDirector::sharedDirector()->getWinSize();
	setContentSize(winSize);

	m_layout = UiLayout::create("layout/ranking_panel.xml");
	m_layout->setMenuTouchPriority(m_touchPriority);
	m_layout->setAnchorPoint(ccp(0.5f, 0.5f));
	m_layout->setPosition(ccpMult(winSize, 0.5f));
	addChild(m_layout);

	auto titlePanel = TitlePanel::create(m_touchPriority);
	titlePanel->setBottomThief(kThiefRankingPanel);
	addChild(titlePanel);

	if (RankingModel::theModel()->isOverOpponent())
	{
		m_bottomlayout = UiLayout::create("layout/ranking_bottom.xml");
		m_bottomlayout->setMenuTouchPriority(m_touchPriority);
		addChild(m_bottomlayout);
		initBottomLayout();
	}
	
	initLayout();
	GuideMgr::theMgr()->startGuide(kGuideStart_ranking_in);

	return true;
}

void RankingScene::initLayout()
{
	auto pos = m_layout->getChildById(7)->getPosition();
	m_rankList = ListSlideView::create(ccp(356, 400));
	addChild(m_rankList);
	m_rankList->setAnchorPoint(ccp(0, 1));
	m_rankList->setPosition(pos);
	m_rankList->setSpacing(10);

	initWaitingLayer();
	addRankingNodes();
}

void RankingScene::addRankingNodes()
{
	int opponentIndex = 0;
	int playerIndex = 0;
	auto ranks = RankingModel::theModel()->getCurRanking();
	m_rankList->setVisible(false);
	for (int i = (int)ranks.size() - 1; i >= 0; --i)
	{
		if (ranks[i].type == kOpponent) opponentIndex = ranks.size() - 1 - i;
		if (ranks[i].type == kPlayer) playerIndex = ranks.size() - 1 - i;

		m_runner->queueAction(CallFuncAction::withFunctor([=]()
		{
			m_rankList->addNode(RankingNode::create(i + 1, ranks[i]));
		}));
		m_runner->queueAction(DelayNFrames::delay(1));
	}
	m_runner->queueAction(CallFuncAction::withFunctor([=]()
	{
		m_rankList->setVisible(true);
		m_rankList->toNode(playerIndex);
		removeChild(m_waitingLayer, true);
	}));
}

void RankingScene::initBottomLayout()
{
	CCMenuItem *getReward = dynamic_cast<CCMenuItem *>((m_bottomlayout->getChildById(1)));
	getReward->setTarget(this, menu_selector(RankingScene::onGetReward));
}

void RankingScene::onGetReward(cocos2d::CCObject* pSender)
{
	SoundMgr::theMgr()->playEffect(kEffectMusicButton);
	RankingModel::theModel()->getOverOpponentReward();
	m_bottomlayout->getChildById(1)->setVisible(false);

}

void RankingScene::onBackKeyTouched()
{
	MainScene::theScene()->clearPanelRecord();
	MainScene::theScene()->showPanel(kMainMenu);
}

void RankingScene::initWaitingLayer()
{
	m_waitingLayer = CCNode::create();
	auto size = getContentSize();
	m_waitingLayer->setContentSize(size);
	addChild(m_waitingLayer);

	auto noTouchLayer = NoTouchLayer::create();
	noTouchLayer->setCanTouch(false);
	m_waitingLayer->addChild(noTouchLayer);
	
	CCLayerColor *mask = CCLayerColor::create(ccc4(0, 0, 0, 175));
	mask->ignoreAnchorPointForPosition(false);
	mask->setPosition(ccp(size.width * 0.5f, size.height * 0.5f));
	m_waitingLayer->addChild(mask);

	auto layout = UiLayout::create("layout/ranking_wait.xml");
	layout->setAnchorPoint(ccp(0.5f, 0.5f));
	layout->setPosition(ccpMult(size, 0.5f));
	m_waitingLayer->addChild(layout);

	auto icon = layout->getChildById(1);
	icon->runAction(CCRepeatForever::create(CCRotateBy::create(0.5f, 90)));
}
