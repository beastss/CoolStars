#include "PetEntity.h"
#include "PetSkill.h"
#include "PetSavingHelper.h"
#include "StageOperator.h"
#include "CommonMacros.h"
#include "UserInfo.h"
#include "StageDataMgr.h"
#include "StageLayersMgr.h"
#include "PetManager.h"
#include <algorithm>
#include "GuideMgr.h"
#include "StarsEraseModule.h"
#include "GameDataAnalysis.h"
PetEntity::PetEntity(int petId)
{
	//上次保存的宠物数据
	m_data = PetSavingHelper::getPetState(petId);
	m_data.isOwned = m_data.level != 0;
	refreshPetData();
}

PetEntity::~PetEntity()
{
}

void PetEntity::refreshPetData()
{
	m_data.energy = 0;

	auto petRes = DataManagerSelf->getPetResConfig(m_data.petId);
	m_data.petImgRes = petRes.petImgRes;
	m_data.petSkillRes = petRes.skillRes;
	m_data.petAnimationRes = petRes.petAnimationRes;
	m_data.petNameRes = petRes.petNameRes;

	auto commonData = DataManagerSelf->getPetCommonConfig(m_data.commonid);
	m_data.level = max(m_data.level, 1);//默认最低为1级
	int level = m_data.level;
	
	m_data.maxEnergy = commonData.maxEnergy[level - 1];
	m_data.skillPower = commonData.skillPower[level - 1];
	m_data.foodToUpgrade = commonData.foodToUpgrade[level - 1];
	m_data.skillTarget = commonData.skillTarget;
	m_data.maxLevel = commonData.maxLevel;
	m_data.skillDescRes = commonData.skillDescRes;
}

PetEntity *PetEntity::PetFactory(int petId)
{
	auto data = PetSavingHelper::getPetState(petId);
	int petType = data.commonid;
	int petColor = data.color;
	switch (data.commonid)
	{
	case kPetRat:
		return new PetRat(petId);
	case kPetOx:
		return new PetOx(petId);
	case kPetTiger:
		return new PetTiger(petId);
	case kPetRabbit:
		return new PetRabbit(petId);
	case kPetDragon:
		return new PetDragon(petId);
	case kPetSnake:
		return new PetSnake(petId);
	case kPetHorse:
		return new PetHorse(petId);
	case kPetGoat:
		return new PetGoat(petId);
	case kPetMonkey:
		return new PetMonkey(petId);
	case kPetRooster:
		return new PetRooster(petId);
	case kPetDog:
		return new PetDog(petId);
	case kPetPig:
		return new PetPig(petId);
	default:
		assert(false && "no this kind of pet");
		return NULL;
	}
}

bool PetEntity::isMaxLevel()
{
	return m_data.level >= m_data.maxLevel;
}

void PetEntity::getThisNewPet()
{
	m_data.level = 1;
	m_data.isOwned = true;
	refreshPetData();
	PetSavingHelper::setPetState(m_data);
}

bool PetEntity::canUpgrade()
{
	if (!m_data.isOwned || isMaxLevel()) return false;

	int foodNum = UserInfo::theInfo()->getFood();
	int diamondNum = UserInfo::theInfo()->getDiamond();

	int foodCost = m_data.foodToUpgrade;
	int diamondCost = foodCost / (DataManagerSelf->getSystemConfig().foodsByOneDiamond);

	return foodNum >= foodCost || diamondNum >= diamondCost;
}

void PetEntity::upgrade()
{
	if (!canUpgrade()) return;

	int foodNum = UserInfo::theInfo()->getFood();

	int foodCost = m_data.foodToUpgrade;
	int diamondCost = foodCost / (DataManagerSelf->getSystemConfig().foodsByOneDiamond);
	if (foodNum >= foodCost)
	{
		UserInfo::theInfo()->setFood(foodNum - foodCost);
	}
	else
	{
		UserInfo::theInfo()->consumeDiamond(diamondCost);
		GameDataAnalysis::theModel()->consumeDiamond(kDiamondConsumePetUpgrade, m_data.petId, diamondCost);
	}

	m_data.level++;
	refreshPetData();
	PetSavingHelper::setPetState(m_data);
}

void PetEntity::setEnergy(int energy)
{
	int value = max(0, min(energy, m_data.maxEnergy));
	int oldEnergy = m_data.energy;
	m_data.energy = value;
	
	PetManager::petMgr()->petEnergyChanged(m_data.petId, oldEnergy, value);
	if (canUseSkill())
	{
		GuideMgr::theMgr()->startGuide(kGuideStart_stage_petFullPower);
	}
}

void PetEntity::onUseSkill()
{
	setEnergy(0);
	//StarsController::theModel()->moveOneStep(false); 
}

void PetEntity::useNoTargetSkill()
{
	noTargetSkill();
	onUseSkill();
}

void PetEntity::useToStarSkill(const LogicGrid &grid)
{
	toStarSkill(grid);
	onUseSkill();
}

void PetEntity::useToPetSkill(int petId)
{
	toPetSkill(petId);
	onUseSkill();
}
//////////////////////////////////////////////////////////////////////////////
void PetRat::skillInit()
{
	vector<LogicGrid> grids;
	auto nodes = StarsController::theModel()->getStarNodes();
	for (size_t i = 0; i < nodes.size(); ++i)
	{
		auto attr = nodes[i]->getAttr();
		if (attr.type == kColorStar && attr.color == m_data.color)
		{
			grids.push_back(attr.grid);
		}
	}
	StageLayersMgr::theMgr()->highLightStars(grids, m_data.skillPower / 2, 0);
}

void PetRat::toStarSkill(const LogicGrid &grid)
{
	StarsEraseModule::theModel()->scaleErase(grid, m_data.skillPower / 2, 0);
}
//////////////////////////////////////////////////////////////////////////////
void PetOx::skillInit()
{
	vector<LogicGrid> grids;
	auto nodes = StarsController::theModel()->getStarNodes();
	for (size_t i = 0; i < nodes.size(); ++i)
	{
		auto attr = nodes[i]->getAttr();
		if (attr.type == kColorStar && attr.color == m_data.color)
		{
			grids.push_back(attr.grid);
		}
	}
	StageLayersMgr::theMgr()->highLightStars(grids, 0, m_data.skillPower / 2);
}

void PetOx::toStarSkill(const LogicGrid &grid)
{
	StarsEraseModule::theModel()->scaleErase(grid, 0, m_data.skillPower / 2);
}
//////////////////////////////////////////////////////////////////////////////

void PetTiger::noTargetSkill()
{
	StageOp->addSteps(m_data.skillPower);//如果使用技能也算一步 需要+1
}
//////////////////////////////////////////////////////////////////////////////

void PetRabbit::noTargetSkill()
{
	StageOp->randomReplaceStars(m_data.petId, kColorStar, m_data.color, m_data.skillPower);
}
//////////////////////////////////////////////////////////////////////////////

void PetDragon::noTargetSkill()
{
	StageLayersMgr::theMgr()->doubleScore();
	auto info = StageDataMgr::theMgr();
	info->setNextScoreBonus(m_data.skillPower);
}
//////////////////////////////////////////////////////////////////////////////
void PetSnake::skillInit()
{
	vector<LogicGrid> grids;
	auto nodes = StarsController::theModel()->getStarNodes();
	for (size_t i = 0; i < nodes.size(); ++i)
	{
		auto attr = nodes[i]->getAttr();
		if (attr.type == kColorStar && attr.color == m_data.color)
		{
			grids.push_back(attr.grid);
		}
	}
	StageLayersMgr::theMgr()->highLightStars(grids, m_data.skillPower, m_data.skillPower);
}

void PetSnake::toStarSkill(const LogicGrid &grid)
{
	StarsEraseModule::theModel()->scaleErase(grid, m_data.skillPower, m_data.skillPower);
}
//////////////////////////////////////////////////////////////////////////////

void PetHorse::noTargetSkill()
{
	StageOp->randomReplaceStars(m_data.petId, kDiamond, kColorRandom, m_data.skillPower);
}
//////////////////////////////////////////////////////////////////////////////

void PetGoat::noTargetSkill()
{
	StarsEraseModule::theModel()->randomErase(m_data.skillPower);
}
//////////////////////////////////////////////////////////////////////////////
void PetMonkey::skillInit()
{
	vector<int> highLightPetsId;
	std::vector<int> petIds = PetManager::petMgr()->getCurPetIds();
	for (size_t i = 0; i < petIds.size(); ++i)
	{
		int petId = petIds[i];
		if (m_data.petId == petId) continue;
		auto pet = PetManager::petMgr()->getPetById(petId);
		if (pet && !pet->canUseSkill())
		{
			highLightPetsId.push_back(petId);
		}
		
	}
	StageLayersMgr::theMgr()->highLightPets(highLightPetsId);
}

void PetMonkey::toPetSkill(int petId)
{
	StageOp->addPetEnergy(petId, m_data.skillPower);
}
//////////////////////////////////////////////////////////////////////////////

void PetRooster::noTargetSkill()
{
	StageOp->loadDesignatedStar(m_data.color, m_data.skillPower);//使用技能也算一步 需要+1
}
//////////////////////////////////////////////////////////////////////////////
void PetDog::noTargetSkill()
{
	StageOp->randomReplaceStars(m_data.petId, kKey, kColorRandom, m_data.skillPower);

}
//////////////////////////////////////////////////////////////////////////////
void PetPig::toPetSkill(int petId)
{

}
