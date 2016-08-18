#include "MenuScene.h"
#include "UiLayout.h"
#include "StarsController.h"
#include "SoundMgr.h"
#include "StageScene.h"
#include "PetScene.h"
#include "RankingScene.h"
#include "PackageDialog.h"
#include "ShopScene.h"
#include "HelpPanel.h"
#include "CommonMacros.h"
#include "PreStageScene.h"
#include "LotteryScene.h"
#include "MainScene.h"
#include "RankingPanelUtil.h"
#include "GuideMacros.h"
#include "GuideMgr.h"
#include "RankingModel.h"
#include "ThiefModel.h"
#include "GoodsView.h"
#include "PackageModel.h"
#include "KeyPadWatcher.h"
#include "CommonUtil.h"
#include "EmptyBox.h"

USING_NS_CC;
using namespace std;

void MenuScene::onEnter()
{
	BasePanel::onEnter();
	MsgNotifier::theModel()->addView(this);
	MainScene::theScene()->showSimpleBk(false);
}

void MenuScene::onExit()
{
	BasePanel::onExit();
	MsgNotifier::theModel()->removeView(this);
	MainScene::theScene()->showSimpleBk(true);
}

bool MenuScene::init()
{
	setPanelId(kMainMenu);
	auto winSize = CCDirector::sharedDirector()->getWinSize();
	setContentSize(winSize);

	m_mainLayout = UiLayout::create("layout/main_menu.xml");
	m_mainLayout->setAnchorPoint(ccp(0.5f, 0.5f));
	m_mainLayout->setPosition(ccpMult(winSize, 0.5f));
	m_mainLayout->setMenuTouchPriority(m_touchPriority);
	addChild(m_mainLayout);

	m_bottomLayout = UiLayout::create("layout/main_menu_bottom.xml");
	m_bottomLayout->setAnchorPoint(ccp(0, 0));
	m_bottomLayout->setPosition(ccp(0, 0));
	m_bottomLayout->setMenuTouchPriority(m_touchPriority);
	addChild(m_bottomLayout);

	initMainLayout();
	initBottomLayout();
	refreshPetTips();
	GuideMgr::theMgr()->startGuide(kGuideStart_mainMenu_in, bind(&MenuScene::justShowNormalGameBtn, this));

	runBkAction();
	return true;
}

void MenuScene::initMainLayout()
{
	CCMenuItem * normalBtn= dynamic_cast<CCMenuItem *>((m_mainLayout->getChildById(6)));
	normalBtn->setTarget(this, menu_selector(MenuScene::toNormalGame));
	normalBtn->runAction(CommonUtil::getRepeatScaleAction());

	//CCMenuItem *treasureBtn = dynamic_cast<CCMenuItem *>((m_mainLayout->getChildById(7)));
	//treasureBtn->setTarget(this, menu_selector(MenuScene::toTreasureGame));

	CCMenuItem *packageBtn = dynamic_cast<CCMenuItem *>((m_mainLayout->getChildById(5)));
	packageBtn->setTarget(this, menu_selector(MenuScene::toPackagePanel));

}

void MenuScene::initBottomLayout()
{
	CCMenuItem *settingBtn = dynamic_cast<CCMenuItem *>((m_bottomLayout->getChildById(1)));
	settingBtn->setTarget(this, menu_selector(MenuScene::toSetting));

	//声音打开的图标
	CCMenuItem *toMuteBtn = dynamic_cast<CCMenuItem *>((m_bottomLayout->getChildById(2)));
	toMuteBtn->setTarget(this, menu_selector(MenuScene::toMute));

	//声音关闭的图标
	CCMenuItem *toSoundBtn = dynamic_cast<CCMenuItem *>((m_bottomLayout->getChildById(7)));
	toSoundBtn->setTarget(this, menu_selector(MenuScene::toSound));

	CCMenuItem *helpBtn = dynamic_cast<CCMenuItem *>((m_bottomLayout->getChildById(3)));
	helpBtn->setTarget(this, menu_selector(MenuScene::toHelpPanel));
	
	CCMenuItem *rankBtn = dynamic_cast<CCMenuItem *>((m_bottomLayout->getChildById(4)));
	rankBtn->setTarget(this, menu_selector(MenuScene::toRankPanel));

	CCMenuItem *shopBtn = dynamic_cast<CCMenuItem *>((m_bottomLayout->getChildById(6)));
	shopBtn->setTarget(this, menu_selector(MenuScene::toShopPanel));

	CCMenuItem *drawLotteryBtn = dynamic_cast<CCMenuItem *>((m_bottomLayout->getChildById(8)));
	drawLotteryBtn->setTarget(this, menu_selector(MenuScene::drawLottery));

	CCMenuItem *toPetBtn = dynamic_cast<CCMenuItem *>((m_bottomLayout->getChildById(9)));
	toPetBtn->setTarget(this, menu_selector(MenuScene::toPetPanel));

	bool isMute = SoundMgr::theMgr()->isMute();
	if (isMute)
	{
		toMuteBtn->setVisible(false);
		m_settingBtnsHelper.addPopupNodes(toSoundBtn);
	}
	else
	{
		toSoundBtn->setVisible(false);
		m_settingBtnsHelper.addPopupNodes(toMuteBtn);
	}
	m_settingBtnsHelper.addPopupNodes(helpBtn);
	m_settingBtnsHelper.init(settingBtn->getPosition());
}

void MenuScene::toNormalGame(CCObject* pSender)
{
	SoundMgr::theMgr()->playEffect(kEffectMusicButton);
	MainScene::theScene()->showPanel(kPreStagePanel);
}

void MenuScene::toTreasureGame(CCObject* pSender)
{
	SoundMgr::theMgr()->playEffect(kEffectMusicButton);
	MainScene::theScene()->showPanel(kPreStagePanel, kTreasureType);
}

void MenuScene::drawLottery(cocos2d::CCObject* pSender)
{
	SoundMgr::theMgr()->playEffect(kEffectMusicButton);
	MainScene::theScene()->showPanel(kLotteryPanel);
}

void MenuScene::toPetPanel(cocos2d::CCObject* pSender)
{
	SoundMgr::theMgr()->playEffect(kEffectMusicButton);
	MainScene::theScene()->showPanel(kPetPanel, kPetSceneFromMenuScene);
}

void MenuScene::toSetting(cocos2d::CCObject* pSender)
{
	SoundMgr::theMgr()->playEffect(kEffectMusicButton);
	m_settingBtnsHelper.toggle();
}

void MenuScene::toMute(cocos2d::CCObject* pSender)
{
	SoundMgr::theMgr()->playEffect(kEffectMusicButton);
	SoundMgr::theMgr()->setMute(true);
	CCMenuItem *toMuteBtn = dynamic_cast<CCMenuItem *>((m_bottomLayout->getChildById(2)));
	CCMenuItem *toSoundBtn = dynamic_cast<CCMenuItem *>((m_bottomLayout->getChildById(7)));
	m_settingBtnsHelper.removePopupNodes(toMuteBtn);
	toMuteBtn->setVisible(false);
	m_settingBtnsHelper.addPopupNodes(toSoundBtn);
	toSoundBtn->setVisible(true);
}

void MenuScene::toSound(cocos2d::CCObject* pSender)
{
	SoundMgr::theMgr()->playEffect(kEffectMusicButton);
	SoundMgr::theMgr()->setMute(false);
	CCMenuItem *toMuteBtn = dynamic_cast<CCMenuItem *>((m_bottomLayout->getChildById(2)));
	CCMenuItem *toSoundBtn = dynamic_cast<CCMenuItem *>((m_bottomLayout->getChildById(7)));
	m_settingBtnsHelper.removePopupNodes(toSoundBtn);
	toSoundBtn->setVisible(false);
	m_settingBtnsHelper.addPopupNodes(toMuteBtn);
	toMuteBtn->setVisible(true);
}

void MenuScene::toHelpPanel(cocos2d::CCObject* pSender)
{
	SoundMgr::theMgr()->playEffect(kEffectMusicButton);
	auto panel = HelpPanel::create();
	MainScene::theScene()->showDialog(panel);
}

void MenuScene::toRankPanel(cocos2d::CCObject* pSender)
{
	SoundMgr::theMgr()->playEffect(kEffectMusicButton);
	if (RankingModel::theModel()->alreadyOpenRanking())
	{
		MainScene::theScene()->showPanel(kRankingPanel);
	}
	else
	{
		auto panel = RankingNameInputPanel::create();
		MainScene::theScene()->showDialog(panel);
	}
}

void MenuScene::toPackagePanel(cocos2d::CCObject* pSender)
{
	SoundMgr::theMgr()->playEffect(kEffectMusicButton);
	MainScene::theScene()->showPanel(kPackagePanel, 0, false);
}

void MenuScene::toShopPanel(cocos2d::CCObject* pSender)
{
	SoundMgr::theMgr()->playEffect(kEffectMusicButton);
	MainScene::theScene()->showPanel(kShopPanel);
}

void MenuScene::justShowNormalGameBtn()
{
	m_bottomLayout->setVisible(false);
	//m_mainLayout->getChildById(6)->setPosition(m_mainLayout->getChildById(7)->getPosition());
	//m_mainLayout->getChildById(7)->setVisible(false);
	m_mainLayout->getChildById(5)->setVisible(false);
}

void MenuScene::refreshPetTips()
{
	bool hasPetToUpgrade = PetManager::petMgr()->hasPetToUpgrade();
	m_bottomLayout->getChildById(10)->setVisible(hasPetToUpgrade);
	m_bottomLayout->getChildById(10)->setZOrder(2);

}

void MenuScene::onBackKeyTouched()
{
	KeyPadWatcher::exitGame();
}

//生成num个和为0的列表
vector<float> getRandomList(float max, int num)
{
	vector<float> v;
	int sum = 0;
	for (int i = 0; i < num - 1; ++i)
	{
		float temp = CCRANDOM_MINUS1_1() * max;
		v.push_back(temp);
		sum += temp;
	}
	v.push_back(-sum);
	return v;
}


void MenuScene::runBkAction()
{
	string paths[] = {
		"main_menu/bk/bj_pic_1.png",
		"main_menu/bk/bj_pic_2.png",
		"main_menu/bk/bj_pic_3.png",
		"main_menu/bk/bj_pic_4.png"
	};

	for(int i = 0; i < 4; ++i)
	{
		
		auto spr = CCSprite::create(paths[i].c_str());
		
		auto box = dynamic_cast<EmptyBox *>(m_mainLayout->getChildById(8 + i));
		box->setNode(spr);
		auto rotate = getRandomList(15, 3);
		float duration = CCRANDOM_0_1() * 2.0f + 1.0f;
		auto rotateAction = CCSequence::create(
			CCRotateTo::create(duration, rotate[0]), 
			CCRotateTo::create(duration, rotate[1]),
			CCRotateTo::create(duration, rotate[2]), NULL);

		auto xMove = getRandomList(15, 4);
		auto yMove = getRandomList(10, 4);
		duration = CCRANDOM_0_1() + 1.0f;
		auto moveAction = CCSequence::create(
			CCMoveBy::create(duration, ccp(xMove[0], yMove[0])),
			CCMoveBy::create(duration, ccp(xMove[1], yMove[1])),
			CCMoveBy::create(duration, ccp(xMove[2], yMove[2])),
			CCMoveBy::create(duration, ccp(xMove[3], yMove[3])), NULL);
		box->runAction(CCRepeatForever::create(CCSpawn::create(rotateAction, moveAction, NULL)));
	}

}