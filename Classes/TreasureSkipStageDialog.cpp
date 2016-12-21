#include "TreasureSkipStageDialog.h"
#include "UiLayout.h"
#include "SoundMgr.h"
#include "MainScene.h"
#include "CommonMacros.h"
#include "TreasureStageModel.h"
#include "TreasureStageResultDialog.h"
#include "TreasureStageState.h"
#include "TreasureStageDataMgr.h"
#include "UserInfo.h"
#include "PackageDialog.h"
#include "PackageModel.h"
#include "CommonUtil.h"

USING_NS_CC;
using namespace std;
TreasureSkipStageDialog::TreasureSkipStageDialog()
{

}

TreasureSkipStageDialog *TreasureSkipStageDialog::create()
{
	TreasureSkipStageDialog *dialog = new TreasureSkipStageDialog();
	dialog->init();
	dialog->autorelease();
	return dialog;
}

bool TreasureSkipStageDialog::init()
{
	string xmlPath = "layout/treasure_skip_stage_dialog.xml";
	m_layout = UiLayout::create(xmlPath.c_str());

	setContentSize(m_layout->getContentSize());
	addMaskLayer();
	addChild(m_layout);
	initLayout();
	return true;
}

void TreasureSkipStageDialog::initLayout()
{
	CCMenuItem *backBtn = dynamic_cast<CCMenuItem *>((m_layout->getChildById(5)));
	backBtn->setTarget(this, menu_selector(TreasureSkipStageDialog::onCancel));
	CCMenuItem *confirmBtn = dynamic_cast<CCMenuItem *>((m_layout->getChildById(4)));
	confirmBtn->setTarget(this, menu_selector(TreasureSkipStageDialog::onConfirm));

	CCLabelAtlas *foodNumLabel = dynamic_cast<CCLabelAtlas*>(m_layout->getChildById(16));
	foodNumLabel->setString(CommonUtil::intToStr(TreasureStageState::theState()->getStageFoodReward()));
	CCLabelAtlas *diamonddNumLabel = dynamic_cast<CCLabelAtlas*>(m_layout->getChildById(18));
	diamonddNumLabel->setString(CommonUtil::intToStr(TreasureStageState::theState()->getStageDiamondReward()));
}

void TreasureSkipStageDialog::onCancel(CCObject* pSender)
{
	SoundMgr::theMgr()->playEffect(kEffectMusicButton);
	auto dialog = TreasureStageResultDialog::create();
	MainScene::theScene()->showDialog(dialog);
	removeFromParent();
}

void TreasureSkipStageDialog::onConfirm(CCObject* pSender)
{
	SoundMgr::theMgr()->playEffect(kEffectMusicButton);
	bool ok = UserInfo::theInfo()->consumeDiamond(TreasureStageDataMgrSelf->costToSkipCurStage());
	if (ok)
	{
		TreasureStageModel::theModel()->nextStage(true);
		removeFromParent();
	}
	else
	{
		auto dialog = PackageDialog::create(kPackageDiamond);
		dialog->setCancelHandle([]()
		{
			MainScene::theScene()->showDialog(TreasureSkipStageDialog::create(), false);
		});
		dialog->setConfirmHandle([]()
		{
			MainScene::theScene()->showDialog(TreasureSkipStageDialog::create(), false);
		});
		MainScene::theScene()->showDialog(dialog);
	}
}
