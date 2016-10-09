#include "StageLayersMgr.h"
#include <algorithm>
USING_NS_CC;
using namespace std;

StageLayersMgr *StageLayersMgr::theMgr()
{
	static StageLayersMgr mgr;
	return &mgr;
}

void StageLayersMgr::addLayer(IStageLayer *layer)
{
	auto iter = find(m_layers.begin(), m_layers.end(), layer);
	if (iter == m_layers.end())
	{
		m_layers.push_back(layer);
	}
}

void StageLayersMgr::removeLayer(IStageLayer *layer)
{
	auto iter = find(m_layers.begin(), m_layers.end(), layer);
	if (iter != m_layers.end())
	{
		m_layers.erase(iter);
	}
}

void StageLayersMgr::initStarDone()
{
	NOTIFY_LAYERSS(onInitStarsDone);
}

void StageLayersMgr::targetPanelDone()
{
	NOTIFY_LAYERSS(onTargetPanelDone);
}

void StageLayersMgr::highLightStars(const std::vector<LogicGrid> &grids, int radiusX, int radiusY)
{
	NOTIFY_LAYERSS(onHighLightStars, grids, radiusX, radiusY);
}

void StageLayersMgr::highLightPets(const std::vector<int> &petIds)
{
	NOTIFY_LAYERSS(onHighLightPets, petIds);
}

void StageLayersMgr::toNormalState()
{
	NOTIFY_LAYERSS(onToNormalState);
}

void StageLayersMgr::starErased(cocos2d::CCPoint pos, int starType, int color)
{
	NOTIFY_LAYERSS(onStarErased, pos, starType, color);
}

void StageLayersMgr::doubleScore()
{
	NOTIFY_LAYERSS(onScoreDouble);
}

void StageLayersMgr::touchEnable(bool canTouch)
{
	NOTIFY_LAYERSS(onTouchEnable, canTouch);
}

void StageLayersMgr::explodeGrid(const LogicGrid &grid)
{
	NOTIFY_LAYERSS(onExplodeGrid, grid);
}

void StageLayersMgr::removeGuideView()
{
	NOTIFY_LAYERSS(onGuideViewRemoved);
}

void StageLayersMgr::linkErase(int num)
{
	NOTIFY_LAYERSS(onLinkErase, num);
}

void StageLayersMgr::petSpreadStar(int petId, const vector<LogicGrid> &grids, function<void()> callback)
{
	NOTIFY_LAYERSS(onPetSpreadStar, petId, grids, callback);
}