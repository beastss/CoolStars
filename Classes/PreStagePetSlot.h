#ifndef __PRESTAGEPETSLOT_H__
#define __PRESTAGEPETSLOT_H__
#include "cocos2d.h"
#include "PreStageModel.h"

class UiLayout;
class ListPetView;

class PetSkillIcon : public cocos2d::CCSprite
{
public:
	static PetSkillIcon *create(int petId);
	virtual bool init();
private:
	PetSkillIcon(int petId) : m_petId(petId){}
private:
	int m_petId;
};


class PreStagePetSlotNode : public cocos2d::CCNode
{
public:
	static PreStagePetSlotNode *create(int petId);
	int getPetId(){ return m_petId; }
	std::string getBgPath();
private:
	PreStagePetSlotNode(int petId) : m_petId(petId){}
	virtual bool init();
private:
	int m_petId;
	UiLayout *m_layout;
};

class PreStagePetSlot
	: public cocos2d::CCNode
	, public IPreStageView
{
public:
	static PreStagePetSlot *create(int petId);
	virtual bool init();
	
private:
	PreStagePetSlot(int petId): m_curPetId(petId){}
	void initLayout();
	void refreshList();
	int getPetIdByListIndex(int index);
	void onSelectItemCallback(int index);
	virtual void onEnter();
	virtual void onExit();
	virtual void onPetSelect(int oldPetId);
	int getIndexByPetId(int petId);
private:
	UiLayout *m_layout;
	ListPetView *m_listView;
	int m_curPetId;
	const int kInvalidIndex = -1;
};
#endif