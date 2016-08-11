#include "GameResultLayer.h"
#include "StageScene.h"
#include "MenuScene.h"
#include "StarsController.h"
#include "MainScene.h"
#include "UiLayout.h"
#include "StageDataMgr.h"
#include "CommonUtil.h"
#include "StageTarget.h"
#include "DataManager.h"
#include "UserInfo.h"
#include "PetManager.h"
#include "CCFunctionAction.h"
#include "LotteryScene.h"
#include "SoundMgr.h"
#include "StagePanelUtil.h"
using namespace std;
USING_NS_CC;

bool GameResultLayer::init(string xmlFile)
{
	setPanelId(kStageFailPanel);

	m_layout = UiLayout::create(xmlFile.c_str());
	initRewardData();
	saveRewards();
	initLayout();

	addChild(m_layout);
	setContentSize(m_layout->getContentSize());
	return true;
}

void GameResultLayer::onEnter()
{
	CCNode::onEnter();
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, m_touchPriority, true);
}

void GameResultLayer::onExit()
{
	CCNode::onExit();
	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
}

void GameResultLayer::initLayout()
{
	CCMenuItem *confirmBtn = dynamic_cast<CCMenuItem *>((m_layout->getChildById(40)));
	confirmBtn->setTarget(this, menu_selector(GameResultLayer::onConfirmBtnClicked));

	CCLabelAtlas *stageNum = dynamic_cast<CCLabelAtlas *>(m_layout->getChildById(26));
	int curStage = StageDataMgr::theMgr()->getCurStage();
	stageNum->setString(CommonUtil::intToStr(curStage));

	auto stageTarget = StarsController::theModel()->getStageTarget();
	CCLabelAtlas *foodNum = dynamic_cast<CCLabelAtlas *>(m_layout->getChildById(30));
	foodNum->setString(CommonUtil::intToStr(m_reward.food));

	CCLabelAtlas *diamondNum = dynamic_cast<CCLabelAtlas *>(m_layout->getChildById(31));
	diamondNum->setString(CommonUtil::intToStr(m_reward.diamond));

	CCLabelAtlas *keyNum = dynamic_cast<CCLabelAtlas *>(m_layout->getChildById(32));
	keyNum->setString(CommonUtil::intToStr(m_reward.key));

	int runeStoneNum = UserInfo::theInfo()->getRuneStone();
	runeStoneNum = min(max(1, runeStoneNum), MAX_RUNESTORN_AMOUNT);
	for (int i = 0; i < 5; ++i)
	{
		auto node = m_layout->getChildById(21 + i);
		node->setVisible(i < runeStoneNum);
	}
	//最后一个符石放大动画
	auto lastRuneStone = m_layout->getChildById(20 + runeStoneNum);
	auto scaleLarge = CCScaleTo::create(0.5f, 2.5f);
	auto scaleSmall = CCScaleTo::create(0.6f, 0.8f);
	auto scaleNormal = CCScaleTo::create(0.4f,1.0f);
	lastRuneStone->runAction(CCSequence::create(scaleLarge, scaleSmall, scaleNormal, NULL));

	//符石达到最大数量后 转光效
	if (runeStoneNum == MAX_RUNESTORN_AMOUNT)
	{
		auto rotate = CCRepeatForever::create(CCRotateBy::create(3.0f, 180));
		m_layout->getChildById(5)->runAction(rotate);
	}
	else
	{
		m_layout->getChildById(5)->setVisible(false);
	}
}

void GameResultLayer::initRewardData()
{
	m_reward = StageDataMgr::theMgr()->getResultBonus();
	StageDataMgr::theMgr()->resetResultBonus();

	auto stageTarget = StarsController::theModel()->getStageTarget();
	m_reward.diamond += stageTarget->getErasedStarAmount(kDiamond);
	m_reward.key += stageTarget->getErasedStarAmount(kKey);
	
	addGameResultReward();
}

void GameResultLayer::onConfirmBtnClicked(CCObject *pSender)
{
	SoundMgr::theMgr()->playEffect(kEffectMusicButton);
	onConfirm();
}

bool GameResultLayer::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
	auto pos = m_layout->convertToNodeSpace(pTouch->getLocation());
	auto node = dynamic_cast<CCSprite *>(m_layout->getChildById(8));
	bool canGetReward = UserInfo::theInfo()->canGetRuneStoneReward();
	if (node->boundingBox().containsPoint(pos) && canGetReward)
	{
		node->initWithFile("lottery/cjjm_baoxiang2.png");
		getRuneStoneReward();
	}
	return true;
}

void GameResultLayer::getRuneStoneReward()
{
	auto rewards = DataManagerSelf->getSystemConfig().runeStoneReward;
	addFood(rewards[0]);
	addDiamond(rewards[1]);
	addKey(rewards[2]);
	for (int i = 0; i < 5; ++i)
	{
		auto node = m_layout->getChildById(21 + i);
		node->setVisible(false);
	}
	UserInfo::theInfo()->setRuneStone(0);
}

void GameResultLayer::saveRewards()
{
	int oldFood = UserInfo::theInfo()->getFood();
	UserInfo::theInfo()->setFood(m_reward.food + oldFood);
	int oldDiamond = UserInfo::theInfo()->getDiamond();
	UserInfo::theInfo()->setDiamond(m_reward.diamond + oldDiamond);
	int oldKey = UserInfo::theInfo()->getKey();
	UserInfo::theInfo()->setKey(m_reward.key + oldKey);
}

void GameResultLayer::addFood(int value)
{
	m_reward.food += value;

	CCSprite *spr = CCSprite::create("common/title_food.png");
	m_layout->addChild(spr);
	spr->setPosition(m_layout->getChildById(8)->getPosition());
	auto targetPos = m_layout->getChildById(27)->getPosition();
	auto moveAction = CCEaseBackInOut::create(CCMoveTo::create(0.3f, targetPos));
	auto func = CCFunctionAction::create([=]()
	{
		CCLabelAtlas *foodNum = dynamic_cast<CCLabelAtlas *>(m_layout->getChildById(30));
		foodNum->setString(CommonUtil::intToStr(m_reward.food));
		int oldFood = UserInfo::theInfo()->getFood();
		UserInfo::theInfo()->setFood(value + oldFood);
	});
	spr->runAction(CCSequence::create(moveAction, func, NULL));
}

void GameResultLayer::addDiamond(int value)
{
	m_reward.diamond += value;

	CCSprite *spr = CCSprite::create("common/title_diamond.png");
	m_layout->addChild(spr);
	spr->setPosition(m_layout->getChildById(8)->getPosition());
	auto targetPos = m_layout->getChildById(28)->getPosition();
	auto moveAction = CCEaseBackInOut::create(CCMoveTo::create(0.3f, targetPos));
	auto func = CCFunctionAction::create([=]()
	{
		CCLabelAtlas *diamondNum = dynamic_cast<CCLabelAtlas *>(m_layout->getChildById(31));
		diamondNum->setString(CommonUtil::intToStr(m_reward.diamond));
		int oldDiamond = UserInfo::theInfo()->getDiamond();
		UserInfo::theInfo()->setDiamond(value + oldDiamond);
	});
	spr->runAction(CCSequence::create(moveAction, func, NULL));
}

void GameResultLayer::addKey(int value)
{
	m_reward.key += value;

	CCSprite *spr = CCSprite::create("lottery/cjjm_yaoshi.png");
	m_layout->addChild(spr);
	spr->setPosition(m_layout->getChildById(8)->getPosition());
	auto targetPos = m_layout->getChildById(29)->getPosition();
	auto moveAction = CCEaseBackInOut::create(CCMoveTo::create(0.3f, targetPos));
	auto func = CCFunctionAction::create([=]()
	{
		CCLabelAtlas *keyNum = dynamic_cast<CCLabelAtlas *>(m_layout->getChildById(32));
		keyNum->setString(CommonUtil::intToStr(m_reward.key));
		int oldKey = UserInfo::theInfo()->getKey();
		UserInfo::theInfo()->setKey(value + oldKey);
	});
	spr->runAction(CCSequence::create(moveAction, func, NULL));
}
///////////////////////////////////////////////////////////////////////////////////
bool GameWinLayer::init()
{
	setPanelId(kStageFailPanel);
	GameResultLayer::init("layout/game_win.xml");
	initPanel();
	return true;
}

void GameWinLayer::initPanel()
{
	auto petIds = PetManager::petMgr()->getCurPetIds();
	for (int i = 0; i < 4; ++i)
	{
		CCArmature *animation = dynamic_cast<CCArmature *>(m_layout->getChildById(41 + i));
		animation->setVisible(i < petIds.size());
		if (i < petIds.size())
		{
			auto pet = PetManager::petMgr()->getPetById(petIds[i]);
			assert(pet);
			string path = pet->getPetData().petAnimationRes;
			CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo(path.c_str());

			int pos1 = path.rfind("/");
			int pos2 = path.rfind(".");
			string armatureName = path.substr(pos1 + 1, pos2 - pos1 - 1);

			animation->init(armatureName.c_str());
			animation->getAnimation()->play("move", -1, -1, 1);
		}
	}

	CCLabelAtlas *stageNum = dynamic_cast<CCLabelAtlas *>(m_layout->getChildById(26));
	int curStage = StageDataMgr::theMgr()->getCurStage();
	stageNum->setString(CommonUtil::intToStr(curStage - 1));
}

void GameWinLayer::addGameResultReward()
{
	int curStage = StageDataMgr::theMgr()->getCurStage() - 1;
	auto rewards = DataManagerSelf->getStageConfig(curStage).rewardWin;
	m_reward.food += rewards[0];
	m_reward.diamond += rewards[1];
	m_reward.key += rewards[2];
}

void GameWinLayer::onConfirm()
{
	MainScene::theScene()->showPanel(kLotteryPanel, kLotterySceneFromStageScene);
}
///////////////////////////////////////////////////////////////////////////

bool GameFailLayer::init()
{
	setPanelId(kStageFailPanel);
	GameResultLayer::init("layout/game_fail.xml");
	initPanel();

	return true;
}

void GameFailLayer::initPanel()
{
	CCMenuItem *buyPetBtn = dynamic_cast<CCMenuItem *>((m_layout->getChildById(19)));
	buyPetBtn->setTarget(this, menu_selector(GameFailLayer::onBuyPetBtnClicked));
	buyPetBtn->setVisible(false);

	int curStage = StageDataMgr::theMgr()->getCurStage();
	auto petIds = DataManagerSelf->getStageConfig(curStage).recommendPets;
	for (size_t i = 0; i < petIds.size(); ++i)
	{
		auto nameRes = DataManagerSelf->getPetResConfig(petIds[i]).petNameRes;
		CCSprite *keyNumLabel = dynamic_cast<CCSprite *>(m_layout->getChildById(13 + i));
		keyNumLabel->initWithFile(nameRes.c_str());
	}
}

void GameFailLayer::addGameResultReward()
{
	int curStage = StageDataMgr::theMgr()->getCurStage();
	auto rewards = DataManagerSelf->getStageConfig(curStage).rewardFail;
	m_reward.food += rewards[0];
	m_reward.diamond += rewards[1];
	m_reward.key += rewards[2];
}

void GameFailLayer::onConfirm()
{
	auto dialog = FailToUpgradePetDialog::create();
	MainScene::theScene()->showDialog(dialog);

}

void GameFailLayer::onBuyPetBtnClicked(CCObject *pSender)
{
	SoundMgr::theMgr()->playEffect(kEffectMusicButton);

}
