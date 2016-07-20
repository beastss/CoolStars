#include "HelpPanel.h"
#include "UiLayout.h"
#include "SoundMgr.h"
USING_NS_CC;
using namespace std;

bool HelpPanel::init()
{
	m_layout = UiLayout::create("layout/about.xml");
	auto size = m_layout->getContentSize();
	setContentSize(size);
	addMaskLayer();
	addChild(m_layout);

	initLayout();
	return true;
}

void HelpPanel::initLayout()
{
	CCMenuItem *confirmBtn = dynamic_cast<CCMenuItem *>((m_layout->getChildById(3)));
	confirmBtn->setTarget(this, menu_selector(HelpPanel::onConfirmBtnClicked));
}

void HelpPanel::onConfirmBtnClicked(cocos2d::CCObject* pSender)
{
	SoundMgr::theMgr()->playEffect(kEffectMusicButton);
	removeFromParent();
}
