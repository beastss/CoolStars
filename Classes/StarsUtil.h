#ifndef __STARSUTIL_H__
#define __STARSUTIL_H__ 

#include "cocos2d.h"
namespace StarsUtil
{
	bool isStaticStar(int starType);
	bool hasMoveStarsInColumn(int col);
	bool hasMoveStarsInRow(int row);
}
#endif