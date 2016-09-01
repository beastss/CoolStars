#ifndef _MENUSCENE_H
#define _MENUSCENE_H
#include "cocos2d.h"
#include "SettingBtnsHelper.h"
#include "BasePanel.h"
#include "GuideDelegate.h"
#include "MsgNotifier.h"

class UiLayout;
class MenuScene
	: public BasePanel 
	, public IMsgWatcherView
{
public:  
	virtual bool init();  
	CREATE_FUNC(MenuScene); 
private:
	void initMainLayout();
	void initBottomLayout();

	void toNormalGame(cocos2d::CCObject* pSender);
	void toTreasureGame(cocos2d::CCObject* pSender);
	void drawLottery(cocos2d::CCObject* pSender);
	void toPetPanel(cocos2d::CCObject* pSender);

	void toSetting(cocos2d::CCObject* pSender);
	void toMute(cocos2d::CCObject* pSender);
	void toSound(cocos2d::CCObject* pSender);
	void toHelpPanel(cocos2d::CCObject* pSender);
	void toRankPanel(cocos2d::CCObject* pSender);
	void toPackagePanel(cocos2d::CCObject* pSender);
	void toShopPanel(cocos2d::CCObject* pSender);
private:
	void justShowNormalGameBtn();
	void refreshPetTips();
	virtual void onEnter();
	virtual void onExit();
	virtual void onBackKeyTouched();
	virtual void onPetPackageBuy();
	void refreshPackage();
private:
	UiLayout *m_mainLayout;
	UiLayout *m_bottomLayout;
	SettingBtnsHelper m_settingBtnsHelper;
};

#endif