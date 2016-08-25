#ifndef __STARSERASEMODULE_H__
#define __STARSERASEMODULE_H__
 
#include "cocos2d.h"
#include "LogicGridUtil.h"

class ActionRunner;
class ScaleEarseRunner
{
public:
	ScaleEarseRunner(const LogicGrid &center, int xRadius, int yRadius);
	~ScaleEarseRunner();
private:
	void runErase();
	void eraseStars(std::vector<LogicGrid> grids, bool isCenter);
	bool isBomb(const LogicGrid &grid);
private:
	ActionRunner *m_runner;
	LogicGrid m_center;
	int m_xRadius;
	int m_yRadius;
};
//四种消除方式
//1.普通点击的连消除
//2.矩形消除
//3.列表消除
//4.//直接消除目标星星 没有爆炸

class StarsEraseModule
{
public:
	void linkErase(const LogicGrid &grid);
	void scaleErase(const LogicGrid &center, int xRadius, int yRadius);
	void listErase(const std::vector<LogicGrid> &grids);
	void removeStar(const LogicGrid &grid);
public:
	~StarsEraseModule();
	static StarsEraseModule *theModel();
	void reset();
	void onScaleEraseDone(ScaleEarseRunner *runner);
	void eraseStarBegan();
	void eraseStarEnd();
	void newRound();
private:
	StarsEraseModule();
private:
	std::vector<ScaleEarseRunner *>m_runners;
	ActionRunner *m_runner;
	int m_starsNotErased;
};

#endif