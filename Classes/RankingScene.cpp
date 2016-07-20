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

USING_NS_CC;
using namespace std;

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
	initLayout();

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
	
	GuideMgr::theMgr()->startGuide(kGuideStart_ranking_in);
	return true;
}

void RankingScene::initLayout()
{
	auto pos = m_layout->getChildById(7)->getPosition();
	ListSlideView *rankList = ListSlideView::create(ccp(356, 400));
	addChild(rankList);
	rankList->setAnchorPoint(ccp(0, 1));
	rankList->setPosition(pos);
	rankList->setSpacing(10);
	auto ranks = RankingModel::theModel()->getNeighboursRanking();
	for (auto iter = ranks.rbegin(); iter != ranks.rend(); ++iter)
	{
		rankList->addNode(RankingNode::create(iter->first, iter->second));
	}
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

