#ifndef __PETMANAGER_H__
#define __PETMANAGER_H__
#include "cocos2d.h"
#include "PetEntity.h"
#include "CommonMacros.h"

class PetEntity;

struct IPetView
{
	virtual void onPetEnergyChanged(int petId, int oldEnergy, int newEnergy){}
	virtual void onNewPetAdd(int petId){}
	virtual void onUsePetSkill(int petId){}
};

class PetManager
{
public:
	static PetManager *petMgr();
	void init();
	PetEntity *getPetById(int id);
	std::vector<int> getOwnedPetIds();//包括普通关卡宠物和休闲模式宠物
	std::vector<int> getNotOwnedPetIds();
	std::vector<int> getOwnedStagePetIds(bool normalStage);
	int getMaxPetsAmount(){ return m_pets.size(); }
	bool ownedThisPet(int id);
	void addNewPet(int petId);
	void addTempPet(int petId, int level);
	void removeTempPets();

	//当前选中的宠物 ，将在关卡里出现
	const std::vector<int> &getCurPetIds() { return m_curPets; }
	void setCurPets(std::vector<int> &ids);

	void addPetEnergy(int petId, int value);
	void petEnergyChanged(int petId, int oldEnergy, int newEnergy);
	void newStageInit();
	bool hasPetToUpgrade();
	int getCanUpgradePetId();
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
	std::vector<int> m_tempPets;//临时试用的宠物
};
#endif