#ifndef __STARSLOADER_H__
#define __STARSLOADER_H__
#include "cocos2d.h"
#include "LogicGridUtil.h"
#include "StarNode.h"
#include "DataConfig.h"
#include <deque>

struct StarsLoaderData
{
	int starType;
	int score;
	int percent;
};

enum starLoadMode
{
	kStarLoadRandomByConfig, //根据配置表随机生成星星
	kStarLoadDesignatedStar, //生成指定的星星
	kStarLoadBounceBall,	 //糖丸子模式，梅花间隔
};

struct DesignatedStar
{
	int starType;
	int color;
};

class StarsLoader
{
public:
	StarsLoader() : m_mode(kStarLoadRandomByConfig), m_designatedStarRounds(0){}
	StarAttr genNewStars(const LogicGrid &grid);
	void init();
	void designateStar(int starType, int color, int rounds);
	void onOneRoundEnd();
	int getDesignatedStarRounds(){ return m_designatedStarRounds; }
private:
	StarAttr genStarRandomByConfig(const LogicGrid &grid);
	StarAttr genDesignatedStar(const LogicGrid &grid);
	StarAttr genBouceBall(const LogicGrid &grid);
private:
	std::deque<StarsLoaderData> m_loaderDatas;
	DesignatedStar m_designatedStar;
	int m_designatedStarRounds;
	int m_mode;
};
#endif