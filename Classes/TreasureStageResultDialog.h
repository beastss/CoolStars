#ifndef __PAUSE_LAYER_H__
#define __PAUSE_LAYER_H__

#include "cocos2d.h"
#include "ScaleDialog.h"
class UiLayout;
class TreasureStageResultDialog
	: public ScaleDialog
{
public:
	static TreasureStageResultDialog *create();
    virtual bool init();
private:
	TreasureStageResultDialog();
	void initLayout();
	
	void onToQuitBtnClicked(cocos2d::CCObject* pSender);
	void onReplayBtnClicked(cocos2d::CCObject* pSender);
private:
	UiLayout *m_layout;
public:
};

#endif