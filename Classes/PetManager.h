#ifndef __PETMANAGER_H__
#define __PETMANAGER_H__
#include "cocos2d.h"
#include "PetEntity.h"
#include "CommonMacros.h"

class PetEntity;

struct IPetView
{
	virtual void onPetEnergyChanged(int petId, int oldEnergy, int newEnergy){}
	virtual void onNewPetAdd(){}
	virtual void onUsePetSkill(int petId){}
};

class PetManager
{
public:
	static PetManager *petMgr();
	void init();
	PetEntity *getPetById(int id);
	std::vector<int> getOwnedPetIds();
	std::vector<int> getNotOwnedPetIds();
	int getMaxPetsAmount(){ return m_pets.size(); }
	bool ownedThisPet(int id);
	void addNewPet(int petId);

	//当前选中的宠物 ，将在关卡里出现
	const std::vector<int> &getCurPetIds() { return m_curPets; }
	void setCurPets(std::vector<int> &ids);

	void addPetEnergy(int petId, int value);
	void petEnergyChanged(int petId, int oldEnergy, int newEnergy);
	void newStageInit();
	bool hasPetToUpgrade();
	bool usePetSkill();
public:
	void addView(IPetView *view);
	void removeView(IPetView *view);
private:
	PetManager();
	~PetManager();
private:
	std::map<int, PetEntity *> m_pets;
	std::vector<int> m_curPets;
	std::vector<IPetView *>m_views;
};
#endif