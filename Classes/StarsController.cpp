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
#include "StarsEraseModule.h"
#include "PetManager.h"
#include "StarsMover.h"
#include <xmemory>
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
			auto node = StarNode::createNodeFatory(attr);
			if (node)
			{
				m_starNodes.push_back(node);
			}
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
	StarsEraseModule::theModel()->reset();
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
	auto ret = iter != m_starNodes.end() ? *iter : NULL;
	return ret;
}

bool StarsController::isGridEmpty(const LogicGrid &grid)
{
	auto iter = find_if(m_starNodes.begin(), m_starNodes.end(), [=](StarNode* node)->bool
	{
		return node->getAttr().grid == grid;
	});
	return iter == m_starNodes.end();
}

void StarsController::removeStarNode(StarNode *node)
{
	auto iter = find(m_starNodes.begin(), m_starNodes.end(), node);
	if (iter != m_starNodes.end())
	{
		auto attr = (*iter)->getAttr();
		delete *iter;
		m_starNodes.erase(iter);
		NOTIFY_VIEWS(onStarRemove);
	}
}

void StarsController::gameOver(bool isWon)
{
	GuideMgr::theMgr()->pauseGuide(false);
	GameBackEndState::theModel()->recordStageEnd(isWon);
	StageDataMgr::theMgr()->recordFailState(isWon);

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
	auto_ptr<StarsMover> starsMover(StarsMover::fatory());
	starsMover->moveStars();
	starsMover->genStars();
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
}

bool StarsController::checkGameOver()
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
		return true;
	}
	return false;
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

void StarsController::initOneRound()
{
	if (!checkGameOver())
	{
		preOneRound();
		StageDataMgr::theMgr()->newRound();
	}
}

void StarsController::preOneRound()
{
	if (!PetManager::petMgr()->usePetSkill())
	{
		startOneRound();
	}
}

//玩家移动一步，新回合开始
void StarsController::startOneRound()
{
	m_starsBehavior.onOneRoundBegin();
	//游戏中没有可消除的星星 则重排
	if (noStarsToErase())
	{
		reOrderStars(0);
	}

	//checkGameOver();
	NOTIFY_VIEWS(onOneRoundBegan);
}

void StarsController::endOneRound()
{
	moveOneStep();
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

StarNode *StarsController::genStar(const StarAttr &attr)
{
	auto node = StarNode::createNodeFatory(attr);
	m_starNodes.push_back(node);
	NOTIFY_VIEWS(onCreateNewStar, node);
	return node;
}

StarNode *StarsController::genNextStar(const LogicGrid &grid)
{
	auto attr = m_starsLoader.genNewStars(grid);
	return genStar(attr);
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

vector<LogicGrid> StarsController::getEmptyGrids()
{
	vector<LogicGrid> grids;
	for (int col = 0; col < COlUMNS_SIZE; ++col)
	{
		for (int row = 0; row < ROWS_SIZE; ++row)
		{
			auto grid = LogicGrid(col, row);
			if (!getStarNode(grid))
			{
				grids.push_back(grid);
			}
		}
	}
	return grids;
}