#ifndef _TREASURESTARVIEW_H_
#define _TREASURESTARVIEW_H_

#include "cocos2d.h"
#include "TouchNode.h"
#include "LogicGridUtil.h"
class TreasureStarData;

class TreasureStarView
	:public TouchNode
{
public:
	static TreasureStarView *create(TreasureStarData *model);
	void remove();
	void doExplosion(int score);
	void doMove(const LogicGrid &offset);
private:
	virtual bool onTouchBegan(cocos2d::CCPoint pt, bool isInside);
private:
	TreasureStarView(TreasureStarData *model);
	virtual bool init();
	TreasureStarData *m_model;
};
#endif