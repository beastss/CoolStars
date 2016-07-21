#include "StarsController.h"
#include "DataManager.h"
#include "SqliteHelper.h"
#include "StageSavingHelper.h"
#include "CommonMacros.h"
#include "StageDataMgr.h"
#include "UserInfo.h"
#include <algorithm>
#include "StageOperator.h"
#include "GuideMgr.h"
#include "GameBackEndState.h"
USING_NS_CC;
using namespace std;
StarsController::StarsController()
{

}

StarsController::~StarsController()
{
}

StarsController *StarsController::theModel()
{
	static StarsController model;
	return &model;
}


void StarsController::initStarsData()
{
	//返回的数据是保存行的
	vector<vector<StageStarInfo>> stageVec;
	
	StageDataMgr::theMgr()->getStageStars(stageVec);
	
	for (int row = 0; row < ROWS_SIZE; ++row)
	{
		for (int col = 0; col < COlUMNS_SIZE; ++col)
		{
			StarAttr attr;
			attr.type = stageVec[col][row].starType;
			attr.color = stageVec[col][row].color;
			attr.grid = LogicGrid(row, ROWS_SIZE - col - 1);
			m_starNodes.push_back(StarNode::createNodeFatory(attr));
		}
	}

	StageDataMgr::theMgr()->doSave();
}

void StarsController::resetStage(int gameType)
{
	//reset nodes
	for (auto iter = m_starNodes.begin(); iter != m_starNodes.end(); ++iter)
	{
		delete(*iter);
	}
	StageDataMgr::theMgr()->reset(gameType);
	m_starNodes.clear();
	m_target.init();
	m_starsLoader.init();
}

StarNode *StarsController::getStarNode(const LogicGrid &grid)
{
	auto iter = find_if(m_starNodes.begin(), m_starNodes.end(), [=](StarNode *node)->bool
	{
		const LogicGrid temp = node->getAttr().grid;
		return grid == temp;
	});

	return iter != m_starNodes.end() ? *iter : NULL;
}

void StarsController::moveStars()
{
	sort(m_starNodes.begin(), m_starNodes.end(), [=](StarNode *node1, StarNode *node2)->bool
	{
		auto grid1 = node1->getAttr().grid;
		auto grid2 = node2->getAttr().grid;
		switch (StageDataMgr::theMgr()->getCurDirection())
		{
		case kMoveUp:
			return grid1.y > grid2.y;
		case kMoveDown:
			return grid1.y < grid2.y;
		case kMoveLeft :
			return grid1.x < grid2.x;
		case kMoveRight:
			return grid1.x > grid2.x;
		default:
			return false;
		}
	});
	for (auto iter = m_starNodes.begin(); iter != m_starNodes.end(); ++iter)
	{
		moveStar(*iter);
	}
}

bool StarsController::isGridEmpty(const LogicGrid &grid)
{
	auto iter = find_if(m_starNodes.begin(), m_starNodes.end(), [=](StarNode* node)->bool
	{
		return node->getAttr().grid == grid;
	});
	return iter == m_starNodes.end();
}

void StarsController::moveStar(StarNode *node)
{
	int direction = StageDataMgr::theMgr()->getCurDirection();
    auto curGrid = node->getAttr().grid;
    auto targetGrid = curGrid;
    switch (direction)
    {
        case kMoveUp :
            for (int i = curGrid.y + 1; i < ROWS_SIZE; ++i)
            {
				LogicGrid temp(curGrid.x, i);
				if (isGridEmpty(temp))
                {
                    targetGrid.y++;
                }
            }
            break;
        case kMoveDown:
            for (int i = 0; i < curGrid.y; ++i)
            {
				LogicGrid temp(curGrid.x, i);
				if (isGridEmpty(temp))
                {
                    targetGrid.y--;
                }
            }
            break;
        case kMoveLeft:
            for (int i = 0; i < curGrid.x; i++)
            {
				LogicGrid temp(i, curGrid.y);
				if (isGridEmpty(temp))
                {
                    targetGrid.x--;
                }
            }
            break;	
        case kMoveRight:
            for (int i = curGrid.x + 1; i < COlUMNS_SIZE; ++i)
            {
				LogicGrid temp(i, curGrid.y);
				if (isGridEmpty(temp))
                {
                    targetGrid.x++;
                    
                }
            }
            break;
    }
	if (targetGrid != node->getAttr().grid)
	{
		node->moveTo(targetGrid);
		node->setLogicGrid(targetGrid);
	}
}

void StarsController::removeStarNode(StarNode *node)
{
	auto iter = find(m_starNodes.begin(), m_starNodes.end(), node);
	if (iter != m_starNodes.end())
	{
		auto attr = (*iter)->getAttr();
		m_target.starErased(attr.type, attr.color);

		delete *iter;
		m_starNodes.erase(iter);
		NOTIFY_VIEWS(onStarRemove);
	}
}

void StarsController::gameOver(bool isWon)
{
	GuideMgr::theMgr()->pauseGuide(false);
	GameBackEndState::theModel()->recordStageEnd(isWon);

	int value = UserInfo::theInfo()->getRuneStone();
	UserInfo::theInfo()->setRuneStone(value + 1);
	if (isWon)
	{
		StageDataMgr::theMgr()->toNextStage();
	}
	NOTIFY_VIEWS(onShowGameResult, isWon);
}

void StarsController::genNewStars()
{
	//移动方向： 上下左右
	int moveDirection[4][2] = { { 0, 1 }, { 0, -1 }, { -1, 0 }, { 1, 0 } };
	int emptyGridX[COlUMNS_SIZE] = {0};	//值为x的一列上空格的个数
	int emptyGridY[ROWS_SIZE] = {0};		//值为y的一行上空格的个数
	for (int x = 0; x < COlUMNS_SIZE; ++x)
	{
		for (int y = 0; y < ROWS_SIZE; ++y)
		{

			auto iter = find_if(m_starNodes.begin(), m_starNodes.end(), [=](StarNode *node)->bool
			{
				auto grid = node->getAttr().grid;
				return  grid.x == x && grid.y == y;
			});

			if (iter == m_starNodes.end())
			{
				emptyGridX[x]++;
				emptyGridY[y]++;
			}
		}
	}

	int kExtraGridOffset = max(COlUMNS_SIZE, ROWS_SIZE);//新创建的星星初始 在四方扩大kExtraGridOffset个的地方
	vector<LogicGrid> newGrid;
	switch (StageDataMgr::theMgr()->getCurDirection())
	{
	case kMoveUp:
		for (int i = 0; i < COlUMNS_SIZE; ++i)
		{
			for (int j = 0; j < emptyGridX[i]; ++j)
			{
				LogicGrid grid(i, 0 - kExtraGridOffset + j);
				newGrid.push_back(grid);
			}
		}
		break;
	case kMoveDown:
		for (int i = 0; i < COlUMNS_SIZE; ++i)
		{
			for (int j = 0; j < emptyGridX[i]; ++j)
			{
				LogicGrid grid(i, ROWS_SIZE  - 1 + kExtraGridOffset - j);
				newGrid.push_back(grid);
			}
		}
		break;
	case kMoveLeft:
		for (int i = 0; i < ROWS_SIZE; ++i)
		{
			for (int j = 0; j < emptyGridY[i]; ++j)
			{
				LogicGrid grid(COlUMNS_SIZE - 1 + kExtraGridOffset - j ,i);
				newGrid.push_back(grid);
			}
		}
		break;
	case kMoveRight:
		for (int i = 0; i < ROWS_SIZE; ++i)
		{
			for (int j = 0; j < emptyGridY[i]; ++j)
			{
				LogicGrid grid(0 - kExtraGridOffset + j, i);
				newGrid.push_back(grid);
			}
		}
		break;
	default:
		break;
	}
	for (size_t i = 0; i < newGrid.size(); ++i)
	{
		auto attr = m_starsLoader.genNewStars(newGrid[i]);
		StarNode *node = StarNode::createNodeFatory(attr);
		m_starNodes.push_back(node);
		NOTIFY_VIEWS(onCreateNewStar, node);
	}
	moveStars();
	onOneRoundBegan();
}

void StarsController::addView(IStarsControlView *view)
{
	auto iter = find(m_views.begin(), m_views.end(), view);
	if (iter == m_views.end())
	{
		m_views.push_back(view);
	}
}

void StarsController::removeView(IStarsControlView *view)
{
	auto iter = find(m_views.begin(), m_views.end(), view);
	if (iter != m_views.end())
	{
		m_views.erase(iter);
	}
}

void StarsController::moveOneStep(bool addStep)
{
	if (addStep)
	{
		StageDataMgr::theMgr()->addStep();
	}
	onOneRoundEnd();
	checkGameOver();
}

void StarsController::checkGameOver()
{
	if (m_target.isGameOver())
	{
		GuideMgr::theMgr()->finishGuide();
		GuideMgr::theMgr()->pauseGuide(true);
		if (m_target.isReachTarget())
		{
			NOTIFY_VIEWS(onGameWin);
		}
		else
		{
			NOTIFY_VIEWS(onRunOutSteps);
		}
	}
}

bool StarsController::noStarsToErase()
{
	for (size_t i = 0; i < m_starNodes.size(); ++i)
	{
		auto node = m_starNodes[i];
		if (node->canClickErase())
		{
			return false;
		}
	}
	return true;
}

void StarsController::reOrderStars(int times)
{
	const int maxReorderTimes = 3;
	if (times >= maxReorderTimes)
	{
		gameOver(false);
		return;
	}
	StageOperator::theOperator()->reOrderStars();
	if (noStarsToErase())
	{
		reOrderStars(times + 1);
	}
}

//玩家移动一步，新回合开始
void StarsController::onOneRoundBegan()
{
	m_starsBehavior.onOneRoundBegin();
	StageDataMgr::theMgr()->newRound();
	
	//游戏中没有可消除的星星 则重排
	if (noStarsToErase())
	{
		reOrderStars(0);
	}

	NOTIFY_VIEWS(onOneRoundBegan);
}

void StarsController::onOneRoundEnd()
{
	m_starsBehavior.onOneRoundEnd();
	m_starsLoader.onOneRoundEnd();
	NOTIFY_VIEWS(onOneRoundEnd);
}

void StarsController::addScore(int value)
{
	StageDataMgr::theMgr()->addCurScore(value);
}

void StarsController::replaceStar(const StarAttr &attr)
{
	auto grid = attr.grid;
	StarNode *node = getStarNode(grid);
	if (!node) return;

	node->doRemove(false);
	genStar(attr);
}

void StarsController::genStar(const StarAttr &attr)
{
	auto node = StarNode::createNodeFatory(attr);
	m_starNodes.push_back(node);
	NOTIFY_VIEWS(onCreateNewStar, node);
}

int StarsController::getStageAmount()
{
	return DataManagerSelf->getSystemConfig().stageAmount;
}

void StarsController::loadDesignatedStar(int starType, int color, int rounds)
{
	m_starsLoader.designateStar(starType, color, rounds);
	NOTIFY_VIEWS(onDesignatedStarChanged, starType, color, rounds);
}

void StarsController::onDesignatedStarChanged(int starType, int color, int rounds)
{
	NOTIFY_VIEWS(onDesignatedStarChanged, starType, color, rounds);
}