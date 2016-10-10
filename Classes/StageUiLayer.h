#ifndef __STAGEUILAYER_H___
#define __STAGEUILAYER_H___ 

#include "cocos2d.h"
#include "StarsController.h"
#include "StageLayersMgr.h"
#include "StageDataMgr.h"
#include <unordered_map>
#include "RedPackageModule.h"
#include "DataConfig.h"
#include "GameClock.h"
USING_NS_CC;

class StagePetNode;
class UiLayout;
class StageStateOwner;
class NoTouchLayer;
class ActionRunner;

class StageUiLayer
    : public cocos2d::CCNode
	, public IStageDataView
	, public IStarsControlView
	, public IStageLayer
	, public cocos2d::CCTouchDelegate
{
public:
	StageUiLayer();
    ~StageUiLayer(void);

	static StageUiLayer *create();
    virtual bool init();
	void initTopUi();
	void initPets();
	void initBottomUi();
	void showTargetPanel();
	std::unordered_map<int, cocos2d::CCPoint> getPetViewsInfo();
	void gameOverSpreadStars(const GoodsData &data, const LogicGrid &targetGrid, std::function<void()> callback);
public:
	virtual void onInitStarsDone();
	virtual void onTargetPanelDone();
	virtual void onStepsChanged();
	virtual void onScoreChanged();
	virtual void onCoinsChanged();
	virtual void onShowGameResult(int isWon);
	virtual void onStarErased(cocos2d::CCPoint pos, int starType, int color);
	virtual void onHighLightPets(const std::vector<int> &petIds);
	virtual void onToNormalState();
	virtual void onRunOutSteps();
	virtual void onGameWin();
	virtual void onScoreDouble();
	virtual void onTouchEnable(bool canTouch);
	virtual void onExplodeGrid(const LogicGrid &grid);
	virtual void onOneRoundBegan();
	virtual void onOneRoundEnd();
	virtual void onGuideViewRemoved();
	virtual void onLinkErase(int num);
	virtual void onPetSpreadStar(int petId, const vector<LogicGrid> &grids, std::function<void()> callback);
	virtual void onRedPackageBomb();
public:
	void showChangeColorPanel(int myColor, const LogicGrid &grid);
private:
	virtual void onEnter();
	virtual void onExit();
	void initGameStart();
	void handlePetClicked(int petId);
	void handlePropsItemClicked(int type);
	void showGameOverHint();
	void showPetsSkillPanel();
	void showPropsGuide();
	void refreshRedPackage();
	void tryPets();

	void onPauseBtnClicked(CCObject *pSender);
	virtual bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	void playExplosionAction(cocos2d::CCPoint pos);
	void removeExplosionAnimation(cocos2d::extension::CCArmature *, cocos2d::extension::MovementEventType, const char *);
	void bubbleFinished(cocos2d::extension::CCArmature *, cocos2d::extension::MovementEventType, const char *);
	const cocos2d::CCPoint &getStarPos(const LogicGrid &grid);

	void onTick(float dt);
private:

private:
	static const int kZorder_Pet = 10;
	static const int kZorder_Dialog = 20;
	UiLayout *m_bottomUi;
	UiLayout *m_topUi;
	StageStateOwner *m_stateOwner;
	cocos2d::CCProgressTimer *m_scoreProgress;
	std::unordered_map<int , StagePetNode *>m_petViews;
	RedPackageModule m_redPackage;
	NoTouchLayer *m_noTouchLayer;
	unordered_map<int, cocos2d::CCPoint> m_starsPos; //用于保存stars的位置
	TickClock m_clock;
	ActionRunner *m_runner;
};
#endif // __PANELLLAYER_H__