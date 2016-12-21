#include "TreasurePauseDialog.h"
#include "UiLayout.h"
#include "SoundMgr.h"
#include "MainScene.h"
#include "CommonMacros.h"
#include "TreasureStageModel.h"
#include "TreasureStageResultDialog.h"

USING_NS_CC;
using namespace std;
TreasurePauseDialog::TreasurePauseDialog()
{

}

TreasurePauseDialog *TreasurePauseDialog::create()
{
	TreasurePauseDialog *dialog = new TreasurePauseDialog();
	dialog->init();
	dialog->autorelease();
	return dialog;
}

bool TreasurePauseDialog::init()
{
	string xmlPath = "layout/treasure_pause_dialog.xml";
	m_layout = UiLayout::create(xmlPath.c_str());

	setContentSize(m_layout->getContentSize());
	addMaskLayer();
	addChild(m_layout);
	initLayout();
	return true;
}

void TreasurePauseDialog::initLayout()
{
	CCMenuItem *leaveGameBtn = dynamic_cast<CCMenuItem *>((m_layout->getChildById(4)));
	leaveGameBtn->setTarget(this, menu_selector(TreasurePauseDialog::onLeaveGame));

	CCMenuItem *cancelBtn = dynamic_cast<CCMenuItem *>((m_layout->getChildById(5)));
	cancelBtn->setTarget(this, menu_selector(TreasurePauseDialog::onCancel));
}

void TreasurePauseDialog::onLeaveGame(CCObject* pSender)
{
	SoundMgr::theMgr()->playEffect(kEffectMusicButton);

	auto dialog = TreasureStageResultDialog::create();
	MainScene::theScene()->showDialog(dialog);

	removeFromParent();
}

void TreasurePauseDialog::onCancel(CCObject* pSender)
{
	SoundMgr::theMgr()->playEffect(kEffectMusicButton);
	removeFromParent();
}