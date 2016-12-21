#ifndef _PETUPGRADETIPS_H_
#define _PETUPGRADETIPS_H_

#include "ScaleDialog.h"
class UiLayout;

class PetUpgradeTipsNode
	: public cocos2d::CCNode
{
public:
	static PetUpgradeTipsNode *create(int petId, int lv);
private:
	PetUpgradeTipsNode(int petId, int lv);
	virtual bool init();
private:
	UiLayout *m_layout; 
	int m_petId;
	int m_lv;
};


class PetUpgradeTips
	: public cocos2d::CCNode
	//: public ScaleDialog
{
public:
	static PetUpgradeTips *create(int petId);
private:
	PetUpgradeTips(int petId);
	virtual bool init();
private:
	UiLayout *m_layout;
	int m_petId;
};
#endif