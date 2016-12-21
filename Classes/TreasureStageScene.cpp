#include "TreasureStageScene.h"
#include "UiLayout.h"
#include "SoundMgr.h"
#include "EmptyBox.h"
#include "GuideMgr.h"
#include "CommonUtil.h"
#include "CommonMacros.h"
#include "MainScene.h"
#include "TreasureStarsLayer.h"
#include "TreasureStageModel.h"
#include "TreasureStageState.h"
#include "TreasureStageResultDialog.h"
#include "DataManager.h"
#include "TreasureStageDataMgr.h"
#include "TreasureStageTargetPanel.h"
#include "CCFunctionAction.h"
#include "NoTouchLayer.h"
#include "ActionRunner.h"
#include "PetManager.h"
#include "StagePetNode.h"
#include "TreasurePauseDialog.h"
#include "TreasureSkipStageDialog.h"
#include "UserInfo.h"

USING_NS_CC;
using namespace std;
using namespace CommonUtil;

TreasureStageScene::TreasureStageScene()
: m_uiScore(0)
{
	TreasureStageModel::theModel()->startStage();
	m_runner = ActionRunner::create();
	m_runner->retain();
}

TreasureStageScene::~TreasureStageScene()
{
	TreasureStageModel::theModel()->endStage();
	m_runner->clear();
	m_runner->release();
}

void TreasureStageScene::onEnter()
{
	BasePanel::onEnter();
	TreasureStageNotifier::theNotifier()->addView(this);
	SoundMgr::theMgr()->playBackground(kBackGroundMusicStage);

}

void TreasureStageScene::onExit()
{
	BasePanel::onExit();
	TreasureStageNotifier::theNotifier()->removeView(this);
	SoundMgr::theMgr()->playBackground(kBackGroundMusicMainMenu);
}

bool TreasureStageScene::init()
{
	setPanelId(kTreasureStagePanel);

	m_noTouchLayer = NoTouchLayer::create();
	addChild(m_noTouchLayer);

	auto winSize = CCDirector::sharedDirector()->getWinSize();
	setContentSize(winSize);

	m_starsLayer = TreasureStarsLayer::create();
	m_starsLayer->setAnchorPoint(ccp(0.5f, 0));
	m_starsLayer->setPosition(ccp(winSize.width * 0.5f, 0));
	addChild(m_starsLayer);

	m_topLayout = UiLayout::create("layout/treasure_stage_top.xml");
	m_topLayout->setAnchorPoint(ccp(0.0f, 1.0f));
	m_topLayout->setPosition(ccp(0, winSize.height));
	m_topLayout->setMenuTouchPriority(m_touchPriority);
	addChild(m_topLayout);
	initTopLayout();

	m_mainLayout = UiLayout::create("layout/stage.xml");
	m_mainLayout->setMenuTouchPriority(m_touchPriority);
	m_mainLayout->setAnchorPoint(ccp(0.5f, 0.5f));
	m_mainLayout->setPosition(ccpMult(winSize, 0.5f));
	initMainLayout();
	//addChild(m_mainLayout);

	m_bottomLayout = UiLayout::create("layout/tressure_stage_bottom.xml");
	m_bottomLayout->setMenuTouchPriority(m_touchPriority);
	addChild(m_bottomLayout);
	initBottomLayout();

	m_effectLayer = CCNode::create();
	addChild(m_effectLayer);

	onNewStage();
	scheduleUpdate();
	GuideMgr::theMgr()->startGuide(kGuideStart_treasure_in);
	return true;
}

void TreasureStageScene::initMainLayout()
{
	
}

void TreasureStageScene::initTopLayout()
{
	initPets();
}

void TreasureStageScene::initBottomLayout()
{
	CCMenuItem *backBtn = dynamic_cast<CCMenuItem *>((m_bottomLayout->getChildById(2)));
	backBtn->setTarget(this, menu_selector(TreasureStageScene::onBackBtnClicked));
}

void TreasureStageScene::onBackBtnClicked(cocos2d::CCObject* pSender)
{
	SoundMgr::theMgr()->playEffect(kEffectMusicButton);
	auto dialog = TreasurePauseDialog::create();
	MainScene::theScene()->showDialog(dialog);
}

void TreasureStageScene::initPets()
{
	int uiIds[] = { 10, 11, 12, 13, 14};
	auto ids = PetManager::petMgr()->getOwnedStagePetIds(false);
	m_noTouchLayer->setCanTouch(false);
	m_runner->queueAction(DelayAction::withDelay(1.0f));
	for (size_t i = 0; i < ids.size(); ++i)
	{
		int uiId = uiIds[i];
		int petId = ids[i];
		m_runner->queueAction(CallFuncAction::withFunctor([=]()
		{
			CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo("stage/pets/animation/Youxitexiao.ExportJson");
			auto armature = CCArmature::create("Youxitexiao");
			armature->getAnimation()->playWithIndex(0);
			auto box = m_topLayout->getChildById(uiId);
			auto pos = convertToNodeSpace(box->getParent()->convertToWorldSpace(box->getPosition()));
			armature->setPosition(pos);
			armature->getAnimation()->setMovementEventCallFunc(this, SEL_MovementEventCallFunc(&TreasureStageScene::bubbleFinished));
			addChild(armature);
		}));

		m_runner->queueAction(DelayAction::withDelay(0.3f));
		m_runner->queueAction(CallFuncAction::withFunctor([=]()
		{
			StagePetNode *petNode = StagePetNode::create(petId, kStageUiTouchPriority);
			petNode->setIconVisible(false);
			petNode->setScale(0.7f);
			auto node = dynamic_cast<EmptyBox *>((m_topLayout->getChildById(uiId)));
			node->setNode(petNode);
			node->setAnchorPoint(ccp(0.5f, 0.5f));
		}));
		m_runner->queueAction(DelayAction::withDelay(0.3f));
	}
	m_runner->queueAction(CallFuncAction::withFunctor([=]()
	{
		m_noTouchLayer->setCanTouch(true);
	}));
}

void TreasureStageScene::bubbleFinished(cocos2d::extension::CCArmature *armature, cocos2d::extension::MovementEventType, const char *)
{
	armature->removeFromParent();
}

void TreasureStageScene::hideHomeBackBtn()
{
}

void TreasureStageScene::onBackKeyTouched()
{

}

void TreasureStageScene::onPassCurStage(int stage)
{
	SoundMgr::theMgr()->playEffect(kEffectGameWin);

	auto spr = CCSprite::create("treasure_stage/tips/pic_6.png");
	auto size = getContentSize();
	spr->setPosition(ccpMult(size, 0.5f));
	auto func = CCFunctionAction::create([=]()
	{
		spr->removeFromParent();
	});
	spr->setScale(0.8f);
	auto scaleLarge = CCScaleTo::create(1.5f, 1.6f);
	auto scaleNormal = CCScaleTo::create(1.0f, 1.0f);
	auto delay1 = CCDelayTime::create(0.5f);
	auto fadeOut = CCFadeOut::create(1.0f);
	auto delay2 = CCDelayTime::create(0.5f);
	spr->runAction(CCSequence::create(scaleLarge, scaleNormal, delay1, fadeOut, delay2, func, NULL));
	m_effectLayer->addChild(spr, 1);
}

void TreasureStageScene::onStageOverStart(int stage, bool won)
{
	m_noTouchLayer->setCanTouch(false);
}

void TreasureStageScene::onStageOverEnd(int stage, bool won)
{
	if (won)
	{
		TreasureStageModel::theModel()->nextStage();
	}
	else
	{
		auto dialog = TreasureSkipStageDialog::create();
		MainScene::theScene()->showDialog(dialog);
	}

	m_noTouchLayer->setCanTouch(true);
}

void TreasureStageScene::update(float dt)
{
	int curScore = TreasureStageState::theState()->getCurScore();
	if (m_uiScore < curScore)
	{
		const int kMinStep = 1;
		const int kMaxStep = 100;
		int step = max(kMinStep, min(kMaxStep, (curScore - m_uiScore) / 10));
		m_uiScore += step;
	}
	m_uiScore = min(m_uiScore, curScore);
	CCLabelAtlas*curScoreLabel = dynamic_cast<CCLabelAtlas *>(m_topLayout->getChildById(15));
	curScoreLabel->setString(CommonUtil::intToStr(m_uiScore));
}

void TreasureStageScene::onEraseStars(int num, int score)
{
	if (num <= 1) return;

	//连消分数
	const int kTagBouns = 10;
	m_effectLayer->removeChildByTag(kTagBouns);
	auto text = DataManagerSelf->getText("erase_scores");
	auto layout = UiLayout::create("layout/tressure_stage_erase_hint.xml");
	layout->setAnchorPoint(ccp(0.5f, 0.5f));
	auto numLabel = dynamic_cast<CCLabelAtlas *>(layout->getChildById(2));
	numLabel->setString(CommonUtil::intToStr(num));
	auto scoreLabel = dynamic_cast<CCLabelAtlas *>(layout->getChildById(3));
	scoreLabel->setString(CommonUtil::intToStr(score));
	auto seq = CCSequence::create(CCDelayTime::create(1.5f), CCHide::create(), NULL);
	layout->runAction(seq);
	auto pos = m_topLayout->convertToWorldSpace(m_topLayout->getChildById(42)->getPosition());
	pos = m_effectLayer->convertToNodeSpace(pos);
	layout->setPosition(pos);
	m_effectLayer->addChild(layout, 0, kTagBouns);

	//连消称赞
	const int kTagTips = 20;
	auto tipsPath = TreasureStageDataMgrSelf->getEraseBonusData(num).tipsPath;
	if (!tipsPath.empty())
	{
		m_effectLayer->removeChildByTag(kTagTips);
		CCSprite *tipsImg = CCSprite::create(tipsPath.c_str());
		seq = CCSequence::create(CCBlink::create(1.0f, 6), CCDelayTime::create(0.5f), CCHide::create(), NULL);
		tipsImg->runAction(seq);
		auto size = getContentSize();
		tipsImg->setPosition(ccpMult(size, 0.5f));
		m_effectLayer->addChild(tipsImg, 0, kTagTips);
	}
}

void TreasureStageScene::onNewStage()
{
	//topUi
	int curStage = TreasureStageState::theState()->getCurStage();
	CCLabelAtlas *curScoreLabel = dynamic_cast<CCLabelAtlas *>(m_topLayout->getChildById(30));
	curScoreLabel->setString(CommonUtil::intToStr(curStage));

	int targetScore = TreasureStageState::theState()->getTargeScore();
	CCLabelAtlas *curTargetScoreLabel = dynamic_cast<CCLabelAtlas *>(m_topLayout->getChildById(40));
	curTargetScoreLabel->setString(CommonUtil::intToStr(targetScore));

	int foodReward = TreasureStageState::theState()->getCurFoodReward();
	CCLabelAtlas *foodRewardLabel = dynamic_cast<CCLabelAtlas *>(m_topLayout->getChildById(16));
	foodRewardLabel->setString(CommonUtil::intToStr(foodReward));

	int diamondReward = TreasureStageState::theState()->getCurDiamondReward();
	CCLabelAtlas *diamondRewardLabel = dynamic_cast<CCLabelAtlas *>(m_topLayout->getChildById(18));
	diamondRewardLabel->setString(CommonUtil::intToStr(diamondReward));

	//starlayer
	m_starsLayer->refresh();
	m_effectLayer->removeAllChildren();
	showTargetPanel();
	m_noTouchLayer->setCanTouch(true);
}

void TreasureStageScene::onStarExplore(int color, int score, cocos2d::CCPoint pt)
{
	/*
	CCLabelTTF *label = CCLabelTTF::create(CommonUtil::intToStr(score), "Arial", 20);
	label->setPosition(m_effectLayer->convertToNodeSpace(pt));
	auto targetPos = m_topLayout->convertToWorldSpace(m_topLayout->getChildById(15)->getPosition());
	targetPos = m_effectLayer->convertToNodeSpace(targetPos);
	auto seq = CCSequence::create(CCMoveTo::create(1.0f, targetPos), CCRemoveSelf::create(), NULL);
	label->runAction(seq);
	m_effectLayer->addChild(label);
	*/

	float kDuration = 1.0f;
	auto starSpr = CCSprite::create(TreasureStageDataMgrSelf->getStarData(color).starPath.c_str());
	starSpr->setPosition(m_effectLayer->convertToNodeSpace(pt));
	auto targetPos = m_topLayout->convertToWorldSpace(m_topLayout->getChildById(15)->getPosition());
	targetPos = m_effectLayer->convertToNodeSpace(targetPos);
	auto move = CCEaseExponentialInOut::create(CCMoveTo::create(kDuration, targetPos));
	auto scaleTo = CCScaleTo::create(kDuration, 0.5f);
	auto rotateBy = (CCRotateBy::create(kDuration, 360 * 10));
	starSpr->runAction(CCSequence::create(
		CCSpawn::create(move, scaleTo, rotateBy, NULL),
		CCRemoveSelf::create(), NULL));
	m_effectLayer->addChild(starSpr);
}

void TreasureStageScene::showTargetPanel()
{
	auto targetPanel = TreasureStageTargetPanel::create();
	auto size = getContentSize();
	targetPanel->setAnchorPoint(ccp(0.5f, 0.5f));
	targetPanel->setPosition(ccpMult(size, 0.5f));
	m_effectLayer->addChild(targetPanel);
}