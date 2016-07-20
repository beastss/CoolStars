#ifndef __STARSERASEMODULE_H__
#define __STARSERASEMODULE_H__
 
#include "cocos2d.h"
#include "LogicGridUtil.h"

class ActionRunner;
class StarsEraseModule
{
public:
	~StarsEraseModule();
	static StarsEraseModule *theModel();
	void handleClick(const LogicGrid &grid);
	void scaleErase(const LogicGrid &center, int xRadius, int yRadius);
	void removeStar(const LogicGrid &grid);
private:
	StarsEraseModule();
	void eraseStars(std::vector<LogicGrid> grids);
	void runScaleErase(const LogicGrid &center, int xRadius, int yRadius);
	bool isBomb(const LogicGrid &grid);
private:
	ActionRunner *m_runner;

};
#endif