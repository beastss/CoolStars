#include "TreasureStageDataMgr.h"
#include "TreasureStarView.h"
#include "cocos2d.h"
#include "SqliteHelper.h"
USING_NS_CC;
using namespace std;

TreasureStageDataMgr *TreasureStageDataMgr::theSelf()
{
	static TreasureStageDataMgr config;
	return &config;
}

TreasureStageDataMgr::TreasureStageDataMgr()
{
}

void TreasureStageDataMgr::loadData()
{
	loadStageConfig();
	loadStarConfig();
	loadEraseBonusConfig();
}

void TreasureStageDataMgr::loadStageConfig()
{
	SqliteHelper sqlHelper(DB_TREASURE_STAGE);
	auto result = sqlHelper.readRecord("select * from stages");

	for (size_t i = 0; i < result.size(); ++i)
	{
		auto data = result[i];
		TreasureStageConfig config;
		config.id = atoi(data[0]);
		config.factor = atoi(data[1]);
		config.targeSocre = atoi(data[2]);
		config.food = atoi(data[3]);
		config.diamond = atoi(data[4]);
		m_stageData.push_back(config);
	}
}

TreasureStageConfig TreasureStageDataMgr::getStageData(int stage)
{
	assert(stage > 0);
	TreasureStageConfig config;
	int size = m_stageData.size();
	if (stage < size)
	{
		config = m_stageData[stage - 1];
	}
	else
	{
		config = m_stageData[size - 1];
		int lastOffset = m_stageData[size - 1].targeSocre - m_stageData[size - 2].targeSocre;
		config.targeSocre += lastOffset * (stage - size);
	}
	return config;
}

void TreasureStageDataMgr::loadStarConfig()
{
	SqliteHelper sqlHelper(DB_TREASURE_STAGE);
	auto result = sqlHelper.readRecord("select * from stars");

	for (size_t i = 0; i < result.size(); ++i)
	{
		auto data = result[i];
		TreasureStarConfig config;
		config.id = atoi(data[0]);
		config.starPath = data[1];
		config.explosionPath = data[2];
		m_starData.push_back(config);
	}
}

TreasureStarConfig TreasureStageDataMgr::getStarData(int color)
{
	assert(color >= 0 && color < m_starData.size());
	return m_starData[color];
}

void TreasureStageDataMgr::loadEraseBonusConfig()
{
	SqliteHelper sqlHelper(DB_TREASURE_STAGE);
	auto result = sqlHelper.readRecord("select * from erase_bonus");

	for (size_t i = 0; i < result.size(); ++i)
	{
		auto data = result[i];
		TreasureEraseBonusConfig config;
		config.id = atoi(data[0]);
		config.num = atoi(data[1]);
		config.score = atoi(data[2]);
		config.tipsPath = data[3];
		m_earseBonusData.push_back(config);
	}
}

TreasureEraseBonusConfig TreasureStageDataMgr::getEraseBonusData(int num)
{
	for (size_t i = 0; i < m_earseBonusData.size(); ++i)
	{
		auto data = m_earseBonusData[i];
		if (data.num == num)
		{
			return data;
		}
	}
	return m_earseBonusData.back();
}

int TreasureStageDataMgr::costToSkipCurStage()
{
	return 150;//hard code
}