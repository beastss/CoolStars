#ifndef __PETSAVINGHELPER_H__
#define __PETSAVINGHELPER_H__
#include "cocos2d.h"
#include "sqlite3.h"
#include "PetEntity.h"

class PetSavingHelper
{
public:
	static void setPetState(const PetData &data);
	static PetData getPetState(int petId);

	static void recordCurActivePets();
	static std::vector<int> getCurActivePets();
};

#endif
