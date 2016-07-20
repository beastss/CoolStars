#include "RedPackageDialog.h"
#include "UiLayout.h"
#include "ThiefModel.h"
#include "DataManager.h"
#include "GoodsMgr.h"
#include "SoundMgr.h"
#include "CommonUtil.h"
USING_NS_CC;
using namespace std;
RedPackageDialog *RedPackageDialog::create()
{
	RedPackageDialog *view = new RedPackageDialog();
	view->init();
	view->autorelease();
	return view;
}

RedPackageDialog::RedPackageDialog()
{

}

bool RedPackageDialog::init()
{
	m_layout = UiLayout::create("layout/red_package_dialog.xml");
	m_layout->setMenuTouchPriority(m_touchPriority);
	addChild(m_layout);
	initLayout();
	setContentSize(m_layout->getContentSize());

	addMaskLayer();
	return true;
}

void RedPackageDialog::initLayout()
{
	CCMenuItem *btn = dynamic_cast<CCMenuItem *>((m_layout->getChildById(7)));
	btn->setTarget(this, menu_selector(RedPackageDialog::onConfirmBtnClicked));

	CCLabelAtlas *diamonds = dynamic_cast<CCLabelAtlas *>(m_layout->getChildById(8));
	auto data = DataManagerSelf->getRewardsConfig().redPackage;
	diamonds->setString(CommonUtil::intToStr(data.amount));
}

void RedPackageDialog::onConfirmBtnClicked(cocos2d::CCObject* pSender)
{
	SoundMgr::theMgr()->playEffect(kEffectMusicButton);
	auto rewards = DataManagerSelf->getRewardsConfig().redPackage;
	GoodsMgr::theMgr()->addGoods(rewards);
	removeFromParent();
}
