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
	bool isBusinessMode = GameBackEndState::theModel()->isBusinessMode();
	m_layout->getChildById(4)->setVisible(!isBusinessMode);
	m_layout->getChildById(5)->setVisible(!isBusinessMode);
	m_layout->getChildById(7)->setVisible(isBusinessMode);
	m_layout->getChildById(8)->setVisible(isBusinessMode);

	CCMenuItem *cancelBtn = dynamic_cast<CCMenuItem *>((m_layout->getChildById(isBusinessMode ? 7 : 4)));
	cancelBtn->setTarget(this, menu_selector(PackageDialog::onCancelBtnClicked));

	CCMenuItem *buyBtn = dynamic_cast<CCMenuItem *>((m_layout->getChildById(isBusinessMode ? 8 : 5)));
	buyBtn->setTarget(this, menu_selector(PackageDialog::onBuyBtnClicked));
	
	auto config = DataManagerSelf->getPackageConfig(m_type);

	CCSprite *text = dynamic_cast<CCSprite *>((m_layout->getChildById(2)));
	text->initWithFile(config.textPath.c_str());
	
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

	auto dialog = PackageDialog::create(kPackageProps);
	addChild(dialog);
	dialog->setAnchorPoint(ccp(0.5f, 0.5f));
	dialog->setPosition(ccpMult(winSize, 0.5f));
	dialog->setCancelHandle([=]()
	{
		MainScene::theScene()->clearPanelRecord();
		MainScene::theScene()->showPanel(kMainMenu);;
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