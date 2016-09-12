#ifndef __STARBKGRID_H__
#define __STARBKGRID_H__ 

#include "cocos2d.h"
#include "LogicGridUtil.h"

class UiLayout;

struct GridRound //方格为中心八个方向
{
	bool top;
	bool upperRight;
	bool right;
	bool lowerRight;
	bool bottom;
	bool lowerLeft;
	bool left;
	bool upperLeft;
};
class StarBkGrid
	: public cocos2d::CCNode
{
public:
	static StarBkGrid *create(const LogicGrid &grid);
private:
	StarBkGrid(const LogicGrid &grid) :m_grid(grid){}
	virtual bool init();
private:
	LogicGrid m_grid;
	UiLayout *m_layout;

private:
	/*
	enum GridDirection
	{
		kGridtop = 1,
		kGridUpperRight,
		kGridRight,
		kGridLowerRight,
		kGridBottom,
		kGridLowerLeft,
		kGridLeft,
		kGridUpperLeft,
	};
	*/
	enum GridDirection
	{
		kGridtop,
		kGridRight,
		kGridBottom,
		kGridLeft,
	};
};
#endif