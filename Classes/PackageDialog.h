#ifndef __PACKAGEDIALOG_H__
#define __PACKAGEDIALOG_H__ 
#include "cocos2d.h"
#include "BasePanel.h"
#include "ScaleDialog.h"
#include <functional>
#include "MsgNotifier.h"

class UiLayout;

class PackageDialog: public ScaleDialog
{
public:
	static PackageDialog *create(int type);
	void setConfirmHandle(std::function<void()> handle){ m_confirmHandle = handle; }
	void setCancelHandle(std::function<void()> handle){ m_cancelHandle = handle; }

private:
	PackageDialog(int type);
	virtual bool init();
	void initLayout();
	void onCancelBtnClicked(cocos2d::CCObject* pSender);
	void onBuyBtnClicked(cocos2d::CCObject* pSender);
	
private:
	UiLayout *m_layout;
	std::function<void()> m_confirmHandle;
	std::function<void()> m_cancelHandle;
	int m_type;
};

class PackageScene
	: public BasePanel
	, public cocos2d::CCTouchDelegate
	, public IMsgWatcherView
{
public:
	CREATE_FUNC(PackageScene);
private:
	virtual void onEnter();
	virtual void onExit();
	virtual bool init();
	virtual bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);

	virtual void onThiefShowUp();
	virtual void onThiefDisappear();
	virtual void onBackKeyTouched();
private:	
	cocos2d::CCSprite *m_thief;
};
#endif