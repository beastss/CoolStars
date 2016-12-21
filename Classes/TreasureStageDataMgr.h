#ifndef _TREASURESTAGECONFIG_H_
#define _TREASURESTAGECONFIG_H_

#include "cocos2d.h"
#define TreasureStageDataMgrSelf TreasureStageDataMgr::theSelf()

struct TreasureStageConfig
{
	int id;
	int factor;
	int targeSocre;
	int food;
	int diamond;
};

struct TreasureStarConfig
{
	int id;
	std::string starPath;
	std::string explosionPath;
};

struct TreasureEraseBonusConfig
{
	int id;
	int num;
	int score;
	std::string tipsPath;
};

class TreasureStageDataMgr
{
public:
	static TreasureStageDataMgr *theSelf();
	void loadData();

	TreasureStageConfig getStageData(int stage);
	int getStageNum(){ return m_stageData.size(); }
	TreasureStarConfig getStarData(int color);
	TreasureEraseBonusConfig getEraseBonusData(int num);

	int costToSkipCurStage();
private:
	TreasureStageDataMgr();
	void loadStageConfig();
	void loadStarConfig();
	void loadEraseBonusConfig();
private:
	std::vector<TreasureStageConfig> m_stageData;
	std::vector<TreasureStarConfig> m_starData;
	std::vector<TreasureEraseBonusConfig> m_earseBonusData;
};
#endif