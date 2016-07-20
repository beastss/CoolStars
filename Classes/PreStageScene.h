#ifndef __PRESTAGESCENE_H__
#define __PRESTAGESCENE_H__
#include "cocos2d.h"
#include "BasePanel.h"

class UiLayout;
class TitlePanel;
class PreStageScene
	: public BasePanel
{
public:
	CREATE_FUNC(PreStageScene);
	virtual bool init();
	
private:
	void initMainLayout();
	void initBottomLayout();
	void toStartGame(cocos2d::CCObject* pSender);
	void hideHomeBackBtn();
private:
	TitlePanel *m_titlePanel;
	UiLayout *m_mainLayout;
	UiLayout *m_bottomLayout;
};
#endif