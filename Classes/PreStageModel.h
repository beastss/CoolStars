#ifndef __PRESTAGEMODEL_H__
#define __PRESTAGEMODEL_H__
#include "cocos2d.h"

#define NOT_SELECT_PET_ID 0

struct IPreStageView
{
	virtual void onPetSelect(int oldPetId){}
};

class PreStageModel
{
public:
	static PreStageModel *theModel();
	void addView(IPreStageView *view);
	void removeView(IPreStageView *view);

	void selectPet(int newPetId, int oldPetId);
	std::vector<int> getPetsCanSelect(int curPetId);
	
	void confirmCurPets();
	void init();
private:
	std::vector<int> m_selectedPets;
	std::vector<IPreStageView *>m_views;
};
#endif