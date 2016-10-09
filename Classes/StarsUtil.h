#ifndef __STARSUTIL_H__
#define __STARSUTIL_H__ 

#include "cocos2d.h"
namespace StarsUtil
{
	struct StarsRange
	{
		int x;
		int y;
		int cols;
		int rows;
		StarsRange(int ix = 0, int iy = 0, int ic = 0, int ir = 0) :x(ix), y(iy), cols(ic), rows(ir){}
	};
	bool isStaticStar(int starType);
	bool hasMoveStarsInColumn(int col);
	bool hasMoveStarsInRow(int row);

	const StarsRange usedRange();
}
#endif