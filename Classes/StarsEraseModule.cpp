#include "StarsEraseModule.h"
#include "StarsController.h"
#include "ActionRunner.h"
#include "SoundMgr.h"
#include "StageLayersMgr.h"
using namespace std;
USING_NS_CC;

StarsEraseModule::StarsEraseModule()
{
	reset();
}

StarsEraseModule::~StarsEraseModule()
{
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
			StageLayersMgr::theMgr()->eraseStarsStart();
			for (size_t j = 0; j < count; ++j)
			{
				auto node = connectedNodes[j];
				node->removeNeighbours();
				StageLayersMgr::theMgr()->explodeGrid(node->getAttr().grid);
				node->doRemove();
			}
			StageLayersMgr::theMgr()->eraseStarsEnd();
			StarsController::theModel()->moveOneStep();
			StarsController::theModel()->genNewStars();
			SoundMgr::theMgr()->playEffect(kEffectStarErase);
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
		StageLayersMgr::theMgr()->eraseStarsEnd();
		StarsController::theModel()->moveOneStep(false);
		StarsController::theModel()->genNewStars();
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




