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

void StageLayersMgr::highLightStars(int color)
{
	NOTIFY_LAYERSS(onHighLightStars, color);
}

void StageLayersMgr::highLightPets(const std::vector<int> &petIds)
{
	NOTIFY_LAYERSS(onHighLightPets, petIds);
}

void StageLayersMgr::highLightRectStars(int x, int y, int width, int height)
{
	NOTIFY_LAYERSS(onHighLightRectStars, x, y, width, height);
}

void StageLayersMgr::toNormalState()
{
	NOTIFY_LAYERSS(onToNormalState);
}

void StageLayersMgr::colorStarErased(cocos2d::CCPoint pos, int starType, int color)
{
	NOTIFY_LAYERSS(onNormalStarErased, pos, starType, color);
}

void StageLayersMgr::doubleScore()
{
	NOTIFY_LAYERSS(onScoreBouble);
}

void StageLayersMgr::touchEnable(bool canTouch)
{
	NOTIFY_LAYERSS(onTouchEnable, canTouch);
}

void StageLayersMgr::eraseStarsStart()
{
	NOTIFY_LAYERSS(onEraseStarsStart);
}

void StageLayersMgr::eraseStarsEnd()
{
	NOTIFY_LAYERSS(onEraseStarsEnd);
}