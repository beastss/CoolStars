#include "StarsEraseModule.h"
#include "StarsController.h"
#include "ActionRunner.h"
#include "SoundMgr.h"
using namespace std;
USING_NS_CC;

StarsEraseModule::StarsEraseModule()
{
	m_runner = ActionRunner::create();
	m_runner->retain();
}

StarsEraseModule::~StarsEraseModule()
{
	m_runner->clear();
	m_runner->release();
}

StarsEraseModule *StarsEraseModule::theModel()
{
	static StarsEraseModule model;
	return &model;
}

void StarsEraseModule::handleClick(const LogicGrid &grid)
{
	auto node = StarsController::theModel()->getStarNode(grid);
	if (node && node->canClickErase())
	{
		std::vector<StarNode *> connectedNodes;
		node->getConnectedStars(node, connectedNodes);
		size_t count = connectedNodes.size();
		if (count >= CONNECT_COUNT)
		{
			for (size_t j = 0; j < count; ++j)
			{
				auto node = connectedNodes[j];
				node->removeNeighbours();
				node->doRemove();
			}
			StarsController::theModel()->moveOneStep();
			StarsController::theModel()->genNewStars();
			SoundMgr::theMgr()->playEffect(kEffectStarErase);
		}
	}
}

bool StarsEraseModule::isBomb(const LogicGrid &grid)
{
	auto node = StarsController::theModel()->getStarNode(grid);
	return node->getAttr().type == kBomb;
}

void StarsEraseModule::scaleErase(const LogicGrid &center, int xRadius, int yRadius)
{
	if (isBomb(center))
	{
		runScaleErase(center, COlUMNS_SIZE, ROWS_SIZE);//全屏消除
		return;
	}
	
	auto grids = getRectGrids(center, xRadius, yRadius);
	for (size_t i = 0; i < grids.size(); ++i)
	{
		if (isBomb(grids[i]))
		{
			runScaleErase(grids[i], COlUMNS_SIZE, ROWS_SIZE);//全屏消除
			return;
		}
	}

	runScaleErase(center, xRadius, yRadius);
}

//口字型消除
void StarsEraseModule::runScaleErase(const LogicGrid &center, int xRadius, int yRadius)
{
	int rounds = max(xRadius, yRadius) + 1;
	for (int i = 0; i < rounds; ++i)
	{
		m_runner->queueAction(CallFuncAction::withFunctor([=]()
		{
			auto roundGrids = getSquareRoundGrids(center, i);
			vector<LogicGrid> grids;
			int minX = center.x - xRadius;
			int maxX = center.x + xRadius;
			int minY = center.y - yRadius;
			int maxY = center.y + yRadius;
			for (size_t i = 0; i < roundGrids.size(); ++i)
			{
				auto grid = roundGrids[i];
				if (grid.x >= minX && grid.x <= maxX && grid.y >= minY && grid.y <= maxY)
				{
					grids.push_back(grid);
				}
			}
			eraseStars(grids);
		}));
		m_runner->queueAction(DelayAction::withDelay(0.4f));
	}

	m_runner->queueAction(CallFuncAction::withFunctor([=]()
	{
		StarsController::theModel()->moveOneStep(false);
		StarsController::theModel()->genNewStars();
	}));
}

void StarsEraseModule::eraseStars(vector<LogicGrid> grids)
{
	if (!grids.empty())
	{
		SoundMgr::theMgr()->playEffect(kEffectMusicBomb);
	}

	for (size_t i = 0; i < grids.size(); ++i)
	{
		auto node = StarsController::theModel()->getStarNode(grids[i]);
		if (node && node->canBeRemoved())
		{
			node->doRemove();
		}
	}
}

void StarsEraseModule::removeStar(const LogicGrid &grid)
{
	auto node = StarsController::theModel()->getStarNode(grid);
	if (node )
	{
		node->doRemove(false);
	}
}

