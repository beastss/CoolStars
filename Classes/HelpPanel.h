#ifndef __HELPPANEL_H__
#define __HELPPANEL_H__
#include "cocos2d.h"
#include "ScaleDialog.h"

class UiLayout;

class HelpPanel
	: public ScaleDialog
{
public:
	CREATE_FUNC(HelpPanel);
	virtual bool init();
	
private:
	void initLayout();
	void onConfirmBtnClicked(cocos2d::CCObject* pSender);
private:
	UiLayout *m_layout;
};
#endif