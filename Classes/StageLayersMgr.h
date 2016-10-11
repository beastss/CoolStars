#ifndef __STAGELAYERSMGR_H__
#define __STAGELAYERSMGR_H__
#include "cocos2d.h"
#include "LogicGridUtil.h"
#include "StarNode.h"
#include <functional>
struct IStageLayer
{
	virtual void onInitStarsDone(){}
	virtual void onTargetPanelDone(){}
	virtual void onHighLightStars(const std::vector<LogicGrid> &grids, int radiusX, int radiusY){}
	virtual void onToNormalState(){}
	virtual void onStarErased(cocos2d::CCPoint pos, int starType, int color){}
	virtual void onLinkErase(int num){}
	virtual void onHighLightPets(const std::vector<int> &petIds){}
	virtual void onScoreDouble(){}
	virtual void onTouchEnable(bool canTouch){}
	virtual void onEraseStarsStart(){}
	virtual void onEraseStarsEnd(){}
	virtual void onExplodeGrid(const LogicGrid &grid){}
	virtual void onGuideViewRemoved(int guideId){}
	virtual void onPetSpreadStar(int petId, const vector<LogicGrid> &grids, std::function<void()> callback){}
	virtual void onPetChangeStep(int petId, int newSteps){}
	virtual void onAddPetEnergy(int fromPetId, int toPetId, int energy){}
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
	void explodeGrid(const LogicGrid &grid);
	void removeGuideView(int guideId);
	void linkErase(int num);
	void petSpreadStar(int petId, const vector<LogicGrid> &grids, function<void()> callback);
	void petChangeStep(int petId, int newSteps);
	void addPetEnergy(int fromPetId, int toPetId, int energy);
private:
	StageLayersMgr(){}
private:
	std::vector<IStageLayer *> m_layers;

};
#endif