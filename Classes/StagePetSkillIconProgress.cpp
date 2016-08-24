#include "StagePetSkillIconProgress.h"
#include "PetManager.h"
#include "CCFunctionAction.h"
#include "UiLayout.h"
#include <algorithm>
#include "EmptyBox.h"
USING_NS_CC;
using namespace std;
StagePetSkillIconProgress *StagePetSkillIconProgress::create(int petId)
{
	StagePetSkillIconProgress *progress = new StagePetSkillIconProgress(petId);
	progress->init();
	progress->autorelease();
	return progress;
}

void StagePetSkillIconProgress::onEnter()
{
	CCNode::onEnter();
	PetManager::petMgr()->addView(this);
}

void StagePetSkillIconProgress::onExit()
{
	CCNode::onExit();
	PetManager::petMgr()->removeView(this);
}

bool StagePetSkillIconProgress::init()
{
	m_layout = UiLayout::create("layout/stage_pet_skill_icon.xml");
	addChild(m_layout);
	auto pet = PetManager::petMgr()->getPetById(m_petId);
	auto iconPath = pet->getPetData().petSkillRes;
	
	CCSprite *iconBk = dynamic_cast<CCSprite *>(m_layout->getChildById(1));
	iconBk->initWithFile(iconPath.c_str());
	CCSize size = iconBk->getContentSize();
	setScale(0.8f);
	setContentSize(ccpMult(size, 0.8f));

	EmptyBox *box = dynamic_cast<EmptyBox *>(m_layout->getChildById(4));
	m_progress = CCProgressTimer::create(CCSprite::create(iconPath.c_str()));
	m_progress->setType(kCCProgressTimerTypeRadial);
	m_progress->setPosition(ccpMult(size, 0.5f));
	box->setNode(m_progress);

	int maxEnergy = pet->getPetData().maxEnergy;
	setPercentage((float)pet->getPetData().energy / maxEnergy, false);

	//不能直接用PetSkillIcon,因为CCProgressTimer只是支持CCSprite
	int commandId = pet->getPetData().commonid;
	char str[100] = { 0 };
	auto numSize = CCSprite::create("num/ty_ziti6.png")->getContentSize();
	if (commandId == kPetDragon)
	{
		sprintf(str, ";%d", pet->getPetData().skillPower);
		auto pLabel = CCLabelAtlas::create(str, "num/ty_ziti6.png", numSize.width / 12, numSize.height, '0');
		pLabel->setAnchorPoint(ccp(0.5f, 0.5f));
		pLabel->setPosition(ccpMult(size, 0.5));
		addChild(pLabel);
	}
	else if (commandId == kPetTiger)
	{
		sprintf(str, ":%d", pet->getPetData().skillPower);
		auto pLabel = CCLabelAtlas::create(str, "num/ty_ziti6.png", numSize.width / 12, numSize.height, '0');
		pLabel->setAnchorPoint(ccp(0.5f, 0.5f));
		pLabel->setPosition(ccpMult(size, 0.5));
		addChild(pLabel);
	}

	return true;
}

void StagePetSkillIconProgress::setPercentage(float value, bool withAction)
{
	value = max(0.0f, min(value * 100, 100.0f));
	auto pet = PetManager::petMgr()->getPetById(m_petId);
	if (withAction)
	{
		static const float kDurainon = 0.5f;
		CCProgressTo *progressto = CCProgressTo::create(kDurainon, value);
		CCDelayTime *delay = CCDelayTime::create(0.2f);
		CCFunctionAction *func = CCFunctionAction::create([=]()
		{
			m_layout->getChildById(3)->setVisible(pet->isEnergyFull());
		});
		m_progress->runAction(CCSequence::create(progressto, delay, func, NULL));
	}
	else
	{
		m_progress->setPercentage(value);
		m_layout->getChildById(3)->setVisible(pet->isEnergyFull());
	}
}

void StagePetSkillIconProgress::runEnergyAddAction(int oldEnergy)
{
	auto pet = PetManager::petMgr()->getPetById(m_petId);
	int maxEnergy = pet->getPetData().maxEnergy;

	setPercentage((float)oldEnergy / maxEnergy, false);
	setPercentage((float)pet->getPetData().energy / maxEnergy, true);
}

void StagePetSkillIconProgress::onPetEnergyChanged(int petId, int oldEnergy, int newEnergy)
{
	if (m_petId == petId)
	{
		bool isAddEnergy = oldEnergy <= newEnergy;

		auto pet = PetManager::petMgr()->getPetById(m_petId);
		int maxEnergy = pet->getPetData().maxEnergy;
		setPercentage((float)newEnergy / maxEnergy, isAddEnergy);
	}
}