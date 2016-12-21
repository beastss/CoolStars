#ifndef __TREASURESTARSLAYER_H__ 
#define __TREASURESTARSLAYER_H__
#include "cocos2d.h"
#include "LogicGridUtil.h"
#include "TreasureStageNotifier.h"

USING_NS_CC;
class UiLayout;
class TreasureStarsLayer
	: public cocos2d::CCNode
	, public cocos2d::CCTouchDelegate
	, public ITreasureStageView
{
public:
	static TreasureStarsLayer *create();
	//void removeStar(StarViewNode *node);
	cocos2d::CCPoint getStartWorldPos();
	void refresh();
public:
	virtual void onStarExplore(int color, int score, cocos2d::CCPoint pt);
private:
	TreasureStarsLayer();
	~TreasureStarsLayer();
	virtual bool init();

	void initStars();
	void starInitDone();
	virtual bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);

	virtual void onEnter();
	virtual void onExit();
	void addClippingNode();
	void addBackground();
	cocos2d::CCPoint getLeftLowerPt();

private:
	cocos2d::CCPoint getPosByGrid(LogicGrid grid);
private:
	cocos2d::CCClippingNode *m_clippingNode;
	cocos2d::CCNode *m_bkLayer;
};
#endif 