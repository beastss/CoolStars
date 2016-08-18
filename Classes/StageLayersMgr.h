#ifndef __STAGELAYERSMGR_H__
#define __STAGELAYERSMGR_H__
#include "cocos2d.h"
#include "LogicGridUtil.h"
struct IStageLayer
{
	virtual void onInitStarsDone(){}
	virtual void onTargetPanelDone(){}
	virtual void onHighLightStars(const std::vector<LogicGrid> &grids, int radiusX, int radiusY){}
	virtual void onToNormalState(){}
	virtual void onStarErased(cocos2d::CCPoint pos, int starType, int color){}
	virtual void onLinkErase(int num){}
	virtual void onHighLightPets(const std::vector<int> &petIds){}
	virtual void onScoreBouble(){}
	virtual void onTouchEnable(bool canTouch){}
	virtual void onEraseStarsStart(){}
	virtual void onEraseStarsEnd(){}
	virtual void onExplodeGrid(const LogicGrid &grid){}
	virtual void onGuideViewRemoved(){}
};

#define NOTIFY_LAYERSS(_FUNC_ ,...)						 \
for (size_t index = 0; index < m_layers.size(); ++index) \
{														 \
	m_layers[index]->_FUNC_(__VA_ARGS__);				 \
}

//这个类用于关卡中界面的通信
class StageLayersMgr
{
public:
	static StageLayersMgr *theMgr();
	~StageLayersMgr(){}
	
	void addLayer(IStageLayer *layer);
	void removeLayer(IStageLayer *layer);
public:
	void initStarDone();
	void targetPanelDone();
	void highLightStars(const std::vector<LogicGrid> &grids, int radiusX, int radiusY);
	void highLightPets(const std::vector<int> &petIds);
	void toNormalState();
	void starErased(cocos2d::CCPoint pos, int starType, int color);
	void doubleScore();
	void touchEnable(bool canTouch);
	void eraseStarsStart();
	void eraseStarsEnd();
	void explodeGrid(const LogicGrid &grid);
	void removeGuideView();
	void linkErase(int num);
private:
	StageLayersMgr(){}
private:
	std::vector<IStageLayer *> m_layers;

};
#endif