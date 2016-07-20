#include "TitlePanel.h"
#include "MenuScene.h"
#include "UiLayout.h"
#include "CCFunctionAction.h"
#include "MainScene.h"
#include "CommonMacros.h"
#include "PackageDialog.h"
#include "UserInfo.h"
#include "CommonUtil.h"
#include "ThiefModel.h"
#include "GoodsMgr.h"
#include "DataManager.h"
#include "GoodsView.h"
#include "ThiefDialog.h"
#include "SoundMgr.h"
#include "PackageModel.h"
USING_NS_CC;
using namespace std;
using namespace CommonUtil;

TitlePanel *TitlePanel::create(int touchPriority)
{
	auto panel = new TitlePanel(touchPriority);
	panel->init();
	panel->autorelease();
	return panel;
}

void TitlePanel::onEnter()
{
	CCNode::onEnter();
	UserInfo::theInfo()->addView(this);
	MsgNotifier::theModel()->addView(this);
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, m_touchPriority, true);
}

void TitlePanel::onExit()
{
	CCNode::onExit();
	UserInfo::theInfo()->removeView(this);
	MsgNotifier::theModel()->removeView(this);
	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
}

bool TitlePanel::init()
{
	auto winSize = CCDirector::sharedDirector()->getWinSize();
	setContentSize(winSize);

	m_topLayout = UiLayout::create("layout/common_title.xml");
	m_topLayout->setMenuTouchPriority(m_touchPriority);
	m_topLayout->setAnchorPoint(ccp(0, 1));
	m_topLayout->setPosition(ccp(0, winSize.height));
	addChild(m_topLayout);

	m_bottomLayout = UiLayout::create("layout/common_bottom.xml");
	m_bottomLayout->setMenuTouchPriority(m_touchPriority);
	m_bottomLayout->setAnchorPoint(ccp(0, 0));
	m_bottomLayout->setPosition(ccp(0, 0));
	addChild(m_bottomLayout);

	initTopLayout();
	initBottomLayout();
	return true;
}

void TitlePanel::initTopLayout()
{
	CCMenuItem *addStrengthBtn = dynamic_cast<CCMenuItem *>(m_topLayout->getChildById(9));
	addStrengthBtn->setTarget(this, menu_selector(TitlePanel::onAddStrengthBtnClicked));

	CCMenuItem *addDiamondBtn = dynamic_cast<CCMenuItem *>(m_topLayout->getChildById(6));
	addDiamondBtn->setTarget(this, menu_selector(TitlePanel::onAddDiamondBtnClicked));

	auto theInfo = UserInfo::theInfo();
	CCLabelAtlas *strengthNum = dynamic_cast<CCLabelAtlas *>(m_topLayout->getChildById(16));
	strengthNum->setString(intToStr(theInfo->getStrength()));

	CCLabelAtlas *foodNum = dynamic_cast<CCLabelAtlas *>(m_topLayout->getChildById(17));
	foodNum->setString(intToStr(theInfo->getFood()));

	CCLabelAtlas *diamondNum = dynamic_cast<CCLabelAtlas *>(m_topLayout->getChildById(18));
	diamondNum->setString(intToStr(theInfo->getDiamond()));
	
	m_topLayout->getChildById(19)->setVisible(false);

	CCLabelTTF *leftTime = dynamic_cast<CCLabelTTF *>(m_topLayout->getChildById(20));
	leftTime->setVisible(false);
	leftTime->setColor(ccc3(0, 0, 0));
}

void TitlePanel::initBottomLayout()
{
	CCMenuItem *backHomeBtn = dynamic_cast<CCMenuItem *>(m_bottomLayout->getChildById(2));
	backHomeBtn->setTarget(this, menu_selector(TitlePanel::onBackHomeBtnClicked));

	m_bottomLayout->getChildById(3)->setVisible(false);
}

void TitlePanel::onAddStrengthBtnClicked(cocos2d::CCObject* pSender)
{
	SoundMgr::theMgr()->playEffect(kEffectMusicButton);
	auto dialog = PackageDialog::create(kPackageStrength);
	MainScene::theScene()->showDialog(dialog);
}

void TitlePanel::onAddDiamondBtnClicked(cocos2d::CCObject* pSender)
{
	SoundMgr::theMgr()->playEffect(kEffectMusicButton);
	auto dialog = PackageDialog::create(kPackageDiamond);
	MainScene::theScene()->showDialog(dialog);
}

void TitlePanel::onBackHomeBtnClicked(cocos2d::CCObject* pSender)
{
	SoundMgr::theMgr()->playEffect(kEffectMusicButton);
	MainScene::theScene()->clearPanelRecord();
	MainScene::theScene()->showPanel(kMainMenu);
}

void TitlePanel::setUiVisible(int who, bool isVisible)
{
	switch (who)
	{
	case kTitlePanelStrength:
		m_topLayout->getChildById(7)->setVisible(isVisible);
		m_topLayout->getChildById(8)->setVisible(isVisible);
		m_topLayout->getChildById(9)->setVisible(isVisible);
		m_topLayout->getChildById(10)->setVisible(isVisible);
		break;
	case kTitlePanelFood:
		m_topLayout->getChildById(1)->setVisible(isVisible);
		m_topLayout->getChildById(2)->setVisible(isVisible);
		m_topLayout->getChildById(12)->setVisible(isVisible);
		break;
	case kTitlePanelDiamond:
		m_topLayout->getChildById(4)->setVisible(isVisible);
		m_topLayout->getChildById(5)->setVisible(isVisible);
		m_topLayout->getChildById(6)->setVisible(isVisible);
		m_topLayout->getChildById(11)->setVisible(isVisible);
		break;
	case kTitlePanelBackHome:
		m_bottomLayout->getChildById(2)->setVisible(isVisible);
		break;
	default:
		break;
	}
}

void TitlePanel::onDiamondChanged()
{
	CCLabelAtlas *diamondNum = dynamic_cast<CCLabelAtlas *>(m_topLayout->getChildById(18));
	diamondNum->setString(intToStr(UserInfo::theInfo()->getDiamond()));
	diamondNum->runAction(getScaleAction());
}

void TitlePanel::onFoodChanged()
{
	CCLabelAtlas *foodNum = dynamic_cast<CCLabelAtlas *>(m_topLayout->getChildById(17));
	foodNum->setString(intToStr(UserInfo::theInfo()->getFood()));
	foodNum->runAction(getScaleAction());

}

void TitlePanel::onStrengthChanged()
{
	CCLabelAtlas *strengthNum = dynamic_cast<CCLabelAtlas *>(m_topLayout->getChildById(16));
	strengthNum->setString(intToStr(UserInfo::theInfo()->getStrength()));
	strengthNum->runAction(getScaleAction());
}

void TitlePanel::onStrengthChanged(int leftSecs)
{
	int mins = leftSecs / 60;
	int secs = leftSecs % 60;
	CCLabelTTF *leftTime = dynamic_cast<CCLabelTTF *>(m_topLayout->getChildById(20));
	leftTime->setVisible(true);
	char str[100] = { 0 };
	sprintf(str, "%02d : %02d", mins, secs);
	leftTime->setString(str);
}

CCAction *TitlePanel::getScaleAction()
{
	CCScaleTo *scaleLarge = CCScaleTo::create(0.15f, 1.5f);
	CCScaleTo *scaleSmall = CCScaleTo::create(0.25f, 0.8f);
	CCScaleTo *scaleNormal = CCScaleTo::create(0.1f, 1.0f);
	CCSequence *seq = CCSequence::create(scaleLarge, scaleSmall, scaleNormal, NULL);
	return seq;
}

bool TitlePanel::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
	vector<CCNode *>nodes;
	CCNode *node = NULL;
	node = m_topLayout->getChildById(19);
	if (node->isVisible())
	{
		nodes.push_back(node);
	}

	node = m_bottomLayout->getChildById(3);
	if (node->isVisible())
	{
		nodes.push_back(node);
	}

	for (size_t i = 0; i < nodes.size(); ++i)
	{
		auto pos = nodes[i]->getParent()->convertToNodeSpace(pTouch->getLocation());
		auto box = nodes[i]->boundingBox();
		if (node->boundingBox().containsPoint(pos))
		{
			SoundMgr::theMgr()->playEffect(kEffectMusicButton);
			CCSprite *spr = dynamic_cast<CCSprite *>(node);
			spr->initWithFile("thief/youxijiemian_sentou2.png");
			
			auto dailog = ThiefDialog::create();
			MainScene::theScene()->showDialog(dailog);

			return true;
		}
	}
	return false;
}

void TitlePanel::setTopThief(int type)
{
	m_thiefNode = m_topLayout->getChildById(19);
	m_thiefType = type;

	onThiefShowUp();
}

void TitlePanel::setBottomThief(int type)
{
	m_thiefNode = m_bottomLayout->getChildById(3);
	m_thiefType = type;

	onThiefShowUp();
}

void TitlePanel::onThiefShowUp()
{
	bool isInPanel = ThiefModel::theModel()->isThisPanel(m_thiefType);
	if (m_thiefNode && isInPanel)
	{
		m_thiefNode->setVisible(true);
		m_thiefNode->runAction(CCFadeIn::create(1.0f));
	}
}

void TitlePanel::onThiefDisappear()
{
	if (m_thiefNode)
	{
		m_thiefNode->setVisible(false);
	}

}