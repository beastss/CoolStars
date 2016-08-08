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

class StarsEraseModule
{
public:
	~StarsEraseModule();
	static StarsEraseModule *theModel();
	void handleClick(const LogicGrid &grid);
	void scaleErase(const LogicGrid &center, int xRadius, int yRadius);
	void randomErase(int num);
	void removeStar(const LogicGrid &grid);//直接消除 没有爆炸
	void reset();
	void onScaleEraseDone(ScaleEarseRunner *runner);
private:
	StarsEraseModule();
private:
	std::vector<ScaleEarseRunner *>m_runners;
};

#endif