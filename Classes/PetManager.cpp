#include "PetManager.h"
#include "DataManager.h"
#include "PetSavingHelper.h"
#include "CommonMacros.h"
#include <algorithm>
PetManager::PetManager()
{

}

PetManager::~PetManager()
{

}

PetManager *PetManager::petMgr()
{
	static PetManager mgr;
	return &mgr;
}

void PetManager::init()
{
	auto openingPets = DataManagerSelf->getOpeningPetIds();
	for (size_t i = 0; i < openingPets.size(); ++i)
	{
		int petId = openingPets[i];
		m_pets[petId] = PetEntity::PetFactory(petId);
	}

	m_curPets = PetSavingHelper::getCurActivePets();
	for (size_t i = 0; i < m_curPets.size(); ++i)
	{
		assert(ownedThisPet(m_curPets[i]));
	}
}

vector<int> PetManager::getOwnedPetIds()
{
	vector<int> ids;
	for (auto iter = m_pets.begin(); iter != m_pets.end(); ++iter)
	{
		int petId = iter->first;
		if (ownedThisPet(petId))
		{
			ids.push_back(iter->first);
		}
	}
	return ids;
}

vector<int> PetManager::getNotOwnedPetIds()
{
	vector<int> ids;
	for (auto iter = m_pets.begin(); iter != m_pets.end(); ++iter)
	{
		int petId = iter->first;
		if (!ownedThisPet(petId))
		{
			ids.push_back(iter->first);
		}
	}
	return ids;
}

bool PetManager::ownedThisPet(int id)
{
	auto pet = getPetById(id);
	if (!pet) return false;
	return pet->getPetData().isOwned;
}

void PetManager::addNewPet(int petId)
{
	auto pet = getPetById(petId);
	assert(pet);
	if (pet)
	{
		if (!ownedThisPet(petId))
		{
			pet->getThisNewPet();
			NOTIFY_VIEWS(onNewPetAdd);
		}
	}
}

PetEntity *PetManager::getPetById(int id)
{
	if (m_pets.find(id) != m_pets.end())
	{
		return m_pets[id];
	}
	return NULL;
}

void PetManager::setCurPets(std::vector<int> &ids)
{ 
	m_curPets = ids; 
	PetSavingHelper::recordCurActivePets();
}

void PetManager::addPetEnergy(int petId, int value)
{
	if (find(m_curPets.begin(), m_curPets.end(), petId) == m_curPets.end()) return;

	auto pet = getPetById(petId);
	if (pet)
	{
		auto data = pet->getPetData();
		pet->setEnergy(data.energy + value);
	}
}

void PetManager::petEnergyChanged(int petId, int oldEnergy, int newEnergy)
{
	NOTIFY_VIEWS(onPetEnergyChanged, petId, oldEnergy, newEnergy);
}

void PetManager::newStageInit()
{
	for (size_t i = 0; i < m_curPets.size(); ++i)
	{
		auto pet = getPetById(m_curPets[i]);
		if (pet)
		{
			pet->setEnergy(0);
		}
	}
}

void PetManager::addView(IPetView *view)
{
	auto iter = find(m_views.begin(), m_views.end(), view);
	if (iter == m_views.end())
	{
		m_views.push_back(view);
	}
}

void PetManager::removeView(IPetView *view)
{
	auto iter = find(m_views.begin(), m_views.end(), view);
	if (iter != m_views.end())
	{
		m_views.erase(iter);
	}
}

bool PetManager::hasPetToUpgrade()
{
	auto ids = getOwnedPetIds();
	for (size_t i = 0; i < ids.size(); ++i)
	{
		auto pet = getPetById(ids[i]);
		if (pet && pet->canUpgrade())
		{
			return true;
		}
	}
	return false;
}

bool PetManager::usePetSkill()
{
	for (size_t i = 0; i < m_curPets.size(); ++i)
	{
		auto pet = getPetById(m_curPets[i]);
		if (pet && pet->canUseSkill())
		{
			pet->triggerSkill();
			NOTIFY_VIEWS(onUsePetSkill, pet->getPetData().petId);
			return true;
		}
	}
	return false;
}