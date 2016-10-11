#include "PackageDialog.h"
#include "UiLayout.h"
#include "MenuScene.h"
#include "TitlePanel.h"
#include "DataManager.h"
#include "UserInfo.h"
#include "CommonMacros.h"
#include "GoodsMgr.h"
#include "ThiefModel.h"
#include "MyPurchase.h"
#include "PackageModel.h"
#include "SoundMgr.h"
#include "ThiefDialog.h"
#include "MainScene.h"
#include "MsgNotifier.h"
#include "GameBackEndState.h"
#include "CommonUtil.h"

USING_NS_CC;
using namespace std;

PackageDialog *PackageDialog::create(int type)
{
	PackageDialog *view = new PackageDialog(type);
	view->init();
	view->autorelease();
	return view;
}

PackageDialog::PackageDialog(int type)
: m_type(type)
{

}

bool PackageDialog::init()
{
	m_layout = UiLayout::create("layout/package_common.xml");
	m_layout->setMenuTouchPriority(m_touchPriority);
	addChild(m_layout);
	initLayout();
	setContentSize(m_layout->getContentSize());

	addMaskLayer();
	return true;
}

void PackageDialog::initLayout()
{
	CCMenuItem *cancelBtn = dynamic_cast<CCMenuItem *>(m_layout->getChildById(10));
	cancelBtn->setTarget(this, menu_selector(PackageDialog::onCancelBtnClicked));

	CCMenuItem *buyBtn = dynamic_cast<CCMenuItem *>(m_layout->getChildById(9));
	buyBtn->setTarget(this, menu_selector(PackageDialog::onBuyBtnClicked));
	buyBtn->runAction(CommonUtil::getScaleAction());
	
	auto config = DataManagerSelf->getPackageConfig(m_type);

	CCSprite *text = dynamic_cast<CCSprite *>((m_layout->getChildById(2)));
	text->initWithFile(config.textPath.c_str());
	
	m_layout->getChildById(4)->setVisible(false);
	CCSprite *fingerSpr = dynamic_cast<CCSprite *>(m_layout->getChildById(3));
	auto oldPos = fingerSpr->getPosition();
	auto newPos = m_layout->getChildById(4)->getPosition();
	fingerSpr->setZOrder(2);
	fingerSpr->runAction(CCRepeatForever::create(
		CCSequence::create(
		CCMoveTo::create(0.3f, newPos), 
		CCMoveTo::create(0.3f, oldPos), NULL)));
}

void PackageDialog::onCancelBtnClicked(cocos2d::CCObject* pSender)
{
	SoundMgr::theMgr()->playEffect(kEffectMusicButton);
	if (m_cancelHandle)
	{
		m_cancelHandle();
	}
	removeFromParent();
}

void PackageDialog::onBuyBtnClicked(cocos2d::CCObject* pSender)
{
	SoundMgr::theMgr()->playEffect(kEffectMusicButton);
	bool success = PackageModel::theModel()->buyPackage(m_type, [=]()
	{
		if (m_confirmHandle)
		{
			m_confirmHandle();
		}
		removeFromParent();
	});
	//只用于钻石扣费
	if (!success)
	{
		MainScene::theScene()->showDialog(PackageDialog::create(kPackageDiamond));
		CCLOG("not enough diamond");
		MyPurchase::sharedPurchase()->showToast(kToastTextNotEnoughDiamond);
	}
}
////////////////////////////////////////////////////////////////////
void PackageScene::onEnter()
{
	BasePanel::onEnter();
	MsgNotifier::theModel()->addView(this);
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, kDialogTouchPriority - 1, true);
}

void PackageScene::onExit()
{
	BasePanel::onExit();
	MsgNotifier::theModel()->removeView(this);
	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
}

bool PackageScene::init()
{
	setPanelId(kPackagePanel);

	auto winSize = CCDirector::sharedDirector()->getWinSize();
	setContentSize(winSize);

	auto dialog = PackageDialog::create(kPackagePetFirstGet);
	addChild(dialog);
	dialog->setAnchorPoint(ccp(0.5f, 0.5f));
	dialog->setPosition(ccpMult(winSize, 0.5f));
	dialog->setCancelHandle([=]()
	{
		MainScene::theScene()->clearPanelRecord();
		MainScene::theScene()->showPanel(kMainMenu);;
	});
	dialog->setConfirmHandle([=]()
	{
		MsgNotifier::theModel()->onPetPackageBuy();
	});
	m_thief = CCSprite::create("thief/youxijiemian_sentou1.png");
	m_thief->setScale(0.1f);
	addChild(m_thief);
	auto size = dialog->getContentSize();
	m_thief->setPosition(ccp(size.width * 0.5, size.height *0.2f));

	onThiefShowUp();
	return true;
}

bool PackageScene::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
	auto pos = m_thief->getParent()->convertToNodeSpace(pTouch->getLocation());
	bool isInPanel = ThiefModel::theModel()->isThisPanel(kThiefPackagePanel);

	if (isInPanel && m_thief->boundingBox().containsPoint(pos))
	{
		SoundMgr::theMgr()->playEffect(kEffectMusicButton);
		m_thief->initWithFile("thief/youxijiemian_sentou2.png");

		auto dailog = ThiefDialog::create();
		MainScene::theScene()->showDialog(dailog);

		return true;
	}
	return false;
}

void PackageScene::onThiefShowUp()
{
	bool isInPanel = ThiefModel::theModel()->isThisPanel(kThiefPackagePanel);
	m_thief->setVisible(isInPanel);
}

void PackageScene::onThiefDisappear()
{
	m_thief->setVisible(false);
}

void PackageScene::onBackKeyTouched()
{
	MainScene::theScene()->clearPanelRecord();
	MainScene::theScene()->showPanel(kMainMenu);
}