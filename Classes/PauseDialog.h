#ifndef __PAUSE_LAYER_H__
#define __PAUSE_LAYER_H__

#include "cocos2d.h"
#include "ScaleDialog.h"
class UiLayout;
class PauseDialog
	: public ScaleDialog
{
public:
    CREATE_FUNC(PauseDialog);
    virtual bool init();

	void onResume(cocos2d::CCObject* pSender);
	void onMute(cocos2d::CCObject* pSender);
	void onSound(cocos2d::CCObject* pSender);
	void onBackHome(cocos2d::CCObject* pSender);
	void onPurchase(cocos2d::CCObject* pSender);
private:
	PauseDialog(void);
	void initLayout();
	void setMute(bool isMute);
private:
	UiLayout *m_layout;
};

#endif