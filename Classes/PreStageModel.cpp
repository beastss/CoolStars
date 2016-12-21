#include "PreStageModel.h"
#include "PetManager.h"
#include "CommonMacros.h"
#include <algorithm>
using namespace std;
USING_NS_CC;

PreStageModel *PreStageModel::theModel()
{
	static PreStageModel model;
	return &model;
}

void PreStageModel::selectPet(int newPetId, int oldPetId)
{
	auto petMgr = PetManager::petMgr();
	auto iter = find(m_selectedPets.begin(), m_selectedPets.end(), oldPetId);
	if (iter != m_selectedPets.end())
	{
		m_selectedPets.erase(iter);
	}

	iter = find(m_selectedPets.begin(), m_selectedPets.end(), newPetId);
	if (iter == m_selectedPets.end() && petMgr->ownedThisPet(newPetId))
	{
		m_selectedPets.push_back(newPetId);
	}

	NOTIFY_VIEWS(onPetSelect, oldPetId);
}

std::vector<int> PreStageModel::getPetsCanSelect(int curPetId)
{

	vector<int> diff(m_canSelectPets.size());
	sort(m_canSelectPets.begin(), m_canSelectPets.end());
	sort(m_selectedPets.begin(), m_selectedPets.end());

	auto iter = set_difference(m_canSelectPets.begin(), m_canSelectPets.end(), m_selectedPets.begin(), m_selectedPets.end(), diff.begin());
	
	//添加当前petid 和 缺省不存在宠物的petid 
	vector<int> result(diff.begin(), iter);
	if (find(result.begin(), result.end(), curPetId) == result.end())
	{
		result.push_back(curPetId);
	}
	if (find(result.begin(), result.end(), NOT_SELECT_PET_ID) == result.end())
	{
		result.push_back(NOT_SELECT_PET_ID);
	}
	sort(result.begin(), result.end());
	return result;
}

void PreStageModel::addView(IPreStageView *view)
{
	auto iter = find(m_views.begin(), m_views.end(), view);
	if (iter == m_views.end())
	{
		m_views.push_back(view);
	}
}

void PreStageModel::removeView(IPreStageView *view)
{
	auto iter = find(m_views.begin(), m_views.end(), view);
	if (iter != m_views.end())
	{
		m_views.erase(iter);
	}
}

void PreStageModel::confirmCurPets()
{
	PetManager::petMgr()->setCurPets(m_selectedPets);
	PetManager::petMgr()->newStageInit();
}

void PreStageModel::init()
{
	m_selectedPets.clear();
	m_canSelectPets = PetManager::petMgr()->getOwnedStagePetIds(true);
}

