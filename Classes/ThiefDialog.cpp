#include "ThiefDialog.h"
#include "UiLayout.h"
#include "ThiefModel.h"
#include "SoundMgr.h"
USING_NS_CC;
using namespace std;
ThiefDialog *ThiefDialog::create()
{
	ThiefDialog *view = new ThiefDialog();
	view->init();
	view->autorelease();
	return view;
}

ThiefDialog::ThiefDialog()
{

}

bool ThiefDialog::init()
{
	m_layout = UiLayout::create("layout/thief_dialog.xml");
	m_layout->setMenuTouchPriority(m_touchPriority);
	addChild(m_layout);
	initLayout();
	setContentSize(m_layout->getContentSize());

	addMaskLayer();
	return true;
}

void ThiefDialog::initLayout()
{
	CCMenuItem *btn = dynamic_cast<CCMenuItem *>((m_layout->getChildById(7)));
	btn->setTarget(this, menu_selector(ThiefDialog::onConfirmBtnClicked));
}

void ThiefDialog::onConfirmBtnClicked(cocos2d::CCObject* pSender)
{
	SoundMgr::theMgr()->playEffect(kEffectMusicButton);
	ThiefModel::theModel()->triggerThief();
	removeFromParent();
}
