#include "LogicGridUtil.h"
#ifndef _TREASURESTARDATA_H_
#define _TREASURESTARDATA_H_

class TreasureStarView;

class TreasureStarData
{
public:
	TreasureStarData(const LogicGrid &grid, int color);
	~TreasureStarData();
	void bindView(TreasureStarView *view){ m_view = view; }
	void doErase();
	void doExplosion(int score);
	std::vector<LogicGrid> getNeighboursGrid();
	void handleTouch();
	void moveBy(const LogicGrid &offset);
public:
	int getColor(){ return m_color; }
	const LogicGrid &getGrid(){ return m_grid; }
private:
	bool isValidGrid(const LogicGrid &grid);
	void removeView();
private:
	TreasureStarView *m_view;
	LogicGrid m_grid;
	int m_color;
};
#endif