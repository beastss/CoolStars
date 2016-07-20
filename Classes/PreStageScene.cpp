#include "PreStageScene.h"
#include "UiLayout.h"
#include "TitlePanel.h"
#include "SoundMgr.h"
#include "StageScene.h"
#include "EmptyBox.h"
#include "PreStagePetSlot.h"
#include "PetManager.h"
#include "MainScene.h"
#include "StarsController.h"
#include "StageTarget.h"
#include "StageTargetView.h"
#include "CommonUtil.h"
#include "GuideMgr.h"
#include "StageDataMgr.h"
#include "ThiefModel.h"
USING_NS_CC;
using namespace std;
using namespace CommonUtil;

bool PreStageScene::init()
{
	PreStageModel::theModel()->init();

	setPanelId(kPreStagePanel);

	auto winSize = CCDirector::sharedDirector()->getWinSize();
	setContentSize(winSize);

	m_mainLayout = UiLayout::create("layout/pre_stage.xml");
	m_mainLayout->setMenuTouchPriority(m_touchPriority);
	m_mainLayout->setAnchorPoint(ccp(0.5f, 0.5f));
	m_mainLayout->setPosition(ccpMult(winSize, 0.5f));
	addChild(m_mainLayout);
	initMainLayout();

	m_titlePanel = TitlePanel::create(m_touchPriority);
	m_titlePanel->setBottomThief(kThiefPreStagePanel);
	addChild(m_titlePanel);

	m_bottomLayout = UiLayout::create("layout/pre_stage_bottom.xml");
	m_bottomLayout->setMenuTouchPriority(m_touchPriority);
	addChild(m_bottomLayout);
	initBottomLayout();

	GuideMgr::theMgr()->startGuide(kGuideStart_preStage_in, bind(&PreStageScene::hideHomeBackBtn, this));

	return true;
}

void PreStageScene::initMainLayout()
{
	//宠物滚动槽
	int boxIds[] = { 11, 12, 13, 14 };
	auto temp = PetManager::petMgr()->getCurPetIds();
	vector<int> ids(4);
	for (size_t i = 0; i < temp.size(); ++i)
	{
		ids[i] = temp[i];
	}
	
	for (int i = 0; i < 4; ++i)
	{
		EmptyBox *box = dynamic_cast<EmptyBox *>((m_mainLayout->getChildById(boxIds[i])));
		auto node = PreStagePetSlot::create(ids[i]);
		box->setNode(node);
	}

	//游戏目标
	int targetBoxIds[] = { 18, 19, 20 };
	auto target = StarsController::theModel()->getStageTarget();
	auto leftTarget = target->getEraseStarsLeft();
	assert(leftTarget.size() <= 3);
	for (size_t i = 0; i < leftTarget.size(); ++i)
	{
		StageTargetView *view = StageTargetView::create(leftTarget[i]);
		auto node = dynamic_cast<EmptyBox *>((m_mainLayout->getChildById(targetBoxIds[i])));
		node->setNode(view);
	}

	CCLabelAtlas *stepLabel = dynamic_cast<CCLabelAtlas *>(m_mainLayout->getChildById(17));
	int stepLeft = StageDataMgr::theMgr()->getLeftSteps();
	stepLabel->setString(intToStr(stepLeft));
	
	CCLabelAtlas *stageLabel = dynamic_cast<CCLabelAtlas *>(m_mainLayout->getChildById(15));
	int curStage = StageDataMgr::theMgr()->getCurStage();
	stageLabel->setString(intToStr(curStage));
}

void PreStageScene::initBottomLayout()
{
	CCMenuItem *startGameBtn = dynamic_cast<CCMenuItem *>((m_bottomLayout->getChildById(1)));
	startGameBtn->setTarget(this, menu_selector(PreStageScene::toStartGame));
}

void PreStageScene::toStartGame(cocos2d::CCObject* pSender)
{
	SoundMgr::theMgr()->playEffect(kEffectMusicButton);
	if (UserInfo::theInfo()->consumeStrength())
	{
		PreStageModel::theModel()->confirmCurPets();
		MainScene::theScene()->showPanel(kStageView);
		MainScene::theScene()->clearPanelRecord();
	}
}

void PreStageScene::hideHomeBackBtn()
{
	m_titlePanel->setUiVisible(kTitlePanelBackHome, false);
}