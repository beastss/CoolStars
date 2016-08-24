#include "StarsEraseModule.h"
#include "StarsController.h"
#include "ActionRunner.h"
#include "SoundMgr.h"
#include "StageLayersMgr.h"
#include <algorithm>
using namespace std;
USING_NS_CC;

StarsEraseModule::StarsEraseModule()
{
	reset();
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
	StageLayersMgr::theMgr()->eraseStarsEnd();//点击星星后，该回合结束

	auto node = StarsController::theModel()->getStarNode(grid);
	if (node && node->canClickErase())
	{
		std::vector<StarNode *> connectedNodes;
		node->getConnectedStars(node, connectedNodes);
		size_t count = connectedNodes.size();
		sort(connectedNodes.begin(), connectedNodes.end(), [=](StarNode *node1, StarNode *node2)
		{
			auto grid1 = node1->getAttr().grid;
			auto grid2 = node2->getAttr().grid;
			if (grid1.x == grid2.x) return grid1.y < grid2.y;
			return grid1.x < grid2.x;
		});
		if (count >= CONNECT_COUNT)
		{
			StageLayersMgr::theMgr()->eraseStarsStart();
			for (size_t j = 0; j < count; ++j)
			{
				m_runner->queueAction(CallFuncAction::withFunctor([=]()
				{
					auto node = connectedNodes[j];
					node->removeNeighbours();
					StageLayersMgr::theMgr()->explodeGrid(node->getAttr().grid);
					node->doRemove();
					//eraseStarBegan();
					SoundMgr::theMgr()->playEffect(kEffectStarErase);
				}));
				m_runner->queueAction(DelayAction::withDelay(0.1f));
			}
		}
	}
}

void StarsEraseModule::scaleErase(const LogicGrid &center, int xRadius, int yRadius)
{
	if (m_runners.empty())
	{
		StageLayersMgr::theMgr()->eraseStarsStart();
	}

	auto runner = new ScaleEarseRunner(center, xRadius, yRadius);
	m_runners.push_back(runner);
}


void StarsEraseModule::onScaleEraseDone(ScaleEarseRunner *runner)
{
	auto iter = find(m_runners.begin(), m_runners.end(), runner);
	if (iter != m_runners.end())
	{
		delete *iter;
		m_runners.erase(iter);
	}

	if (m_runners.empty())
	{
		StarsController::theModel()->moveOneStep(false);
		StarsController::theModel()->genNewStars();
		m_runner->queueAction(DelayAction::withDelay(0.5f));
		m_runner->queueAction(CallFuncAction::withFunctor([=]()
		{
			StageLayersMgr::theMgr()->eraseStarsEnd();
			StarsController::theModel()->onOneRoundBegan();
		}));
	}
}

void StarsEraseModule::randomErase(int num)
{
	vector<LogicGrid> grids;
	for (size_t i = 0; i < ROWS_SIZE; ++i)
	{
		for (int j = 0; j < COlUMNS_SIZE; ++j)
		{
			grids.push_back(LogicGrid(j, i));
		}
	}

	auto targetGrids = getRandomGrids(grids, num);
	
	bool hasBomb = false;
	StageLayersMgr::theMgr()->eraseStarsStart();

	for (size_t i = 0; i < targetGrids.size(); ++i)
	{
		auto node = StarsController::theModel()->getStarNode(targetGrids[i]);
		if (node && node->canBeRemoved())
		{
			if (node->getAttr().type == kBomb)
			{
				hasBomb = true;
				scaleErase(targetGrids[i], COlUMNS_SIZE, ROWS_SIZE);
			}
			else
			{
				node->doRemove();
			}
		}
	}

	if (!hasBomb)
	{
		StageLayersMgr::theMgr()->eraseStarsEnd();
		StarsController::theModel()->moveOneStep(false);
		StarsController::theModel()->genNewStars();
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

void StarsEraseModule::reset()
{
	for (size_t i = 0; i < m_runners.size(); ++i)
	{
		delete m_runners[i];
	}
	m_runners.clear();
}

void StarsEraseModule::eraseStarBegan()
{
	m_starsNotErased++;
}

void StarsEraseModule::eraseStarEnd()
{
	m_starsNotErased--;
	if (m_starsNotErased <= 0)
	{
		m_starsNotErased = 0;
		StarsController::theModel()->genNewStars();
		StarsController::theModel()->moveOneStep();
	}
}

//////////////////////////////////////////////////////////////////
ScaleEarseRunner::ScaleEarseRunner(const LogicGrid &center, int xRadius, int yRadius)
: m_center(center)
, m_xRadius(xRadius)
, m_yRadius(yRadius)
{
	m_runner = ActionRunner::create();
	m_runner->retain();
	runErase();
}

ScaleEarseRunner::~ScaleEarseRunner()
{
	m_runner->clear();
	m_runner->release();
}

//口字型消除
void ScaleEarseRunner::runErase()
{
	int rounds = max(m_xRadius, m_yRadius) + 1;
	for (int i = 0; i < rounds; ++i)
	{
		bool isCenter = i == 0;
		m_runner->queueAction(CallFuncAction::withFunctor([=]()
		{
			//获取一环的方格（方形）
			auto roundGrids = getSquareRoundGrids(m_center, i);
			vector<LogicGrid> grids;
			int minX = m_center.x - m_xRadius;
			int maxX = m_center.x + m_xRadius;
			int minY = m_center.y - m_yRadius;
			int maxY = m_center.y + m_yRadius;
			//将方格裁剪成需要的矩形
			for (size_t i = 0; i < roundGrids.size(); ++i)
			{
				auto grid = roundGrids[i];
				if (grid.x >= minX && grid.x <= maxX && grid.y >= minY && grid.y <= maxY)
				{
					grids.push_back(grid);
				}
			}
			eraseStars(grids, isCenter);
		}));
		m_runner->queueAction(DelayAction::withDelay(0.4f));
	}

	m_runner->queueAction(CallFuncAction::withFunctor(bind(&StarsEraseModule::onScaleEraseDone, StarsEraseModule::theModel(), this)));
}

bool ScaleEarseRunner::isBomb(const LogicGrid &grid)
{
	auto node = StarsController::theModel()->getStarNode(grid);
	return node && node->getAttr().type == kBomb;
}

void ScaleEarseRunner::eraseStars(vector<LogicGrid> grids, bool isCenter)
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
			if (!isCenter && isBomb(grids[i]))
			{
				StarsEraseModule::theModel()->scaleErase(grids[i], COlUMNS_SIZE, ROWS_SIZE);//全屏消除
			}

			node->doRemove();
		}
		StageLayersMgr::theMgr()->explodeGrid(grids[i]);
	}
}
