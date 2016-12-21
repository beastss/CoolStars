#include "TreasureStageModel.h"
#include "TreasureStageDataMgr.h"
#include "CommonMacros.h"
#include "CommonUtil.h"
#include "TreasureStageMacro.h"
#include "TreasureStageState.h"
#include "TreasureStageNotifier.h"
#include "ActionRunner.h"
#include <algorithm>
#include "StageDataMgr.h"
#include "SoundMgr.h"
using namespace std;

TreasureStageModel::TreasureStageModel()
{
	m_runner = ActionRunner::create();
	m_runner->retain();
}

TreasureStageModel::~TreasureStageModel()
{
	m_runner->clear();
	m_runner->release();
}

TreasureStageModel *TreasureStageModel::theModel()
{
	static TreasureStageModel model;
	return &model;
}

void TreasureStageModel::startStage()
{
	reset();
	TreasureStageState::theState()->reset();
}

void TreasureStageModel::endStage()
{
	reset();
	TreasureStageState::theState()->reset();
}

void TreasureStageModel::nextStage(bool skipCurStage)
{
	reset();
	TreasureStageState::theState()->nextStage(skipCurStage);
	TreasureStageNotifier::theNotifier()->onNewStage();
}

void TreasureStageModel::startNewStage()
{
	reset();
	TreasureStageState::theState()->reset();
	TreasureStageNotifier::theNotifier()->onNewStage();
}

void TreasureStageModel::initData()
{
	int color = CommonUtil::getRandomValue(kColorRed, kColorPurple);
	LogicGrid grid(0, 0);
	genNeighbourStars(grid, color);
}

TreasureStarData *TreasureStageModel::getStarDataByGrid(const LogicGrid &grid)
{
	for (size_t i = 0; i < m_stars.size(); ++i)
	{
		if (m_stars[i]->getGrid() == grid)
		{
			return m_stars[i];
		}
	}
	return NULL;
}

void TreasureStageModel::genNeighbourStars(const LogicGrid &grid, int color)
{
	if (getStarDataByGrid(grid)) return;
	TreasureStarData *pNewData = new TreasureStarData(grid, color);
	m_stars.push_back(pNewData);

	auto neighboursGrids = pNewData->getNeighboursGrid();
	for (size_t i = 0; i < neighboursGrids.size(); ++i)
	{
		genNeighbourStars(neighboursGrids[i], getNeighbourColor(color));
	}
}

int TreasureStageModel::getNeighbourColor(int myColor)
{
	vector<int> otherColors;
	for (int color = kColorRed; color <= kColorPurple; ++color)
	{
		if (color != myColor)
		{
			otherColors.push_back(color);
		}
	}
	int curStage = TreasureStageState::theState()->getCurStage();
	auto config = TreasureStageDataMgrSelf->getStageData(curStage);
	const float factor = config.factor;
	
	vector<float> percents;
	percents.push_back(factor);
	percents.push_back(100 - factor);
	bool isSameColor = CommonUtil::getResultByPercent(percents) == 0;
	if (isSameColor)
	{
		return myColor;
	}
	else
	{
		int index = CommonUtil::getRandomValue(0, COLOR_AMOUNT - 2);//剩余的颜色
		return otherColors[index];
	}
}

void TreasureStageModel::reset()
{
	for (size_t i = 0; i < m_stars.size(); ++i)
	{
		delete m_stars[i];
	}
	m_stars.clear();

}

void TreasureStageModel::removeStar(TreasureStarData *star)
{
	delete star;
	m_stars.erase(find(m_stars.begin(), m_stars.end(), star));
}

void TreasureStageModel::handleTouch(TreasureStarData *data)
{
	vector<TreasureStarData *>link;
	getSameColorNeighbours(link, data);
	int eraseNum = link.size();
	if (eraseNum >= TREASURE_STAR_MIN_LINK_NUM)
	{
		SoundMgr::theMgr()->playEffect(kEffectStarErase);
		int score = TreasureStageDataMgrSelf->getEraseBonusData(eraseNum).score / eraseNum;
		for (auto iter = link.begin(); iter != link.end(); ++iter)
		{
			(*iter)->doExplosion(score);
			removeStar(*iter);
		}

		moveDown();
		moveLeft();
		TreasureStageState::theState()->eraseStars(eraseNum);
		if (checkGameOver())
		{
			overCurStage();
		}
	}	
}

void TreasureStageModel::moveDown()
{
	//sort 从下到上 从左到右
	vector<TreasureStarData *> temp;
	for (int row = 0; row < TREASURE_ROWS_NUM; ++row)
	{
		for (int col = 0; col < TREASURE_COLUMNS_NUM; ++col)
		{
			auto grid = LogicGrid(col, row);
			auto data = getStarDataByGrid(grid);
			if (data)
			{
				temp.push_back(data);
			}
		}
	}

	for (size_t i = 0; i < temp.size(); ++i)
	{
		auto grid = temp[i]->getGrid();
		LogicGrid offset;
		for (int row = grid.y - 1; row >= 0; --row)
		{
			LogicGrid downGrid = grid;
			downGrid.y = row;
			if (!getStarDataByGrid(downGrid))
			{
				offset.y--;
			}
		}
		temp[i]->moveBy(offset);
	}
}

void TreasureStageModel::moveLeft()
{
	//sort  从左到右 从下到上
	vector<TreasureStarData *> temp;
	vector<int> moveColoumsToLeft;
	int leftEmptyColumns = 0;
	for (int col = 0; col < TREASURE_COLUMNS_NUM; ++col)
	{
		bool isEmptyCol = true;
		for (int row = 0; row < TREASURE_ROWS_NUM; ++row)
		{
			auto grid = LogicGrid(col, row);
			auto data = getStarDataByGrid(grid);
			if (data)
			{
				temp.push_back(data);
				isEmptyCol = false;
			}
		}
		moveColoumsToLeft.push_back(leftEmptyColumns);
		if (isEmptyCol)
		{
			leftEmptyColumns++;
		}
	}

	for (size_t i = 0; i < temp.size(); ++i)
	{
		LogicGrid grid = temp[i]->getGrid();
		LogicGrid offset;
		offset.x = -moveColoumsToLeft[grid.x];
		temp[i]->moveBy(offset);
	}
}

void TreasureStageModel::getSameColorNeighbours(vector<TreasureStarData *> &link, TreasureStarData *center)
{
	link.push_back(center);
	
	auto heighboursGrids = center->getNeighboursGrid();
	for (size_t i = 0; i < heighboursGrids.size(); ++i)
	{
		auto heighbour = getStarDataByGrid(heighboursGrids[i]);
		bool alreadyIn = find(link.begin(), link.end(), heighbour) != link.end();
		if (heighbour && !alreadyIn && heighbour->getColor() == center->getColor())
		{
			getSameColorNeighbours(link, heighbour);
		}
	}
}

bool TreasureStageModel::checkGameOver()
{
	//bool isWon = TreasureStageState::theState()->isWon();
	//if (isWon) return true;

	//效率不高
	for (size_t i = 0; i < m_stars.size(); ++i)
	{
		vector<TreasureStarData *>link;
		getSameColorNeighbours(link, m_stars[i]);
		if (link.size() >= TREASURE_STAR_MIN_LINK_NUM)
		{
			return false;
		}
	}
	return true;
}

void TreasureStageModel::overCurStage()
{
	auto stateModel = TreasureStageState::theState();
	bool isWon = stateModel->isWon();
	int curStage = stateModel->getCurStage();

	m_runner->clear();
	stateModel->block();
	TreasureStageNotifier::theNotifier()->onStageOverStart(curStage, isWon);
	
	//胜利后消除剩余星星
	if (isWon)
	{
		//sort  从左到右 从上到下
		vector<TreasureStarData *> temp;
		vector<int> moveColoumsToLeft;
		for (int col = 0; col < TREASURE_COLUMNS_NUM; ++col)
		{
			for (int row = TREASURE_ROWS_NUM - 1; row >= 0; --row)
			{
				auto grid = LogicGrid(col, row);
				auto data = getStarDataByGrid(grid);
				if (data)
				{
					temp.push_back(data);
				}
			}
		}
		for (size_t i = 0; i < temp.size(); ++i)
		{
			m_runner->queueAction(DelayAction::withDelay(0.1f));
			m_runner->queueAction(CallFuncAction::withFunctor([=]()
			{
				temp[i]->doExplosion(0);
				removeStar(temp[i]);
				TreasureStageState::theState()->eraseStars(1);
			}));
		}
		m_runner->queueAction(DelayAction::withDelay(1.0f));
	}

	m_runner->queueAction(CallFuncAction::withFunctor([=]()
	{
		TreasureStageNotifier::theNotifier()->onStageOverEnd(curStage, isWon);
	}));
}

bool TreasureStageModel::isOpening()
{
	return true;

	int stage = StageDataMgr::theMgr()->getCurStage();
	return stage >= 3;//hard code
}