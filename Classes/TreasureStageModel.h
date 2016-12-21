#ifndef _TREASURE_STAGE_MODEL_H
#define _TREASURE_STAGE_MODEL_H
#include "cocos2d.h"
#include "TreasureStarData.h"

class ActionRunner;
class TreasureStageModel
{
public:
	static TreasureStageModel *theModel(); 
	~TreasureStageModel();
	void startStage();
	void endStage();
	void nextStage(bool skipCurStage = false);//skipCurStage直接跳过当前关卡，分数直接置为当前关目标分数
	void startNewStage();//从第一关开始

	void initData();
	const std::vector<TreasureStarData *> &getStarsData(){ return m_stars; }
	TreasureStarData *getStarDataByGrid(const LogicGrid &grid);
	bool isOpening();
public:
	void handleTouch(TreasureStarData *data);
private:
	TreasureStageModel();
	void reset();
	void genNeighbourStars(const LogicGrid &grid, int color);
	int getNeighbourColor(int myColor);
	void getSameColorNeighbours(std::vector<TreasureStarData *> &link, TreasureStarData *center);
	void moveDown();
	void moveLeft();
	bool checkGameOver();
	void overCurStage();
	void removeStar(TreasureStarData *star);
private:
	std::vector<TreasureStarData *> m_stars;
	ActionRunner *m_runner;
};
#endif