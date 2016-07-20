#ifndef __STAGETARGET_H__
#define __STAGETARGET_H__
#include "cocos2d.h"
#include <unordered_map>
#include "StarNode.h"

struct EraseStarsData
{
	int starType;
	int color;
	int num;
};

class StageTarget
{
public:
	StageTarget();
	~StageTarget();
	
	void init();
	bool isGameOver();
	bool isReachTarget();
	void starErased(int starType, int color);
	std::vector<EraseStarsData> getEraseStarsLeft();
	int getTargetScore(){ return m_targetScore; }
	int getErasedStarAmount(int starType){ return m_record[starType]; }
private:
	void reset();

	bool isGetEnoughScore();
	bool isErasedEnoughStars();
	bool isReachTargetGrid();
private:
	enum winType
	{
		kEraseStars,//消除特定数目的星星
		kTargetGrid,//护送指定对象到指定位置
	};
private:
	int m_winType;
	int m_targetScore;
	std::vector<EraseStarsData> m_starsLeft;
	unordered_map<int, int> m_record;
	LogicGrid m_targetGrid;
};
#endif