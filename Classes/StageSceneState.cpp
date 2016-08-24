#include "StageSceneState.h"
#include "StarsController.h"
#include "StageUiLayer.h"
#include "StarsLayer.h"
#include "StageOperator.h"
#include "PropManager.h"
#include "PetManager.h"
#include "GuideMgr.h"
#include "StarsEraseModule.h"
#include "SoundMgr.h"
USING_NS_CC;
using namespace std;
StageStateOwner::StageStateOwner()
{
	
}

void StageStateOwner::init()
{
	m_normalState = new StageNormalState(this);
	m_propsState = new StagePropsClickState(this);
	m_petSkillState = new StagePetSkillState(this);
	enterNormalState();
}

StageStateOwner::~StageStateOwner()
{
	delete m_normalState;
	delete m_propsState;
	delete m_petSkillState;
}

void StageStateOwner::clickStar(const LogicGrid &grid)
{
	SoundMgr::theMgr()->playEffect(kEffectMusicButton);
	GuideMgr::theMgr()->endGuide(kGuideEnd_star_clicked);
	m_curState->clickStar(grid);
}

void StageStateOwner::clickPet(int petId)
{
	SoundMgr::theMgr()->playEffect(kEffectMusicButton);
	GuideMgr::theMgr()->endGuide(kGuideEnd_pet_clicked);
	m_curState->clickPet(petId);
}

void StageStateOwner::clickProp(int propId)
{
	SoundMgr::theMgr()->playEffect(kEffectMusicButton);
	m_curState->clickProp(propId);
}

void StageStateOwner::enterNormalState()
{
	m_curState = m_normalState;
	StageLayersMgr::theMgr()->toNormalState();
}

void StageStateOwner::enterPropsClickState(int propType)
{
	m_propsState->setCurProp(propType);
	m_curState = m_propsState;
}

void StageStateOwner::enterPetSkillState(int petId)
{
	m_petSkillState->setCurPet(petId);
	m_curState = m_petSkillState;
	auto pet = PetManager::petMgr()->getPetById(petId);
	if (pet)
	{
		pet->skillInit();
	}
	GuideMgr::theMgr()->startGuide(kGuideStart_stage_in_pet_skill_state);
}

////////////////////////////////////////////////////////////////////////////////////
StageSceneState::StageSceneState(StageStateOwner *owner)
{
	m_owner = owner;
	m_starsLayer = owner->getStarsLayer();
	m_uiLayer = owner->getUiLayer();
}
////////////////////////////////////////////////////////////////////////////////////
void StageNormalState::clickStar(const LogicGrid &grid)
{
	StarsEraseModule::theModel()->handleClick(grid);
}

void StageNormalState::clickPet(int petId)
{
	auto pet = PetManager::petMgr()->getPetById(petId);
	if (!pet || !pet->canUseSkill()) return;
	int targetType = pet->getPetData().skillTarget;
}

void StageNormalState::clickProp(int propId)
{
	if (propId == kPropReorder)
	{
		PropManager::propMgr()->usePropReorder();
	}
	else if (propId == kPropBrush)
	{
		PropManager::propMgr()->highLightCanBrushArea();
		m_owner->enterPropsClickState(propId);
	}
	else
	{
		PropManager::propMgr()->highLightCanBombArea();
		m_owner->enterPropsClickState(propId);
	}
}

////////////////////////////////////////////////////////////////////////////////////
void StagePropsClickState::clickStar(const LogicGrid &grid)
{
	StarNode *node = StarsController::theModel()->getStarNode(grid);
	if (!node) return;
	int starType = node->getAttr().type;
	if (m_curType == kPropBrush)
	{
		if (starType == kColorStar)
		{
			m_uiLayer->showChangeColorPanel(node->getAttr().color, grid);
		}
		else
		{
			m_owner->enterNormalState();
		}
	}
	else if (m_curType == kPropBomb)
	{
		PropManager::propMgr()->usePropBomb(starType, grid);
		m_owner->enterNormalState();
	}

}

void StagePropsClickState::clickPet(int petId)
{
	m_owner->enterNormalState();
}
////////////////////////////////////////////////////////////////////////////////////
void StagePetSkillState::clickStar(const LogicGrid &grid)
{
	auto pet = PetManager::petMgr()->getPetById(m_curPetId);
	if (pet)
	{
		//pet->useToStarSkill(grid); 
		outPetSkillState();
	}
}

void StagePetSkillState::clickPet(int petId)
{
	auto pet = PetManager::petMgr()->getPetById(m_curPetId);
	if (pet)
	{
		//pet->useToPetSkill(petId);
		outPetSkillState();
	}
}

void StagePetSkillState::outPetSkillState()
{
	m_owner->enterNormalState();
	GuideMgr::theMgr()->endGuide(kGuideStart_stage_in_pet_skill_state);
}