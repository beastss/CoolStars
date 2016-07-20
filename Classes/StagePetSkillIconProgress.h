#ifndef __STAGEPETSKILLICON_H__
#define __STAGEPETSKILLICON_H__
#include <functional>
#include "cocos2d.h"
#include "PetManager.h"

class UiLayout;
class StagePetSkillIconProgress
	: public cocos2d::CCNode
	, public IPetView
{
public:
	static StagePetSkillIconProgress *create(int petId);
	void runEnergyAddAction(int oldEnergy);
private:
	StagePetSkillIconProgress(int petId):m_petId(petId){}
	virtual bool init();
	virtual void onEnter();
	virtual void onExit();
	void setPercentage(float value, bool withAction);
	virtual void onPetEnergyChanged(int petId, int oldEnergy, int newEnergy);
private:
	int m_petId;
	UiLayout *m_layout;
	cocos2d::CCProgressTimer *m_progress;
};
#endif