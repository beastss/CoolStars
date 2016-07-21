#include "PetScene.h"
#include "UiLayout.h"
#include "MenuScene.h"
#include "TitlePanel.h"
#include "CommonMacros.h"
#include "MainScene.h"
#include "EmptyBox.h"
#include "PetView.h"
#include "PetManager.h"
#include "CommonUtil.h"
#include "ThiefModel.h"
#include "GuideMgr.h"
#include "SoundMgr.h"
#include "PackageModel.h"
#include "PackageDialog.h"
#include "PreStagePetSlot.h"
#include "MyPurchase.h"

USING_NS_CC;
using namespace std;

const float PetScene::kBtnSelectedScale = 1.3f;
int PetScene::s_curPetColor = kColorRandom;

void PetScene::onEnter()
{
	CCNode::onEnter();
	PetManager::petMgr()->addView(this);
}

void PetScene::onExit()
{
	CCNode::onExit();
	PetManager::petMgr()->removeView(this);
}

bool PetScene::init()
{
	setPanelId(kPetPanel);
	initColorPets();
	auto winSize = CCDirector::sharedDirector()->getWinSize();
	setContentSize(winSize);

	auto titlePanel = TitlePanel::create(m_touchPriority);
	titlePanel->setUiVisible(kTitlePanelBackHome, false);
	titlePanel->setTopThief(kThiefPetPanel);
	addChild(titlePanel);

	m_mainLayout = UiLayout::create("layout/pet_ui.xml");
	m_mainLayout->setMenuTouchPriority(m_touchPriority);
	m_mainLayout->setAnchorPoint(ccp(0.5f, 0.5f));
	m_mainLayout->setPosition(ccpMult(winSize, 0.5f));
	addChild(m_mainLayout);

	m_emptyPetLayout = UiLayout::create("layout/pet_ui_empty.xml");
	m_emptyPetLayout->setAnchorPoint(ccp(0.5f, 0.5f));
	m_emptyPetLayout->setPosition(ccpMult(winSize, 0.5f));
	addChild(m_emptyPetLayout);

	m_bottomLayout = UiLayout::create("layout/pet_ui_bottom.xml");
	m_bottomLayout->setMenuTouchPriority(m_touchPriority);
	m_bottomLayout->setAnchorPoint(ccp(0, 0));
	m_bottomLayout->setPosition(ccp(0, 0));
	addChild(m_bottomLayout);
	
	initMainLayout();
	initBottomLayout();
	if (s_curPetColor == kColorRandom)
	{
		auto ids = PetManager::petMgr()->getOwnedPetIds();
		if (!ids.empty())
		{
			s_curPetColor = PetManager::petMgr()->getPetById(ids[0])->getPetData().color;
		}
		else
		{
			s_curPetColor = kColorRed;
		}
	}
	handleColorBtnClicked(s_curPetColor);

	GuideMgr::theMgr()->startGuide(kGuideStart_pet_in);
	return true;
}

void PetScene::initMainLayout()
{
	CCMenuItem *leftPetBtn = dynamic_cast<CCMenuItem *>((m_mainLayout->getChildById(6)));
	leftPetBtn->setTarget(this, menu_selector(PetScene::onLeftPetBtnClicked));

	CCMenuItem *rightPetBtn = dynamic_cast<CCMenuItem *>((m_mainLayout->getChildById(5)));
	rightPetBtn->setTarget(this, menu_selector(PetScene::onRigthPetBtnClicked));

	CCMenuItem *upgradeBtn = dynamic_cast<CCMenuItem *>((m_mainLayout->getChildById(8)));
	upgradeBtn->setTarget(this, menu_selector(PetScene::onUpgradeBtnClicked));

	CCMenuItem *petPackageBtn = dynamic_cast<CCMenuItem *>((m_mainLayout->getChildById(21)));
	petPackageBtn->setTarget(this, menu_selector(PetScene::onPetPackageBtnClicked));
	
	CCPoint leftmost = m_mainLayout->getChildById(19)->getPosition();
	CCPoint center = m_mainLayout->getChildById(10)->getPosition();
	CCPoint rightmost = m_mainLayout->getChildById(20)->getPosition();
	m_moveHelper.init(leftmost, center, rightmost);

}

void PetScene::initBottomLayout()
{
	CCMenuItem *backBtn = dynamic_cast<CCMenuItem *>((m_bottomLayout->getChildById(1)));
	backBtn->setTarget(this, menu_selector(PetScene::onBackBtnClicked));

	CCMenuItem *greenPetBtn = dynamic_cast<CCMenuItem *>((m_bottomLayout->getChildById(2)));
	greenPetBtn->setTarget(this, menu_selector(PetScene::onGreenPetBtnClicked));

	CCMenuItem *purplePetBtn = dynamic_cast<CCMenuItem *>((m_bottomLayout->getChildById(6)));
	purplePetBtn->setTarget(this, menu_selector(PetScene::onPurplePetBtnClicked));

	CCMenuItem *redPetBtn = dynamic_cast<CCMenuItem *>((m_bottomLayout->getChildById(5)));
	redPetBtn->setTarget(this, menu_selector(PetScene::onRedPetBtnClicked));

	CCMenuItem *bluePetBtn = dynamic_cast<CCMenuItem *>((m_bottomLayout->getChildById(3)));
	bluePetBtn->setTarget(this, menu_selector(PetScene::onBluePetBtnClicked));

	CCMenuItem *yellowPetBtn = dynamic_cast<CCMenuItem *>((m_bottomLayout->getChildById(4)));
	yellowPetBtn->setTarget(this, menu_selector(PetScene::onYellowPetBtnClicked));

}

void PetScene::handleColorBtnClicked(int color)
{
	SoundMgr::theMgr()->playEffect(kEffectMusicButton);
	assert(color >= kColorRed && color <= kColorPurple);
	int btnIds[5] = {5, 4, 3, 2, 6};

	for (int i = 0; i < 5; ++i)
	{
		m_bottomLayout->getChildById(btnIds[i])->setScale(1);
	}
	CCMenuItem *btn = dynamic_cast<CCMenuItem *>((m_bottomLayout->getChildById(btnIds[color - 1])));
	btn->setScale(kBtnSelectedScale);
	changePetsColor(color);
}

void PetScene::onLeftPetBtnClicked(cocos2d::CCObject* pSender)
{
	SoundMgr::theMgr()->playEffect(kEffectMusicButton);
	if (m_curColorPetIndex > 0)
	{
		m_curColorPetIndex--;

		int petId = m_colorPets[s_curPetColor][m_curColorPetIndex];
		auto view = PetView::create(petId);
		addChild(view, -1);
		m_moveHelper.moveLeft(view);

		refreshUi();
	}
}

void PetScene::onRigthPetBtnClicked(cocos2d::CCObject* pSender)
{
	SoundMgr::theMgr()->playEffect(kEffectMusicButton);
	if (m_curColorPetIndex < m_colorPets[s_curPetColor].size() - 1)
	{
		m_curColorPetIndex++;

		int petId = m_colorPets[s_curPetColor][m_curColorPetIndex];
		auto view = PetView::create(petId);
		addChild(view, -1);
		m_moveHelper.moveRight(view);

		refreshUi();
	};
}

void PetScene::onUpgradeBtnClicked(cocos2d::CCObject* pSender)
{
	GuideMgr::theMgr()->endGuide(kGuideEnd_pet_upgrade);
	SoundMgr::theMgr()->playEffect(kEffectMusicButton);
	int petId = m_colorPets[s_curPetColor][m_curColorPetIndex];
	auto pet = PetManager::petMgr()->getPetById(petId);
	if (pet->canUpgrade())
	{
		pet->upgrade();
		refreshUi();
	}
	else if (!pet->isMaxLevel())
	{
		auto dialog = PackageDialog::create(kPackageDiamond);
		MainScene::theScene()->showDialog(dialog);
		MyPurchase::sharedPurchase()->showToast(kToastTextNotEnoughDiamond);
	}
}

void PetScene::onPetPackageBtnClicked(cocos2d::CCObject* pSender)
{
	SoundMgr::theMgr()->playEffect(kEffectMusicButton);
	auto dialog = PackageDialog::create(kPackagePetFirstGet);
	MainScene::theScene()->showDialog(dialog);
}

void PetScene::onGreenPetBtnClicked(cocos2d::CCObject* pSender)
{
	handleColorBtnClicked(kColorGreen);
}

void PetScene::onPurplePetBtnClicked(cocos2d::CCObject* pSender)
{
	handleColorBtnClicked(kColorPurple);
}

void PetScene::onRedPetBtnClicked(cocos2d::CCObject* pSender)
{
	handleColorBtnClicked(kColorRed);
}

void PetScene::onBluePetBtnClicked(cocos2d::CCObject* pSender)
{
	handleColorBtnClicked(kColorBlue);
}

void PetScene::onYellowPetBtnClicked(cocos2d::CCObject* pSender)
{
	handleColorBtnClicked(kColorYellow);
}

void PetScene::onBackBtnClicked(cocos2d::CCObject* pSender)
{
	SoundMgr::theMgr()->playEffect(kEffectMusicButton);
	MainScene::theScene()->backPanel();
}

void PetScene::initColorPets()
{
	auto petMgr = PetManager::petMgr();
	auto petIds = petMgr->getOwnedPetIds();
	for (size_t i = 0; i < petIds.size(); ++i)
	{
		auto data = petMgr->getPetById(petIds[i])->getPetData();
		m_colorPets[data.color].push_back(petIds[i]);
	}
}

void PetScene::changePetsColor(int color)
{
	s_curPetColor = color;
	m_curColorPetIndex = 0;

	m_moveHelper.clearNodes();

	int size = m_colorPets[s_curPetColor].size();
	if (m_curColorPetIndex >= 0 && m_curColorPetIndex < size)
	{
		int petId = m_colorPets[s_curPetColor][m_curColorPetIndex];
		auto view = PetView::create(petId);
		addChild(view, -1);
		m_moveHelper.setCenterNode(view);
	}

	refreshUi();
	GuideMgr::theMgr()->endGuide(kGuideEnd_pet_color_select);
	GuideMgr::theMgr()->startGuide(kGuideStart_pet_color_select);
}

void PetScene::refreshUi()
{
	int size = m_colorPets[s_curPetColor].size();
	bool hasPets = size != 0;
	m_mainLayout->setVisible(hasPets);
	m_emptyPetLayout->setVisible(!hasPets);

	if (m_curColorPetIndex >= 0 && m_curColorPetIndex < size)
	{
		int petId = m_colorPets[s_curPetColor][m_curColorPetIndex];

		auto data = PetManager::petMgr()->getPetById(petId)->getPetData();
		auto config = DataManagerSelf->getPetColorConfig(data.color);

		//等级图标
		CCSprite *lvImg = dynamic_cast<CCSprite *>(m_mainLayout->getChildById(13));
		lvImg->initWithFile(config.skillLvLabel.c_str());
		//宠物名字
		CCSprite *nameImg = dynamic_cast<CCSprite *>(m_mainLayout->getChildById(14));
		nameImg->initWithFile(data.petNameRes.c_str());
		//等级
		CCLabelAtlas *lvNum = dynamic_cast<CCLabelAtlas *>(m_mainLayout->getChildById(16));
		CCSprite *numRes = CCSprite::create(config.numRes.c_str());
		auto numResSize = numRes->getContentSize();
		lvNum->initWithString(CommonUtil::intToStr(data.level), config.numRes.c_str(), numResSize.width / 10, numResSize.height, '0');

		CCSprite *skillDesc = dynamic_cast<CCSprite *>(m_mainLayout->getChildById(22));
		auto anchorPt = skillDesc->getAnchorPoint();
		skillDesc->initWithFile(data.skillDescRes.c_str());
		skillDesc->setAnchorPoint(anchorPt);

		CCLabelAtlas *skillPower = dynamic_cast<CCLabelAtlas *>(m_mainLayout->getChildById(23));
		skillPower->setString(CommonUtil::intToStr(data.skillPower));

		auto skillIcon = PetSkillIcon::create(petId);
		EmptyBox *skillIconBox = dynamic_cast<EmptyBox *>(m_mainLayout->getChildById(24));
		skillIconBox->setNode(skillIcon);
	}

	refreshArrows();
	refreshUpgrdeCost();
	refreshPetPackage();
}

void PetScene::refreshUpgrdeCost()
{
	if (m_colorPets[s_curPetColor].empty()) return;

	int foodNum = UserInfo::theInfo()->getFood();
	int diamondNum = UserInfo::theInfo()->getDiamond();
	int petId = m_colorPets[s_curPetColor][m_curColorPetIndex];
	auto pet = PetManager::petMgr()->getPetById(petId);
	if (pet->isMaxLevel())//宠物已经满级
	{
		m_mainLayout->getChildById(9)->setVisible(false);
		m_mainLayout->getChildById(18)->setVisible(false);
		m_mainLayout->getChildById(17)->setVisible(false);
		m_mainLayout->getChildById(8)->setVisible(false);
	}
	else
	{
		int foodCost = pet->getPetData().foodToUpgrade;
		int diamondCost = foodCost / (DataManagerSelf->getSystemConfig().foodsByOneDiamond);
		bool isFoodEnough = foodNum >= foodCost;
		int cost = isFoodEnough ? foodCost : diamondCost;

		m_mainLayout->getChildById(9)->setVisible(!isFoodEnough);
		m_mainLayout->getChildById(18)->setVisible(isFoodEnough);
		m_mainLayout->getChildById(17)->setVisible(true);
		m_mainLayout->getChildById(8)->setVisible(true);

		CCLabelAtlas *costNum = dynamic_cast<CCLabelAtlas *>(m_mainLayout->getChildById(17));
		costNum->setString(CommonUtil::intToStr(cost));
	}
}

void PetScene::refreshArrows()
{
	int size = m_colorPets[s_curPetColor].size();
	m_mainLayout->getChildById(6)->setVisible(m_curColorPetIndex > 0);
	m_mainLayout->getChildById(5)->setVisible(m_curColorPetIndex < size - 1);
}

void PetScene::refreshPetPackage()
{
	bool canBuy = PackageModel::theModel()->canBuyPetPackage();
	m_mainLayout->getChildById(21)->setVisible(canBuy);
}

void PetScene::onNewPetAdd()
{
	initColorPets();
	refreshUi();
}
