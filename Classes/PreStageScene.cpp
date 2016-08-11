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
#include "PackageDialog.h"
#include "PackageModel.h"
#include "GameBackEndState.h"
#include "MyPurchase.h"
#include "CCFunctionAction.h"
#include "NoTouchLayer.h"
USING_NS_CC;
using namespace std;
using namespace CommonUtil;

bool PreStageScene::init()
{
	PreStageModel::theModel()->init();
	StarsController::theModel()->resetStage(m_usage);

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

	auto scaleLarge = CCScaleTo::create(0.5f, 1.2f);
	auto scaleSmall = CCScaleTo::create(0.6f, 0.8f);
	auto scaleNormal = CCScaleTo::create(0.4f, 1.0f);
	auto scale = CCRepeatForever::create(CCSequence::create(scaleLarge, scaleSmall, scaleNormal, NULL));
	startGameBtn->runAction(scale);
}

void PreStageScene::runStartGameAction()
{
	auto noTouchLayer = NoTouchLayer::create();
	noTouchLayer->setCanTouch(false);
	addChild(noTouchLayer);

	auto sourcePos = m_titlePanel->getWidgetPos(kTitlePanelStrength);
	auto targetPos = m_bottomLayout->convertToWorldSpace(m_bottomLayout->getChildById(2)->getPosition());
	auto spr = CCSprite::create("common/title_strength.png");
	spr->setPosition(convertToNodeSpace(sourcePos));
	auto move = CCEaseExponentialInOut::create(CCMoveTo::create(1.0f, convertToNodeSpace(targetPos)));
	auto func = CCFunctionAction::create([=]()
	{
		PreStageModel::theModel()->confirmCurPets();
		MainScene::theScene()->showPanel(kStageView);
		MainScene::theScene()->clearPanelRecord();
		GameBackEndState::theModel()->recordStageStart();
	});
	spr->runAction(CCSequence::create(move, func, NULL));
	addChild(spr);
}

void PreStageScene::toStartGame(cocos2d::CCObject* pSender)
{
	SoundMgr::theMgr()->playEffect(kEffectMusicButton);
	if (UserInfo::theInfo()->consumeStrength())
	{
		runStartGameAction();
	}
	else
	{
		CCLOG("not enough strength");
		MyPurchase::sharedPurchase()->showToast(kToastTextNotEnoughStrength);
		auto dialog = PackageDialog::create(kPackageStrength);
		MainScene::theScene()->showDialog(dialog);
	}
}

void PreStageScene::hideHomeBackBtn()
{
	m_titlePanel->setUiVisible(kTitlePanelBackHome, false);
}

void PreStageScene::onBackKeyTouched()
{
	MainScene::theScene()->clearPanelRecord();
	MainScene::theScene()->showPanel(kMainMenu);
}