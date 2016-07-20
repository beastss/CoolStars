#ifndef __STARSLAYER_H__ 
#define __STARSLAYER_H__
#include "cocos2d.h"
#include "StarViewNode.h"
#include "StarsController.h"
#include "StageLayersMgr.h"
USING_NS_CC;
class StarUtil;
class StageStateOwner;
class UiLayout;
class StarsLayer
	: public cocos2d::CCNode
	, public cocos2d::CCTouchDelegate
	, public IStarsControlView
	, public IStageLayer
{
public:
	static StarsLayer *create();
	void removeStar(StarViewNode *node);
private:
	StarsLayer();
	~StarsLayer();
	virtual bool init();

	void initStars();
	void starInitDone();
	StarViewNode *getClickedStar(CCPoint pos);
	virtual bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);

	CCPoint getPosByGrid(LogicGrid grid);
	StarViewNode *createStarByGrid(const LogicGrid &grid);

	virtual void onCreateNewStar(StarNode *node);
	virtual void onToNormalState();
	virtual void onEnter();
	virtual void onExit();
	void addClippingNode();
	void addBkGrids();
private:
	StarUtil *m_pStarUtil;
	std::vector<StarViewNode *> m_starsSprite;
	cocos2d::CCClippingNode *m_clippingNode;
	StageStateOwner *m_stateOwner;
	UiLayout *m_layout;
};
#endif 