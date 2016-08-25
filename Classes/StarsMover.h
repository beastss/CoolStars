#ifndef __STARSMOVER_H__
#define __STARSMOVER_H__
#include "cocos2d.h"
#include "LogicGridUtil.h"
class StarsMover
{
public:
	void moveStars();
	void move(const LogicGrid grid);
};

class MoveStarsUp
	:public StarsMover
{

};

class MoveStarsDown
	:public StarsMover
{

};
#endif