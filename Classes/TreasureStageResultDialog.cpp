#include "TreasureStageResultDialog.h"
#include "UiLayout.h"
#include "SoundMgr.h"
#include "MainScene.h"
#include "CommonMacros.h"
#include "TreasureStageModel.h"
#include "TreasureStageState.h"
#include "PetManager.h"
#include "CommonUtil.h"
#include "UserInfo.h"

USING_NS_CC;
using namespace std;
TreasureStageResultDialog::TreasureStageResultDialog()
{

}

TreasureStageResultDialog *TreasureStageResultDialog::create()
{
	TreasureStageResultDialog *dialog = new TreasureStageResultDialog();
	dialog->init();
	dialog->autorelease();
	return dialog;
}

bool TreasureStageResultDialog::init()
{
	string xmlPath = "layout/treasure_result_dialog.xml";
	m_layout = UiLayout::create(xmlPath.c_str());
	
	setContentSize(m_layout->getContentSize());
	addMaskLayer();
	addChild(m_layout);
	initLayout();
    return true;
}

void TreasureStageResultDialog::initLayout()
{
	CCMenuItem *quitBtn = dynamic_cast<CCMenuItem *>((m_layout->getChildById(5)));
	quitBtn->setTarget(this, menu_selector(TreasureStageResultDialog::onToQuitBtnClicked));

	CCMenuItem *replayBtn = dynamic_cast<CCMenuItem *>((m_layout->getChildById(32)));
	replayBtn->setTarget(this, menu_selector(TreasureStageResultDialog::onReplayBtnClicked));
	
	auto ids = PetManager::petMgr()->getOwnedStagePetIds(false);
	float extraPercent = 0;
	for (size_t i = 0; i < ids.size(); ++i)
	{
		extraPercent += PetManager::petMgr()->getPetById(ids[i])->getPetData().skillPower / 100.0f;
	}
	//food
	int baseFoodReward = TreasureStageState::theState()->getCurFoodReward();
	int extraFoodReward = baseFoodReward * extraPercent;
	int resultFoodReward = baseFoodReward + extraFoodReward;
	CCLabelAtlas *baseFoodNum = dynamic_cast<CCLabelAtlas*>(m_layout->getChildById(12));
	baseFoodNum->setString(CommonUtil::intToStr(baseFoodReward));
	CCLabelAtlas *extarFoodNum = dynamic_cast<CCLabelAtlas*>(m_layout->getChildById(13));
	extarFoodNum->setString(CommonUtil::intToStr(extraFoodReward));
	CCLabelAtlas *resultFoodNum = dynamic_cast<CCLabelAtlas*>(m_layout->getChildById(14));
	resultFoodNum->setString(CommonUtil::intToStr(resultFoodReward));
	int curFood = UserInfo::theInfo()->getFood();
	UserInfo::theInfo()->setFood(curFood + resultFoodReward);

	//diamond
	int baseDiamondReward = TreasureStageState::theState()->getCurDiamondReward();
	int extraDiamondReward = baseDiamondReward * extraPercent;
	int resultDiamondReward = baseDiamondReward + extraDiamondReward;
	CCLabelAtlas *baseDiamondNum = dynamic_cast<CCLabelAtlas*>(m_layout->getChildById(29));
	baseDiamondNum->setString(CommonUtil::intToStr(baseDiamondReward));
	CCLabelAtlas *extarDiamondNum = dynamic_cast<CCLabelAtlas*>(m_layout->getChildById(30));
	extarDiamondNum->setString(CommonUtil::intToStr(extraDiamondReward));
	CCLabelAtlas *resultDiamondNum = dynamic_cast<CCLabelAtlas*>(m_layout->getChildById(31));
	resultDiamondNum->setString(CommonUtil::intToStr(resultDiamondReward));
	int curDiamond = UserInfo::theInfo()->getDiamond();
	UserInfo::theInfo()->setDiamond(curDiamond + resultDiamondReward);

	m_layout->getChildById(18)->runAction(CommonUtil::getRepeatScaleAction());
	m_layout->getChildById(19)->runAction(CCRepeatForever::create(CCRotateBy::create(1.0f, 120)));
}

void TreasureStageResultDialog::onToQuitBtnClicked(CCObject* pSender)
{
	SoundMgr::theMgr()->playEffect(kEffectMusicButton);
	MainScene::theScene()->showPanel(kMainMenu);
	removeFromParent();
}

void TreasureStageResultDialog::onReplayBtnClicked(CCObject* pSender)
{
	SoundMgr::theMgr()->playEffect(kEffectMusicButton);
	TreasureStageModel::theModel()->startNewStage();
	removeFromParent();
}

