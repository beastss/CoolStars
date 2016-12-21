#ifndef _TREASURE_STAGE_SCENE_H_
#define _TREASURE_STAGE_SCENE_H_
#include "cocos2d.h"
#include "cocos-ext.h"
#include "BasePanel.h"
#include "TreasureStageNotifier.h"

class UiLayout;
class TitlePanel;
class TreasureStarsLayer;
class NoTouchLayer;
class ActionRunner;
class TreasureStageScene
	: public BasePanel
	, public ITreasureStageView
{
public:
	CREATE_FUNC(TreasureStageScene);
	virtual bool init();

private:
	TreasureStageScene();
	~TreasureStageScene();
	void initMainLayout();
	void initTopLayout();
	void initBottomLayout();
	void initPets();
	void bubbleFinished(cocos2d::extension::CCArmature *, cocos2d::extension::MovementEventType, const char *);
	void onBackBtnClicked(cocos2d::CCObject* pSender);
	void hideHomeBackBtn();
	void showTargetPanel();
private:
	virtual void onEnter();
	virtual void onExit();
	virtual void onBackKeyTouched();
	virtual void update(float dt);
	virtual void onPassCurStage(int stage);
	virtual void onStageOverStart(int stage, bool won);
	virtual void onStageOverEnd(int stage, bool won);
	virtual void onEraseStars(int num, int score);
	virtual void onStarExplore(int color, int score, cocos2d::CCPoint pt);
	virtual void onNewStage();
private:
	UiLayout *m_topLayout;
	UiLayout *m_mainLayout;
	UiLayout *m_bottomLayout;
	TreasureStarsLayer *m_starsLayer;
	NoTouchLayer *m_noTouchLayer;
	cocos2d::CCNode *m_effectLayer;
	int m_uiScore;
	ActionRunner *m_runner;
};
#endif