#ifndef __PETVIEW_H__
#define __PETVIEW_H__
#include "cocos2d.h"
class UiLayout;
class PetEntity;
class PetView : public cocos2d::CCNode
{
public:
	static PetView *create(int petId);
	virtual ~PetView(){}
	virtual bool init();
	void initLayout();
private:
	PetView(int petId);
private:
	int m_petId;
	UiLayout *m_layout;
	const PetEntity *m_model;
};

#endif