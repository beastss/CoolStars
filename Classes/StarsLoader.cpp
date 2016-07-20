#include "StarsLoader.h"
#include "StarsController.h"
#include "CommonUtil.h"
#include "StageDataMgr.h"
#include <algorithm>
using namespace std;
using namespace CommonUtil;
USING_NS_CC;

StarAttr StarsLoader::genNewStars(const LogicGrid &grid)
{
	switch (m_mode)
	{
	case kStarLoadRandomByConfig:
		return genStarRandomByConfig(grid);
	case kStarLoadDesignatedStar:
		return genDesignatedStar(grid);
	case kStarLoadBounceBall:
		return genBouceBall(grid);
	default:
		assert(false && "no this type");
		return StarAttr();
	}
}

StarAttr StarsLoader::genDesignatedStar(const LogicGrid &grid)
{
	StarAttr attr;
	attr.grid = grid;
	attr.type = m_designatedStar.starType;
	attr.color = m_designatedStar.color;
	return attr;
}

StarAttr StarsLoader::genBouceBall(const LogicGrid &grid)
{
	StarAttr attr;
	attr.grid = grid;
	attr.type = kColorStar;
	attr.color = kColorRandom;
	
	return attr;
}

StarAttr StarsLoader::genStarRandomByConfig(const LogicGrid &grid)
{
	StarAttr attr;
	attr.grid = grid;
	attr.type = kColorStar;
	attr.color = kColorRandom;

	int curScore = StageDataMgr::theMgr()->getCurScore();
	int popFrontNum = 0;
	for (auto iter = m_loaderDatas.begin(); iter != m_loaderDatas.end(); ++iter)
	{
		auto data = m_loaderDatas.front();
		//当前得分大于data分数才开始算百分比出现
		if (data.score > curScore)
		{
			break;
		}
		popFrontNum++;
		if (isSelected(data.percent))
		{
			attr.type = data.starType;
			CCLOG("type = %d", data.starType);
			break;
		}
	}
	for (int i = 0; i < popFrontNum; ++i)
	{
		m_loaderDatas.pop_front();
	}

	return attr;
	
}

void StarsLoader::init()
{
	int curStage = StageDataMgr::theMgr()->getCurStage();
	auto configs = DataManagerSelf->getStarsLoaderConfig(curStage);
	
	m_loaderDatas.clear();
	for (size_t i = 0; i < configs.size(); ++i)
	{
		auto config = configs[i];
		for (int j = 0; j < config.num; ++j)
		{
			StarsLoaderData data;
			data.starType = config.starType;
			data.percent = config.percent;
			data.score = config.score;
			m_loaderDatas.push_back(data);
		}
	}
	sort(m_loaderDatas.begin(), m_loaderDatas.end(), [=](StarsLoaderData data1, StarsLoaderData data2)->bool
	{
		return data1.score < data2.score;
	});
}

void StarsLoader::onOneRoundEnd()
{
	if (m_mode == kStarLoadDesignatedStar)
	{
		if (--m_designatedStarRounds < 0)
		{
			m_mode = kStarLoadRandomByConfig;
		}
	}
	StarsController::theModel()->onDesignatedStarChanged(m_designatedStar.starType, m_designatedStar.color, m_designatedStarRounds);
}

void StarsLoader::designateStar(int starType, int color, int rounds)
{
	m_designatedStar.starType = starType;
	m_designatedStar.color = color;
	m_designatedStarRounds = rounds;
	if (rounds > 0)
	{
		m_mode = kStarLoadDesignatedStar;
	}
}