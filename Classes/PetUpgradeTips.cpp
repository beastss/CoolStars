#include "PetUpgradeTips.h"
#include "UiLayout.h"
#include "PetManager.h"
#include "CommonUtil.h"
USING_NS_CC;
using namespace std;

PetUpgradeTipsNode::PetUpgradeTipsNode(int petId, int lv)
: m_petId(petId)
, m_lv(lv)
{

}

PetUpgradeTipsNode *PetUpgradeTipsNode::create(int petId, int lv)
{
	auto panel = new PetUpgradeTipsNode(petId, lv);
	panel->init();
	panel->autorelease();
	return panel;
}

bool PetUpgradeTipsNode::init()
{
	m_layout = UiLayout::create("layout/pet_upgrade_tips_node.xml");
	addChild(m_layout);
	auto size = m_layout->getContentSize();
	setContentSize(size);

	auto data = PetManager::petMgr()->getPetById(m_petId)->getPetData();
	auto config = DataManagerSelf->getPetColorConfig(data.color);
	//宠物图片
	CCSprite *petImg = dynamic_cast<CCSprite *>(m_layout->getChildById(9));
	petImg->initWithFile(data.petImgRes.c_str());
	petImg = dynamic_cast<CCSprite *>(m_layout->getChildById(12));
	petImg->initWithFile(data.petImgRes.c_str());
	//等级图标
	CCSprite *lvImg = dynamic_cast<CCSprite *>(m_layout->getChildById(10));
	lvImg->initWithFile(config.skillLvLabel.c_str());
	lvImg = dynamic_cast<CCSprite *>(m_layout->getChildById(13));
	lvImg->initWithFile(config.skillLvLabel.c_str());
	//等级
	CCLabelAtlas *lvNum = dynamic_cast<CCLabelAtlas *>(m_layout->getChildById(11));
	CCSprite *numRes = CCSprite::create(config.numRes.c_str());
	auto numResSize = numRes->getContentSize();
	lvNum->initWithString(CommonUtil::intToStr(m_lv), config.numRes.c_str(), numResSize.width / 10, numResSize.height, '0');

	lvNum = dynamic_cast<CCLabelAtlas *>(m_layout->getChildById(14));
	lvNum->initWithString(CommonUtil::intToStr(m_lv + 1), config.numRes.c_str(), numResSize.width / 10, numResSize.height, '0');
	//所需饲料
	CCLabelAtlas *foodNum = dynamic_cast<CCLabelAtlas *>(m_layout->getChildById(7));
	auto commonData = DataManagerSelf->getPetCommonConfig(data.commonid);
	int foodNeed = commonData.foodToUpgrade[m_lv - 1];
	foodNum->setString(CommonUtil::intToStr(foodNeed));
	return true;
}

/////////////////////////////////////////////////////////////////////
PetUpgradeTips::PetUpgradeTips(int petId)
: m_petId(petId)
{

}

PetUpgradeTips *PetUpgradeTips::create(int petId)
{
	auto panel = new PetUpgradeTips(petId);
	panel->init();
	panel->autorelease();
	return panel;
}

bool PetUpgradeTips::init()
{
	m_layout = UiLayout::create("layout/pet_upgrade_tips.xml");
	addChild(m_layout);
	auto size = m_layout->getContentSize();
	setContentSize(size);

	auto pet = PetManager::petMgr()->getPetById(m_petId);
	auto commonData = DataManagerSelf->getPetCommonConfig(pet->getPetData().commonid);
	auto upgradeVec = commonData.foodToUpgrade;

	int num = upgradeVec.size() - 1;
	auto startPos = m_layout->getChildById(4)->getPosition();
	auto endPos = m_layout->getChildById(5)->getPosition();
	auto posVec = CommonUtil::getSameDistancePos(startPos, endPos, num);
	for (size_t i = 0; i < num; ++i)
	{
		auto node = PetUpgradeTipsNode::create(m_petId, i + 1);
		m_layout->addChild(node);
		node->setPosition(posVec[i]);
	}
	return true;
}