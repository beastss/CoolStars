#ifndef __STARBKGRID_H__
#define __STARBKGRID_H__ 

#include "cocos2d.h"

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
	static StarBkGrid *create(const GridRound &round);
private:
	StarBkGrid(const GridRound &round):m_round(round){}
	virtual bool init();
private:
	GridRound m_round;
	UiLayout *m_layout;

private:
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
};
#endif