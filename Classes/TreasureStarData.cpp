#include "TreasureStarData.h"
#include "TreasureStarView.h"
#include "cocos2d.h"
#include "TreasureStageMacro.h"
#include "TreasureStageModel.h"
USING_NS_CC;
using namespace std;

TreasureStarData::TreasureStarData(const LogicGrid &grid, int color)
:m_view(NULL)
, m_grid(grid)
, m_color(color)
{

}

TreasureStarData::~TreasureStarData()
{
	removeView();
}

void TreasureStarData::removeView()
{
	if (m_view)
	{
		m_view->remove();
		m_view = NULL;
	}
}

void TreasureStarData::doExplosion(int score)
{
	if (m_view)
	{
		m_view->doExplosion(score);
		m_view = NULL;
	}
}

vector<LogicGrid> TreasureStarData::getNeighboursGrid()
{
	int offset[4][2] = {{ 0, 1 }, { 1, 0 }, { 0, -1 }, { -1, 0 }};
	vector<LogicGrid> grids;
	for (int i = 0; i < 4; ++i)
	{
		LogicGrid newGrid;
		newGrid.x = m_grid.x + offset[i][0];
		newGrid.y = m_grid.y + offset[i][1];
		if (isValidGrid(newGrid))
		{
			grids.push_back(newGrid);
		}
	}
	return grids;
}

bool TreasureStarData::isValidGrid(const LogicGrid &grid)
{
	bool isValidX = grid.x >= 0 && grid.x < TREASURE_COLUMNS_NUM;
	bool isValidY = grid.y >= 0 && grid.y < TREASURE_ROWS_NUM;
	return isValidX && isValidY;
}

void TreasureStarData::handleTouch()
{
	TreasureStageModel::theModel()->handleTouch(this);
}

void TreasureStarData::moveBy(const LogicGrid &offset)
{
	if (m_view)
	{
		m_view->doMove(offset);
	}
	m_grid = m_grid + offset;
}