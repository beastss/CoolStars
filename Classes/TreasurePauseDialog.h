#ifndef _TREASUREPAUSEDIALOG_H_
#define _TREASUREPAUSEDIALOG_H_

#include "cocos2d.h"
#include "ScaleDialog.h"
class UiLayout;
class TreasurePauseDialog
	: public ScaleDialog
{
public:
	static TreasurePauseDialog *create();
	virtual bool init();
private:
	TreasurePauseDialog();
	void initLayout();

	void onLeaveGame(cocos2d::CCObject* pSender);
	void onCancel(cocos2d::CCObject* pSender);
private:
	UiLayout *m_layout;
public:
};

#endif