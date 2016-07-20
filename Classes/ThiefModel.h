#ifndef __THIEFMODEL_H__
#define __THIEFMODEL_H__
#include "cocos2d.h"

enum ThiefPanelType
{
	kThiefEmpty,
	kThiefPreStagePanel,
	kThiefPetPanel,
	kThiefLotteryPanel,
	kThiefRankingPanel,
	kThiefShopPanel,
	kThiefPackagePanel,
};

class ThiefModel : public cocos2d::CCNode
{
public:
	static ThiefModel *theModel();

	virtual bool init();
	bool isThisPanel(int type);
	int whichPanel(){ return m_curPanelType; }
	void triggerThief();
private:
	virtual void update(float delta);
	ThiefModel();
private:
	int m_lastTriggerTime;
	int kTriggerDuration = 40 * 60;
	int m_curPanelType;
};
#endif