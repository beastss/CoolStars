#include "PauseDialog.h"
#include "UiLayout.h"
#include "MainScene.h"
#include "CommonMacros.h"
#include "SoundMgr.h"
#include "PackageModel.h"
#include "PackageDialog.h"

USING_NS_CC;
using namespace std;
PauseDialog::PauseDialog(void)
{

}

bool PauseDialog::init()
{
	m_layout = UiLayout::create("layout/package_pause.xml");
	
	setContentSize(m_layout->getContentSize());
	addMaskLayer();
	addChild(m_layout);
	initLayout();
    return true;
}

void PauseDialog::initLayout()
{
	CCMenuItem *resumeBtn = dynamic_cast<CCMenuItem *>((m_layout->getChildById(3)));
	resumeBtn->setTarget(this, menu_selector(PauseDialog::onResume));

	CCMenuItem *toMuteBtn = dynamic_cast<CCMenuItem *>((m_layout->getChildById(4)));
	toMuteBtn->setTarget(this, menu_selector(PauseDialog::onMute));
	
	CCMenuItem *toSoundBtn = dynamic_cast<CCMenuItem *>((m_layout->getChildById(8)));
	toSoundBtn->setTarget(this, menu_selector(PauseDialog::onSound));

	CCMenuItem *backBtn = dynamic_cast<CCMenuItem *>((m_layout->getChildById(5)));
	backBtn->setTarget(this, menu_selector(PauseDialog::onBackHome));

	CCMenuItem *purchaseBtn = dynamic_cast<CCMenuItem *>((m_layout->getChildById(6)));
	purchaseBtn->setTarget(this, menu_selector(PauseDialog::onPurchase));

	setMute(SoundMgr::theMgr()->isMute());
}

void PauseDialog::onResume(CCObject* pSender)
{
	SoundMgr::theMgr()->playEffect(kEffectMusicButton);
	removeFromParent();
}

void PauseDialog::setMute(bool isMute)
{
	SoundMgr::theMgr()->setMute(isMute);
	m_layout->getChildById(4)->setVisible(!isMute);
	m_layout->getChildById(8)->setVisible(isMute);
}

void PauseDialog::onMute(CCObject* pSender)
{
	SoundMgr::theMgr()->playEffect(kEffectMusicButton);
	setMute(true);
}

void PauseDialog::onSound(CCObject* pSender)
{
	SoundMgr::theMgr()->playEffect(kEffectMusicButton);
	setMute(false);
}

void PauseDialog::onBackHome(CCObject* pSender)
{
	SoundMgr::theMgr()->playEffect(kEffectMusicButton);
	removeFromParent();
	MainScene::theScene()->showPanel(kMainMenu);
}

void PauseDialog::onPurchase(CCObject* pSender)
{
	SoundMgr::theMgr()->playEffect(kEffectMusicButton);
	PackageModel::theModel()->buyPackage(kPackPause, [=]()
	{
		removeFromParent();
	});
}
