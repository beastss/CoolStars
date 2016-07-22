#include "StageUiLayer.h"
#include "VisibleRect.h"
#include "UserInfo.h"
#include "GameData.h"
#include "SoundMgr.h"
#include "MenuScene.h"
#include "PauseDialog.h"
#include "PetManager.h"
#include "StageOperator.h"
#include "CommonUtil.h"
#include "GameResultLayer.h"
#include "PropManager.h"
#include "PropItemView.h"
#include "UiLayout.h"
#include "EmptyBox.h"
#include "StagePetNode.h"
#include "StagePanelUtil.h"
#include "MainScene.h"
#include "StageSceneState.h"
#include "StageTarget.h"
#include "StageTargetView.h"
#include "CommonMacros.h"
#include "StageDataMgr.h"
#include "CCFunctionAction.h"
#include "StageScene.h"
#include "StarsController.h"
#include "StarViewNode.h"
#include <algorithm>
#include "StageTargetPanel.h"
#include "PackageModel.h"
#include "NoTouchLayer.h"
#include "StageBonusView.h"
#include "PackageDialog.h"
USING_NS_CC;
using namespace std;
using namespace CommonUtil;
StageUiLayer::StageUiLayer()
{
	m_stateOwner = StageScene::theScene()->getStateOwner();
}

StageUiLayer::~StageUiLayer(void)
{
}

StageUiLayer *StageUiLayer::create()
{
	StageUiLayer* layer = new StageUiLayer();
	layer->init();
	layer->autorelease();
	return layer;
}

void StageUiLayer::onEnter()
{
	CCNode::onEnter();
	StageDataMgr::theMgr()->addView(this);
	StarsController::theModel()->addView(this);
	StageLayersMgr::theMgr()->addLayer(this);
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, kStageUiTouchPriority, true);
}

void StageUiLayer::onExit()
{
	CCNode::onExit();
	StageDataMgr::theMgr()->removeView(this);
	StarsController::theModel()->removeView(this);
	StageLayersMgr::theMgr()->removeLayer(this);
	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);

}

bool StageUiLayer::init()
{
	auto winSize = CCDirector::sharedDirector()->getWinSize();
	setContentSize(winSize);

	m_bottomUi = UiLayout::create("layout/stage_bottom.xml");
	m_bottomUi->setAnchorPoint(ccp(0, 0));
	m_bottomUi->setPosition(ccp(0, 0));
	m_bottomUi->setMenuTouchPriority(kStageUiTouchPriority);
	addChild(m_bottomUi);

	m_topUi = UiLayout::create("layout/stage_top.xml");
	m_topUi->setAnchorPoint(ccp(0, 1));
	m_topUi->setPosition(ccp(0, winSize.height));
	m_topUi->setMenuTouchPriority(kStageUiTouchPriority);
	addChild(m_topUi);

	initTopUi();
	initPets();
	initBottomUi();
	showTargetPanel();

	m_noTouchLayer = NoTouchLayer::create();
	addChild(m_noTouchLayer);
    return true;
}

void StageUiLayer::initTopUi()
{
	int targetBoxIds[] = { 18, 19, 20 };
	auto target = StarsController::theModel()->getStageTarget();
	auto leftTarget = target->getEraseStarsLeft();
	assert(leftTarget.size() <= 3);
	for (size_t i = 0; i < leftTarget.size(); ++i)
	{
		StageTargetView *view = StageTargetView::create(leftTarget[i]);
		auto node = dynamic_cast<EmptyBox *>(m_topUi->getChildById(targetBoxIds[i]));
		node->setNode(view);
	}
	
	m_scoreProgress = CCProgressTimer::create(CCSprite::create("stage/ui/yxjm_fenshucao2.png"));
	m_scoreProgress->setType(kCCProgressTimerTypeBar);
	m_scoreProgress->setMidpoint(ccp(0, 1));
	m_scoreProgress->setBarChangeRate(ccp(1, 0));
	m_scoreProgress->setPercentage(0);
	auto box = dynamic_cast<EmptyBox *>(m_topUi->getChildById(22));
	box->setNode(m_scoreProgress);

	onStepsChanged();
	onScoreChanged();
}

void StageUiLayer::initPets()
{
	int uiIds[] = { 10, 11, 12, 13};
	auto ids = PetManager::petMgr()->getCurPetIds();
	assert(ids.size() <= 4);
	for (size_t i = 0; i < ids.size(); ++i)
	{
		StagePetNode *petNode = StagePetNode::create(ids[i], kStageUiTouchPriority);
		petNode->setTouchHandle(bind(&StageUiLayer::handlePetClicked, this, placeholders::_1));
		auto node = dynamic_cast<EmptyBox *>((m_topUi->getChildById(uiIds[i])));
		node->setNode(petNode);
		node->setAnchorPoint(ccp(0.5f, 0.5f));
		m_petViews[ids[i]] = petNode;
	}
}

void StageUiLayer::initBottomUi()
{
	auto closeBtn = dynamic_cast<CCMenuItem *>((m_bottomUi->getChildById(2)));
	closeBtn->setTarget(this, menu_selector(StageUiLayer::onPauseBtnClicked));

	for (int i = kPropReorder; i < kPorpTypeAmount; i++)
	{
		auto box = dynamic_cast<EmptyBox *>((m_bottomUi->getChildById(12 + i)));
		auto node = PropItemView::create(i, kStageUiTouchPriority);
		node->setTouchHandle(bind(&StageUiLayer::handlePropsItemClicked, this, placeholders::_1));
		node->setAnchorPoint(ccp(0.5f, 0.5f));
		box->setNode(node);
	}

	m_bottomUi->getChildById(15)->setVisible(false);
}

void StageUiLayer::showTargetPanel()
{
	vector<CCPoint> targetsPos;
	for (int i = 0; i < 3; ++i)
	{
		auto node = m_topUi->getChildById(18 + i);
		auto pos = m_topUi->convertToWorldSpace(node->getPosition());
		targetsPos.push_back(pos);
	}
	StageTargetPanel *panel = StageTargetPanel::create();
	auto size = getContentSize();
	panel->loadTargetPos(targetsPos);
	panel->setAnchorPoint(ccp(0.5f, 0.5f));
	panel->setPosition(ccpMult(size, 0.5f));
	addChild(panel, 1);
}

void StageUiLayer::showGameOverHint()
{
	//game over hint
	CCSprite *sp = CCSprite::create("stage_clear.png");
	this->addChild(sp);
}

void StageUiLayer::handlePetClicked(int petId)
{
	m_stateOwner->clickPet(petId);
}

void StageUiLayer::showPetsSkillPanel()
{
	StagePetSkillPanel *panel = StagePetSkillPanel::create(kStageUiTouchPriority - 1);
	auto node = dynamic_cast<EmptyBox *>((m_topUi->getChildById(14)));
	node->setNode(panel);
	node->setAnchorPoint(ccp(0, 1));
}

void StageUiLayer::refreshRedPackage()
{
	if (m_redPackage.isShowUp())
	{
		auto node = m_bottomUi->getChildById(15);
		if (!node->isVisible())
		{
			m_noTouchLayer->setCanTouch(false);
			auto targetPos = node->getPosition();
			auto sourcePos = ccpMult(getContentSize(), 0.5f);
			node->setPosition(sourcePos);
			auto spawn = CCSpawn::create(CCFadeIn::create(1.0f), CCScaleTo::create(1.0f, 1.5f), NULL);
			auto move = CCEaseExponentialInOut::create(CCMoveTo::create(0.4f, targetPos));
			auto toNoraml = CCScaleTo::create(0.3f, 1.0f);
			auto func = CCFunctionAction::create([=]()
			{
				m_noTouchLayer->setCanTouch(true);
			});
			node->setVisible(true);
			node->runAction(CCSequence::create(spawn, move, toNoraml, func, NULL));
		}
	}
}

void StageUiLayer::onStepsChanged()
{
	auto stageInfo = StageDataMgr::theMgr();
	int leftStep = stageInfo->getLeftSteps();
	CCLabelAtlas *stepLabel = dynamic_cast<CCLabelAtlas *>(m_topUi->getChildById(17));
	stepLabel->setString(intToStr(leftStep));
}

void StageUiLayer::onScoreChanged()
{
	auto stageInfo = StageDataMgr::theMgr();
	int curScore = stageInfo->getCurScore();
	auto stageTarget = StarsController::theModel()->getStageTarget();
	int targetScore = stageTarget->getTargetScore();

	CCLabelAtlas * curScoreLabel = dynamic_cast<CCLabelAtlas *>(m_topUi->getChildById(15));
	curScore = min(curScore, targetScore);
	string str = intToStr(curScore);
	str += ";";
	str += intToStr(targetScore);
	curScoreLabel->setString(str.c_str());
	refreshRedPackage();

	if (m_scoreProgress->getPercentage() < 99.9f)
	{
		m_scoreProgress->runAction(CCProgressTo::create(0.5f, ((float)curScore / targetScore) * 100));
	}
}

void StageUiLayer::onCoinsChanged()
{

}

void StageUiLayer::onShowGameResult(int isWon)
{
	CCNode *node = NULL;
	if (isWon)
	{
		MainScene::theScene()->showPanel(kStageWinPanel);
	}
	else
	{
		MainScene::theScene()->showPanel(kStageFailPanel);
	}
	
}

void StageUiLayer::onRunOutSteps()
{
	auto dialog = PackageDialog::create(kPackageStep);
	MainScene::theScene()->showDialog(dialog);
	dialog->setCancelHandle([=]()
	{
		StarsController::theModel()->gameOver(false);
	});
}

void StageUiLayer::onGameWin()
{
	for (auto iter = m_petViews.begin(); iter != m_petViews.end(); ++iter)
	{
		iter->second->playHappyAction(true);
	}
	m_noTouchLayer->setCanTouch(false);

	auto spr = CCSprite::create("stage/game_win/sljm_logo.png");
	auto size = getContentSize();
	spr->setPosition(ccpMult(size, 0.5f));
	auto func = CCFunctionAction::create([=]()
	{
		spr->removeFromParent();
		StageOp->gameOverRandomReplace();
	});
	spr->setScale(0.8f);
	auto scaleLarge = CCScaleTo::create(0.3f, 1.4f);
	auto scaleNormal = CCScaleTo::create(0.3f, 1.0f);
	auto delay1 = CCDelayTime::create(0.4f);
	auto fadeOut = CCFadeOut::create(0.7f);
	auto delay2 = CCDelayTime::create(0.5f);
	spr->runAction(CCSequence::create(scaleLarge, scaleNormal, delay1, fadeOut, delay2, func, NULL));
	addChild(spr);
}

void StageUiLayer::onPauseBtnClicked(CCObject *pSender)
{
	SoundMgr::theMgr()->playEffect(kEffectMusicButton);
	MainScene::theScene()->showDialog(PauseDialog::create());
}

void StageUiLayer::showChangeColorPanel(int myColor, const LogicGrid &grid)
{
	auto *panel = ChangeStarColorPanel::create(myColor, kStageUiTouchPriority - 1);
	panel->setCallback([=](bool isSelect, int color)
	{
		if (isSelect)
		{
			PropManager::propMgr()->usePropBrush(grid, color);
		}
		m_stateOwner->enterNormalState();
	});

	panel->setAnchorPoint(ccp(0.5f, 1));
	auto winSize = CCDirector::sharedDirector()->getWinSize();
	panel->setPosition(ccpMult(winSize, 0.5f));

	addChild(panel);
}

void StageUiLayer::handlePropsItemClicked(int type)
{
	int amount = PropManager::propMgr()->getPropItemAmount(type);
	if (amount > 0)
	{
		m_stateOwner->clickProp(type);
	}
	else
	{
		SoundMgr::theMgr()->playEffect(kEffectMusicButton);
		auto dialog = PackageDialog::create(kPackageProps);
		MainScene::theScene()->showDialog(dialog);
	}
}

void StageUiLayer::playExplosionAction(CCPoint pos)
{
	static const string resPath = "stage/star_explosion/common/baozatexiao.ExportJson";
	string armatureName = "baozatexiao";

	CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo(resPath.c_str());
	auto armature = CCArmature::create(armatureName.c_str());
	armature->getAnimation()->setMovementEventCallFunc(this, SEL_MovementEventCallFunc(&StageUiLayer::removeExplosionAnimation));
	armature->getAnimation()->playWithIndex(0);
	addChild(armature);
	armature->setPosition(pos);
}

void StageUiLayer::removeExplosionAnimation(cocos2d::extension::CCArmature *animation, cocos2d::extension::MovementEventType, const char *)
{
	animation->removeFromParent();
}

void StageUiLayer::onNormalStarErased(cocos2d::CCPoint pos, int starType, int color)
{
	if (starType == kColorStar)
	{
		const static float kDuration = 0.8f;
		for (auto iter = m_petViews.begin(); iter != m_petViews.end(); ++iter)
		{
			int petId = iter->first;
			auto petView = iter->second;
			if (petView->getColor() == color)
			{
				auto config = DataManagerSelf->getStarsColorConfig(color);
				auto resPath = config.colorStarRes;
				CCSprite *starSpr = CCSprite::create(resPath.c_str());
				addChild(starSpr);
				starSpr->setPosition(pos);

				auto func = CCFunctionAction::create([=]()
				{
					starSpr->removeFromParent();
					int oldEnergy = PetManager::petMgr()->getPetById(petId)->getPetData().energy;
					int value = DataManagerSelf->getSystemConfig().starPetEnergy;
					PetManager::petMgr()->addPetEnergy(petId, value);
				});
				auto targetPos = petView->getParent()->convertToWorldSpace(petView->getPosition());
				auto move = CCEaseExponentialIn::create(CCMoveTo::create(kDuration, targetPos));
				auto scaleTo = CCScaleTo::create(kDuration, 0.5f);
				auto rotateBy = (CCRotateBy::create(kDuration, 360 * 10));
				starSpr->runAction(CCSequence::create(
					CCSpawn::create(move, scaleTo, rotateBy, NULL),
					func, NULL));
			}
		}
	}
	
	playExplosionAction(pos);
}

void StageUiLayer::onHighLightPets(const std::vector<int> &petIds)
{
	for (auto iter = m_petViews.begin(); iter != m_petViews.end(); ++iter)
	{
		if (find(petIds.begin(), petIds.end(), iter->first) != petIds.end())
		{
			//隐藏需要高亮的宠物,在stagemasklayer里高亮宠物
			iter->second->setVisible(false);
		}
	}
}

unordered_map<int, cocos2d::CCPoint> StageUiLayer::getPetViewsInfo()
{
	unordered_map<int, CCPoint> info;
	for (auto iter = m_petViews.begin(); iter != m_petViews.end(); ++iter)
	{
		auto petView = iter->second;
		auto pos = petView->getParent()->convertToWorldSpace(petView->getPosition());
		info[iter->first] = pos;
	}
	return info;
}

void StageUiLayer::onToNormalState()
{
	//显示pets
	for (auto iter = m_petViews.begin(); iter != m_petViews.end(); ++iter)
	{
		iter->second->setVisible(true);
	}
}

void StageUiLayer::showPetSpreadStarsAction(int petId, const StarAttr &attr, function<void()> callback)
{
	auto iter = m_petViews.find(petId);
	if (iter != m_petViews.end())
	{
		static const float kDutation = 0.3f;

		auto starNode = StarsController::theModel()->getStarNode(attr.grid);
		auto starView = starNode->getView();
		auto targetPos = starView->getParent()->convertToWorldSpace(starView->getPosition());

		auto tempNode = StarNode::createNodeFatory(attr);
		auto petView = iter->second;
		auto sourcePos = petView->getParent()->convertToWorldSpace(petView->getPosition());

		CCSprite *starImg = CCSprite::create(tempNode->getResPath().c_str());
		starImg->setPosition(sourcePos);
		addChild(starImg);
		delete tempNode;

		auto moveTo = CCMoveTo::create(kDutation, targetPos);
		auto func = CCFunctionAction::create([=]()
		{
			starImg->removeFromParent();
			if (callback) callback();
		});
		starImg->runAction(CCSequence::create(CCEaseExponentialInOut::create(moveTo), func, NULL));
	}
}

void StageUiLayer::gameOverSpreadStars(const GoodsData &data, const LogicGrid &targetGrid, std::function<void()> callback)
{
	static const float kDutation = 0.3f;

	auto starNode = StarsController::theModel()->getStarNode(targetGrid);
	auto starView = starNode->getView();
	auto targetPos = starView->getParent()->convertToWorldSpace(starView->getPosition());

	auto sourcePos = m_topUi->convertToWorldSpace(m_topUi->getChildById(17)->getPosition());
	CCSprite *shootingStar = CCSprite::create("stage/ui/yxjm_xingxing.png");
	shootingStar->setPosition(sourcePos);
	addChild(shootingStar);
	
	auto moveTo = CCMoveTo::create(kDutation, targetPos);
	auto func = CCFunctionAction::create([=]()
	{
		auto view = StageBonusView::create(data);
		view->setAnchorPoint(ccp(0.5f, 0.5f));
		view->scaleAndFadeOut();
		addChild(view);
		view->setPosition(targetPos);

		shootingStar->removeFromParent();
		if (callback) callback();
	});
	shootingStar->runAction(CCSequence::create(CCEaseExponentialInOut::create(moveTo), func, NULL));
}

bool StageUiLayer::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
	auto node = m_bottomUi->getChildById(15);
	auto pos = m_bottomUi->convertToNodeSpace(pTouch->getLocation());
	if (node->boundingBox().containsPoint(pos) && node->isVisible())
	{
		SoundMgr::theMgr()->playEffect(kEffectMusicButton);

		CCSprite *spr = dynamic_cast<CCSprite *>(node);
		spr->initWithFile("stage/ui/yxjm_hongbao2.png");
		m_redPackage.openRedPackage();

		auto func = CCFunctionAction::create([=]()
		{
			spr->setVisible(false);
		});
		spr->runAction(CCSequence::create(CCDelayTime::create(0.5f), func, NULL));
		return true;
	}
	return false;
}

void StageUiLayer::onScoreBouble()
{
	auto spr = CCSprite::create("stage/pets/yxjm_defenshuangbei.png");
	auto winSize = CCDirector::sharedDirector()->getWinSize();
	spr->setPosition(ccpMult(winSize, 0.5f));
	addChild(spr);
	auto func = CCFunctionAction::create([=]()
	{
		spr->removeAllChildren();
	});
	spr->runAction(CCSequence::create(CCFadeIn::create(0.5f), CCDelayTime::create(1.0f), CCFadeOut::create(0.6f), func, NULL));
}

void StageUiLayer::onTouchEnable(bool canTouch)
{
	m_noTouchLayer->setCanTouch(canTouch);
}

void StageUiLayer::onEraseStarsStart()
{
	m_noTouchLayer->setCanTouch(false, 1);
}

void StageUiLayer::onEraseStarsEnd()
{
	auto func = CCFunctionAction::create([=]()
	{
		m_noTouchLayer->setCanTouch(true, 1);
	});
	runAction(CCSequence::create(CCDelayTime::create(0.5f), func, NULL));
}