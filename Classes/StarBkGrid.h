#ifndef __STARBKGRID_H__
#define __STARBKGRID_H__ 

#include "cocos2d.h"
#include "LogicGridUtil.h"

class UiLayout;

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
	enum GridDirection
	{
		kGridtop,
		kGridRight,
		kGridBottom,
		kGridLeft,
	};
};
#endif