#include "LotteryScene.h"
#include "UiLayout.h"
#include "TitlePanel.h"
#include "ListView.h"
#include "EmptyBox.h"
#include "CommonMacros.h"
#include "MainScene.h"
#include "LotteryModel.h"
#include "CommonUtil.h"
#include "UserInfo.h"
#include "DataManager.h"
#include "PetManager.h"
#include "CCFunctionAction.h"
#include "StarsController.h"
#include "ThiefModel.h"
#include "GuideMgr.h"
#include "SoundMgr.h"
#include "PackageDialog.h"
#include "PackageModel.h"
#include "MyPurchase.h"

USING_NS_CC;
using namespace std;

LotteryNode::LotteryNode(int touchPriority)
: TouchNode(touchPriority)
, m_isOpened(false)
{
	m_handle = function<void()>();
}

LotteryNode *LotteryNode::create(int touchPriority)
{
	auto node = new LotteryNode(touchPriority);
	node->init();
	node->autorelease();
	return node;
}

bool LotteryNode::init()
{
	m_layout = UiLayout::create("layout/lottery_node.xml");
	addChild(m_layout);
	setContentSize(m_layout->getContentSize());

	m_layout->getChildById(2)->setVisible(false);
	return true;
}

bool LotteryNode::onTouchBegan(cocos2d::CCPoint pt, bool isInside)
{
	if (isInside && !m_isOpened)
	{
		int key = UserInfo::theInfo()->getKey();
		int cost = DataManagerSelf->getSystemConfig().diamondsForOneKey;
		if (key <= 0 && !UserInfo::theInfo()->hasEnoughDiamond(cost))
		{
			MainScene::theScene()->showDialog(PackageDialog::create(kPackageDiamond));
			MyPurchase::sharedPurchase()->showToast(kToastTextNotEnoughDiamond);
		}
		else
		{
			m_isOpened = true;
			handleTouch();
		}
	}
	return isInside;
}

void LotteryNode::handleTouch()
{
	SoundMgr::theMgr()->playEffect(kEffectMusicButton);
	m_layout->getChildById(1)->setVisible(false);
	m_layout->getChildById(2)->setVisible(true);

	auto data = LotteryModel::theModel()->getLotteryResult();
	string resPath;
	int goodsNum = 0;
	if (data.type == kLotteryPet)
	{
		auto pet = PetManager::petMgr()->getPetById(data.param);
		assert(pet && "this pet is not opening");
		resPath = pet->getPetData().petImgRes;
		goodsNum = 1;
	}
	else
	{
		auto config = DataManagerSelf->getLotteryOutputConfig(data.type);
		resPath = config.resPath;
		goodsNum = data.param;
	}
	m_goodsLayout = UiLayout::create("layout/goods_view.xml");
	auto size = getContentSize();
	m_goodsLayout->setPosition(ccpMult(size, 0.5f));
	m_goodsLayout->setAnchorPoint(ccp(0.5f, 0.5f));
	addChild(m_goodsLayout);

	CCSprite *goods = dynamic_cast<CCSprite *>(m_goodsLayout->getChildById(1));
	goods->initWithFile(resPath.c_str());
	m_goodsLayout->getChildById(2)->setVisible(false);
	
	m_goodsLayout->runAction(getRewardOutAction(goodsNum));

	LotteryModel::theModel()->doLottery(data);
}

CCAction *LotteryNode::getRewardOutAction(int num)
{
	CCMoveBy *moveBy = CCMoveBy::create(0.5f, ccp(0, 30));
	auto func = CCFunctionAction::create([=]()
	{
		CCLabelAtlas *numLabel = dynamic_cast<CCLabelAtlas *>(m_goodsLayout->getChildById(2));
		numLabel->setString(CommonUtil::intToStr(num));
		numLabel->setVisible(true);

		m_handle();
	});
	return CCSequence::create(CCEaseBackInOut::create(moveBy), func, NULL);
}
////////////////////////////////////////////////////////////////
LotteryScene *LotteryScene::create(int usage)
{
	LotteryScene *scene = new LotteryScene(usage);
	scene->init();
	scene->autorelease();
	return scene;
}

LotteryScene::LotteryScene(int usage)
: m_openedBoxNum(0)
{
	setUsage(usage);
}

void LotteryScene::onEnter()
{
	BasePanel::onEnter();
	UserInfo::theInfo()->addView(this);
}

void LotteryScene::onExit()
{
	BasePanel::onExit();
	UserInfo::theInfo()->removeView(this);
}

bool LotteryScene::init()
{
	setPanelId(kLotteryPanel);

	auto winSize = CCDirector::sharedDirector()->getWinSize();
	setContentSize(winSize);

	m_layout = UiLayout::create("layout/lottery_panel.xml");
	m_layout->setMenuTouchPriority(m_touchPriority);
	m_layout->setAnchorPoint(ccp(0.5f, 0.5f));
	auto sourcePos = ccp(winSize.width * 0.5f, winSize.height * 1.5f);
	auto targetPos = ccpMult(winSize, 0.5f);
	m_layout->setPosition(sourcePos);
	m_layout->runAction(CCEaseBackInOut::create(CCMoveTo::create(0.5f, targetPos)));
	addChild(m_layout);
	initLayout();

	m_titlePanel = TitlePanel::create(m_touchPriority);
	m_titlePanel->setBottomThief(kThiefLotteryPanel);
	addChild(m_titlePanel);

	m_bottomLayout = UiLayout::create("layout/pre_stage_bottom.xml");
	addChild(m_bottomLayout);
	initBottomLayout();

	refreshUi();

	GuideMgr::theMgr()->startGuide(kGuideStart_lottery_in, bind(&LotteryScene::hideBottomBtns, this));

	return true;
}

void LotteryScene::initLayout()
{
	CCMenuItem * toPetSceneBtn = dynamic_cast<CCMenuItem *>((m_layout->getChildById(16)));
	toPetSceneBtn->setTarget(this, menu_selector(LotteryScene::toPetScene));
	
	int boxIds[] = { 6, 7, 8, 9, 10, 11, 12, 13, 14 };
	for (int i = 0; i < 9; ++i)
	{
		EmptyBox *box = dynamic_cast<EmptyBox *>((m_layout->getChildById(boxIds[i])));
		auto node = LotteryNode::create(m_touchPriority);
		node->setHandle(bind(&LotteryScene::onOpenReward, this));
		box->setNode(node);
	}
	
	int cost = DataManagerSelf->getSystemConfig().diamondsForOneKey;
	CCLabelAtlas *costDiamond = dynamic_cast<CCLabelAtlas *>(m_layout->getChildById(19));
	costDiamond->setString(CommonUtil::intToStr(cost));

	onKeyChanged();
}

void LotteryScene::initBottomLayout()
{
	CCMenuItem *startGameBtn = dynamic_cast<CCMenuItem *>((m_bottomLayout->getChildById(1)));
	startGameBtn->setTarget(this, menu_selector(LotteryScene::onStartBtnClicked));
}

void LotteryScene::refreshUi()
{
	auto startGameBtn = m_bottomLayout->getChildById(1);
	if (m_usage == kLotterySceneFromStageScene)
	{
		int key = UserInfo::theInfo()->getKey();
		bool isShowUp = m_openedBoxNum >= kMinOpenBoxNum || key <= 0;
		m_layout->getChildById(16)->setVisible(isShowUp);
		startGameBtn->setVisible(isShowUp);
		m_titlePanel->setUiVisible(kTitlePanelBackHome, isShowUp);
	}
	else
	{
		m_bottomLayout->getChildById(1)->setVisible(false);
	}

	int key = UserInfo::theInfo()->getKey();
	bool hasEnoughKey = key > 0;
	m_layout->getChildById(3)->setVisible(hasEnoughKey);
	m_layout->getChildById(4)->setVisible(hasEnoughKey);
	m_layout->getChildById(17)->setVisible(hasEnoughKey);
	m_layout->getChildById(18)->setVisible(!hasEnoughKey);
	m_layout->getChildById(19)->setVisible(!hasEnoughKey);
}

void LotteryScene::toPetScene(CCObject* pSender)
{
	SoundMgr::theMgr()->playEffect(kEffectMusicButton);
	GuideMgr::theMgr()->endGuide(kGuideEnd_lottery_click_pet_btn);
	MainScene::theScene()->showPanel(kPetPanel);
}

void LotteryScene::onStartBtnClicked(CCObject* pSender)
{
	SoundMgr::theMgr()->playEffect(kEffectMusicButton);
	StarsController::theModel()->resetStage(kNormalType);
	MainScene::theScene()->showPanel(kPreStagePanel);
}

void LotteryScene::onOpenReward()
{
	m_openedBoxNum++;

	if (m_openedBoxNum % kRewardBoxNum == 0)
	{
		const float kMoveTime = 0.5f;
		auto moveUpFunc = CCFunctionAction::create([=]()
		{
			auto winSize = CCDirector::sharedDirector()->getWinSize();
			auto targetPos = ccp(winSize.width * 0.5f, winSize.height * 1.5f);
			m_layout->runAction(CCEaseBackInOut::create(CCMoveTo::create(kMoveTime, targetPos)));
		});

		auto removeFunc = CCFunctionAction::create([=]()
		{
			removeAllChildrenWithCleanup(true);
			init();
		});
		runAction(CCSequence::create(CCDelayTime::create(0.5f), moveUpFunc, CCDelayTime::create(kMoveTime), removeFunc,NULL));
	}
	refreshUi();
}

void LotteryScene::onKeyChanged()
{
	CCLabelAtlas * keyNum = dynamic_cast<CCLabelAtlas *>((m_layout->getChildById(17)));
	int amount = UserInfo::theInfo()->getKey();
	char str[100] = { 0 };
	sprintf(str, ":%d", amount);
	keyNum->setString(str);
}

void LotteryScene::hideBottomBtns()
{
	m_layout->getChildById(16)->setVisible(false);
	m_titlePanel->setUiVisible(kTitlePanelBackHome, false);
	m_bottomLayout->getChildById(1)->setVisible(false);
}