#ifndef _TREASURESKIPSTAGEDIALOG_H_
#define _TREASURESKIPSTAGEDIALOG_H_

#include "cocos2d.h"
#include "ScaleDialog.h"
class UiLayout;
class TreasureSkipStageDialog
	: public ScaleDialog
{
public:
	static TreasureSkipStageDialog *create();
	virtual bool init();
private:
	TreasureSkipStageDialog();
	void initLayout();

	void onCancel(cocos2d::CCObject* pSender);
	void onConfirm(cocos2d::CCObject* pSender);
private:
	UiLayout *m_layout;
public:
};

#endif