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
	CCMenuItem *cancelBtn = dynamic_cast<CCMenuItem *>((m_layout->getChildById(4)));
	cancelBtn->setTarget(this, menu_selector(PackageDialog::onCancelBtnClicked));

	CCMenuItem *buyBtn = dynamic_cast<CCMenuItem *>((m_layout->getChildById(5)));
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
	PackageModel::theModel()->buyPackage(m_type, m_confirmHandle);
}

