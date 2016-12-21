#ifndef __THIEFDIALOG_H__
#define __THIEFDIALOG_H__
#include "cocos2d.h"
#include "ScaleDialog.h"

class UiLayout;

class ThiefDialog
	: public ScaleDialog
{
public:
	static ThiefDialog *create();

private:
	ThiefDialog();
	virtual bool init();
	void initLayout();
	void onConfirmBtnClicked(cocos2d::CCObject* pSender);

private:
	UiLayout *m_layout;
};
#endif