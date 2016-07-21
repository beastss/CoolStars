#ifndef __PETSCENE_H__
#define __PETSCENE_H__ 
#include "cocos2d.h"
#include "BasePanel.h"
#include <unordered_map>
#include "PetSceneMoveHelper.h"
#include "PetManager.h"
class UiLayout;

class PetScene 
	: public BasePanel
	, public IPetView
{
public:
	CREATE_FUNC(PetScene);
	virtual bool init();
private:
	PetScene(){}
	~PetScene(){}
	virtual void onEnter();
	virtual void onExit();
	void initMainLayout();
	void initBottomLayout();
	void initColorPets();
	void refreshUi();
	void refreshUpgrdeCost();
	void refreshArrows();
	void refreshPetPackage();
	void changePetsColor(int color);
private:
	void onLeftPetBtnClicked(cocos2d::CCObject* pSender);
	void onRigthPetBtnClicked(cocos2d::CCObject* pSender);
	void onUpgradeBtnClicked(cocos2d::CCObject* pSender);
	void onPetPackageBtnClicked(cocos2d::CCObject* pSender);

	void onBackBtnClicked(cocos2d::CCObject* pSender);
	void onGreenPetBtnClicked(cocos2d::CCObject* pSender);
	void onPurplePetBtnClicked(cocos2d::CCObject* pSender);
	void onRedPetBtnClicked(cocos2d::CCObject* pSender);
	void onBluePetBtnClicked(cocos2d::CCObject* pSender);
	void onYellowPetBtnClicked(cocos2d::CCObject* pSender);
	void handleColorBtnClicked(int color);

	virtual void onBackKeyTouched();
private:
	virtual void onNewPetAdd();
private:
	UiLayout *m_bottomLayout;
	UiLayout *m_mainLayout;
	UiLayout *m_emptyPetLayout;
	PetSceneMoveHelper m_moveHelper;
	static const float kBtnSelectedScale;
	std::unordered_map<int, std::vector<int>>m_colorPets;
	static int s_curPetColor;
	int m_curColorPetIndex;
};
#endif