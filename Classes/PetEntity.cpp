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
#include "CommonUtil.h"
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
	m_data.diamondCost = commonData.diamondCost;
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

void PetEntity::setLevel(int level)
{
	m_data.level = min(max(1, level), m_data.maxLevel);
	m_data.isOwned = level != 0;
	refreshPetData();
	//不做保存
}

void PetEntity::setEnergy(int energy)
{
	int value = max(0, min(energy, m_data.maxEnergy));
	int oldEnergy = m_data.energy;
	m_data.energy = value;
	
	PetManager::petMgr()->petEnergyChanged(m_data.petId, oldEnergy, value);
}

void PetEntity::triggerSkill()
{
	if (canUseSkill())
	{
		setEnergy(0);
		useSkill();
		GuideMgr::theMgr()->startGuide(kGuideStart_stage_petFullPower);
	}
}

static vector<LogicGrid> getSameColorGrids(int color)
{
	vector<LogicGrid> grids;
	auto nodes = StarsController::theModel()->getStarNodes();

	for (size_t i = 0; i < nodes.size(); ++i)
	{
		auto attr = nodes[i]->getAttr();
		if (attr.type == kColorStar && attr.color == color)
		{
			grids.push_back(attr.grid);
		}
	}
	return grids;
}
//////////////////////////////////////////////////////////////////////////////
void PetRat::useSkill()
{
	auto grids = getSameColorGrids(m_data.color);
	if (!grids.empty())
	{
		int index = CommonUtil::getRandomValue(0, grids.size() - 1);
		StageOp->petScaleErase(m_data.petId, grids[index], m_data.skillPower / 2, 0);
	}
}

bool PetRat::canUseSkill()
{
	return isEnergyFull() && !getSameColorGrids(m_data.color).empty();
}
//////////////////////////////////////////////////////////////////////////////
void PetOx::useSkill()
{
	auto grids = getSameColorGrids(m_data.color);
	if (!grids.empty())
	{
		int index = CommonUtil::getRandomValue(0, grids.size() - 1);
		StageOp->petScaleErase(m_data.petId, grids[index], 0, m_data.skillPower / 2);
	}
}

bool PetOx::canUseSkill()
{
	return isEnergyFull() && !getSameColorGrids(m_data.color).empty();
}
//////////////////////////////////////////////////////////////////////////////

void PetTiger::useSkill()
{
	StageOp->petAddSteps(m_data.petId, m_data.skillPower);//如果使用技能也算一步 需要+1
	StarsController::theModel()->preOneRound();
}
//////////////////////////////////////////////////////////////////////////////

void PetRabbit::useSkill()
{
	StageOp->randomReplaceStars(m_data.petId, kColorStar, m_data.color, m_data.skillPower);
}
//////////////////////////////////////////////////////////////////////////////
void PetDragon::useSkill()
{
	auto grids = getSameColorGrids(m_data.color);
	if (!grids.empty())
	{
		int index = CommonUtil::getRandomValue(0, grids.size() - 1);
		StageOp->petScaleErase(m_data.petId, grids[index], m_data.skillPower, m_data.skillPower);
	}
}

bool PetDragon::canUseSkill()
{
	return isEnergyFull() && !getSameColorGrids(m_data.color).empty();
}

//////////////////////////////////////////////////////////////////////////////
void PetSnake::useSkill()
{
	StageLayersMgr::theMgr()->doubleScore();
	auto info = StageDataMgr::theMgr();
	info->setNextScoreBonus(m_data.skillPower);
	StarsController::theModel()->preOneRound();
}
//////////////////////////////////////////////////////////////////////////////

void PetHorse::useSkill()
{
	StageOp->randomReplaceStars(m_data.petId, kDiamond, kColorRandom, m_data.skillPower);
}
//////////////////////////////////////////////////////////////////////////////

void PetGoat::useSkill()
{
	StageOp->petRandomErase(m_data.petId, m_data.skillPower);
}
//////////////////////////////////////////////////////////////////////////////
static vector<int> getNotFullEnergyPets(int selfId)
{
	vector<int> targetPetIds;
	std::vector<int> petIds = PetManager::petMgr()->getCurPetIds();
	for (size_t i = 0; i < petIds.size(); ++i)
	{
		int petId = petIds[i];
		if (selfId == petId) continue;
		auto pet = PetManager::petMgr()->getPetById(petId);
		if (pet && !pet->isEnergyFull())
		{
			targetPetIds.push_back(petId);
		}
	}
	return targetPetIds;
}

void PetMonkey::useSkill()
{
	auto targetPetIds = getNotFullEnergyPets(m_data.petId);
	if (!targetPetIds.empty())
	{
		int index = CommonUtil::getRandomValue(0, targetPetIds.size() - 1);
		StageOp->addPetEnergy(m_data.petId, targetPetIds[index], m_data.skillPower);
		StarsController::theModel()->preOneRound();
	}
}

bool PetMonkey::canUseSkill()
{
	return isEnergyFull() && !getNotFullEnergyPets(m_data.petId).empty();
}
//////////////////////////////////////////////////////////////////////////////
void PetRooster::useSkill()
{
	StageOp->loadDesignatedStar(m_data.color, m_data.skillPower);//使用技能也算一步 需要+1
	StarsController::theModel()->preOneRound();
}
//////////////////////////////////////////////////////////////////////////////
void PetDog::useSkill()
{
	StageOp->randomReplaceStars(m_data.petId, kKey, kColorRandom, m_data.skillPower);
}
//////////////////////////////////////////////////////////////////////////////
void PetPig::useSkill()
{

}
