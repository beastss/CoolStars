#ifndef __LOGICGRID_H__
#define __LOGICGRID_H__
#include "cocos2d.h"

struct LogicGrid
{
	int x;
	int y;
	LogicGrid(int x = 0, int y = 0) : x(x), y(y){}
	//*
	bool operator == (const LogicGrid &grid)const
	{
		return grid.x == x && grid.y == y;
	}
	bool operator != (const LogicGrid &grid)const
	{
		return (x != grid.x || y != grid.y);
	}
};

bool isValidGrid(const LogicGrid &grid);
std::vector<LogicGrid> getSquareGrids(const LogicGrid& grid, int distance);
std::vector<LogicGrid> getRectGrids(const LogicGrid& grid, int xRadius, int yRadius);
std::vector<LogicGrid> getSquareRoundGrids(const LogicGrid& grid, int distance);
std::vector<LogicGrid> getStarsOnRow(const LogicGrid &grid, int num);
std::vector<LogicGrid> getStarsOnColumn(const LogicGrid &grid, int num);
std::vector<LogicGrid> getRandomGrids(const std::vector<LogicGrid> &range, int num);

#endif
