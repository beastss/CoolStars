#include "PreStagePetSlot.h"
#include "UiLayout.h"
#include "EmptyBox.h"
#include "ListView.h"
#include "PetManager.h"
#include "ListPetView.h"
#include "PreStageModel.h"
#include "GuideMgr.h"
#include "GuideMacros.h"
#include "SoundMgr.h"
USING_NS_CC;
using namespace std;

PetSkillIcon *PetSkillIcon::create(int petId)
{
	PetSkillIcon  *icon = new PetSkillIcon(petId);
	icon->init();
	icon->autorelease();
	return icon;
}

bool PetSkillIcon::init()
{
	auto pet = PetManager::petMgr()->getPetById(m_petId);
	auto iconPath = pet->getPetData().petSkillRes;

	CCSprite::initWithFile(iconPath.c_str());
	auto spr = CCSprite::create(iconPath.c_str());
	auto size = spr->getContentSize();
	spr->setPosition(ccpMult(size, 0.5f));
	//addChild(spr);

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
	setContentSize(size);
	return true;
}
//////////////////////////////////////////////////////////////////////////
PreStagePetSlotNode *PreStagePetSlotNode::create(int petId)
{
	auto node = new PreStagePetSlotNode(petId);
	node->init();
	node->autorelease();
	return node;
}

bool PreStagePetSlotNode::init()
{
	m_layout = UiLayout::create("layout/pre_stage_pet_slot_node.xml");
	addChild(m_layout);
	CCSprite *petImg = dynamic_cast<CCSprite *>(m_layout->getChildById(5));
	EmptyBox *iconBox = dynamic_cast<EmptyBox *>(m_layout->getChildById(2));
	setContentSize(m_layout->getContentSize());

	auto pet = PetManager::petMgr()->getPetById(m_petId);
	if (pet)
	{
		auto path = pet->getPetData().petImgRes;
		petImg->initWithFile(path.c_str());
		iconBox->setNode(PetSkillIcon::create(m_petId));
	}
	else
	{
		petImg->initWithFile("pet_img/zbjm_jianying.png");
		iconBox->setNode(CCSprite::create("pet_skill_icon/zbjm_quan.png"));
	}
	return true;
}

string PreStagePetSlotNode::getBgPath()
{
	auto pet = PetManager::petMgr()->getPetById(m_petId);
	string path;
	if (pet)
	{
		int color = pet->getPetData().color;
		path = DataManagerSelf->getPetColorConfig(color).preStageSlotBg;
	}
	return path;
}

/////////////////////////////////////////////////////////////////////
PreStagePetSlot *PreStagePetSlot::create(int petId)
{
	auto node = new PreStagePetSlot(petId);
	node->init();
	node->autorelease();
	return node;
}

void PreStagePetSlot::onEnter()
{
	CCNode::onEnter();
	PreStageModel::theModel()->addView(this);
	PreStageModel::theModel()->selectPet(m_curPetId, NOT_SELECT_PET_ID);//init时还没有吧view加到mgr里
}

void PreStagePetSlot::onExit()
{
	CCNode::onExit();
	PreStageModel::theModel()->removeView(this);
}

bool PreStagePetSlot::init()
{
	m_layout = UiLayout::create("layout/pre_stage_pet_slot.xml");
	setContentSize(m_layout->getContentSize());
	addChild(m_layout);

	initLayout();
	return true;
}

void PreStagePetSlot::initLayout()
{
	m_listView = ListPetView::create(getContentSize());
	m_listView->setItemSelectedCallback(bind(&PreStagePetSlot::onSelectItemCallback, this, placeholders::_1));
	
	EmptyBox *box = dynamic_cast<EmptyBox *>((m_layout->getChildById(2)));
	box->setNode(m_listView);
	PreStageModel::theModel()->selectPet(m_curPetId, NOT_SELECT_PET_ID);
	refreshList();
}

int PreStagePetSlot::getIndexByPetId(int petId)
{
	int count = m_listView->count();
	for (int i = 0; i < count; ++i)
	{
		auto node = dynamic_cast<PreStagePetSlotNode *>(m_listView->getNode(i));
		int tempId = node->getPetId();
		if (tempId == petId) return i;
	}
	return kInvalidIndex;
}

void PreStagePetSlot::refreshList()
{
	if (m_curPetId == 31)
	{
		int i = 100;
	}
	auto petMgr = PetManager::petMgr();
	auto ids = PreStageModel::theModel()->getPetsCanSelect(m_curPetId);
	m_listView->clear();

	for (size_t i = 0; i < ids.size(); ++i)
	{
		auto node = PreStagePetSlotNode::create(ids[i]);
		m_listView->addNode(node);
	}
	int index = getIndexByPetId(m_curPetId);
	if (index != kInvalidIndex)
	{
		m_listView->setCurItem(index);
	}
}

void PreStagePetSlot::onSelectItemCallback(int index)
{
	PreStagePetSlotNode *node = dynamic_cast<PreStagePetSlotNode *>(m_listView->getNode(index));
	int petId = node->getPetId();

	CCSprite *slotBg = dynamic_cast<CCSprite *>(m_layout->getChildById(1));
	slotBg->initWithFile(node->getBgPath().c_str());

	if (m_curPetId != petId)
	{
		SoundMgr::theMgr()->playEffect(kEffectMusicButton);
		int oldPetId = m_curPetId;
		m_curPetId = petId;
		PreStageModel::theModel()->selectPet(petId, oldPetId);
		GuideMgr::theMgr()->endGuide(kGuideEnd_preStage_select_pet, petId);
	}
}


void PreStagePetSlot::onPetSelect(int oldPetId)
{
	refreshList();
}

