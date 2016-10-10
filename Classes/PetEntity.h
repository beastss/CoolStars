#ifndef __PETENTITY_H__
#define __PETENTITY_H__
#include "DataManager.h"
#include "LogicGridUtil.h"

class PetSkill;

struct PetData
{
	//不变
	int petId;
	int commonid;
	int color;
	std::string petImgRes;
	std::string petSkillRes;
	std::string petAnimationRes;
	std::string petNameRes;
	int skillTarget;
	int maxLevel;
	std::string skillDescRes;
	int diamondCost;
	//升级后变化
	int maxEnergy;
	int skillPower;
	int foodToUpgrade;
	//可变
	int level;
	int energy;
	bool isOwned;//已经被拥有
};

enum PetType
{
	kPetRat = 1,
	kPetOx,
	kPetTiger,
	kPetRabbit,
	kPetDragon,
	kPetSnake,
	kPetHorse,
	kPetGoat,
	kPetMonkey,
	kPetRooster,
	kPetDog,
	kPetPig,
};

enum PetSkillTarget
{
	kNoTarget = 1,
	kTargetStar,
	kTargetPet,
};

class PetEntity
{
public:
	virtual ~PetEntity();
	const PetData &getPetData()const{ return m_data; }

	void setEnergy(int energy);
	bool isEnergyFull() const{ return m_data.energy >= m_data.maxEnergy; }

	bool isMaxLevel();
	bool canUpgrade();
	void upgrade();
	void getThisNewPet();
	void triggerSkill();
	void setLevel(int level);
	
	static PetEntity *PetFactory(int petId);
protected:
	PetEntity(int petId);
	void refreshPetData();
public:
	virtual void skillInit(){}
	virtual bool canUseSkill() { return isEnergyFull(); }
private:
	virtual void useSkill(){}
protected:
	PetData m_data;
};


class PetRat: public PetEntity
{
public:
	PetRat(int petId) : PetEntity(petId){}
	virtual bool canUseSkill();
	virtual void useSkill();
};

class PetOx : public PetEntity
{
public:
	PetOx(int petId) : PetEntity(petId){}
	virtual bool canUseSkill();
	virtual void useSkill();
};

class PetTiger : public PetEntity
{
public:
	PetTiger(int petId) : PetEntity(petId){}
	virtual void useSkill();
};

class PetRabbit : public PetEntity
{
public:
	PetRabbit(int petId) : PetEntity(petId){}
	virtual void useSkill();
};

class PetDragon : public PetEntity
{
public:
	PetDragon(int petId) : PetEntity(petId){}
	virtual bool canUseSkill();
	virtual void useSkill();
};

class PetSnake : public PetEntity
{
public:
	PetSnake(int petId) : PetEntity(petId){}
	virtual void useSkill();
};

class PetHorse : public PetEntity
{
public:
	PetHorse(int petId) : PetEntity(petId){}
	virtual void useSkill();
};

class PetGoat : public PetEntity
{
public:
	PetGoat(int petId) : PetEntity(petId){}
	virtual void useSkill();
};

class PetMonkey : public PetEntity
{
public:
	PetMonkey(int petId) : PetEntity(petId){}
	virtual void useSkill();
	virtual bool canUseSkill();
};

class PetRooster : public PetEntity
{
public:
	PetRooster(int petId) : PetEntity(petId){}
	virtual void useSkill();
};

class PetDog : public PetEntity
{
public:
	PetDog(int petId) : PetEntity(petId){}
	virtual void useSkill();
};

class PetPig : public PetEntity
{
public:
	PetPig(int petId) : PetEntity(petId){}
	virtual void useSkill();
};

#endif