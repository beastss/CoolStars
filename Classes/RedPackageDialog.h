#ifndef __REDPACKAGEDIALOG_H__
#define __REDPACKAGEDIALOG_H__
#include "cocos2d.h"
#include "ScaleDialog.h"
#include <functional>
class UiLayout;

class RedPackageDialog
	: public ScaleDialog
{
public:
	static RedPackageDialog *create();

private:
	RedPackageDialog();
	virtual bool init();
	void initLayout();
	void onConfirmBtnClicked(cocos2d::CCObject* pSender);

private:
	UiLayout *m_layout;
};
#endif