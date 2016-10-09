#include "StarsMover.h"
#include "ConfData.h"
#include "StarsController.h"
#include "StarNode.h"
#include "StageDataMgr.h"
#include <algorithm>
#include "StarsUtil.h"
USING_NS_CC;
using namespace std;
using namespace StarsUtil;

StarsMover *StarsMover::fatory()
{
	int direction = StageDataMgr::theMgr()->getCurDirection();
	switch (direction)
	{
	case kMoveUp:
		return new MoveStarsUp;
	case kMoveDown:
		return new MoveStarsDown;
	case kMoveLeft:
		return new MoveStarsLeft;
	case kMoveRight:
		return new MoveStarsRight;
	default:
		return NULL;
	}
	return NULL;
}

StarsMover::StarsMover()
{
	init();
}

void StarsMover::init()
{
	//返回的数据是保存行的
	vector<vector<StageStarInfo>> stageVec;
	StageDataMgr::theMgr()->getStageStars(stageVec);

	for (int col = 0; col < COlUMNS_SIZE; ++col)
	{
		for (int row = 0; row < ROWS_SIZE; ++row)
		{
			int starType = stageVec[ROWS_SIZE - 1 - row][col].starType;
			if (!isStaticStar(starType))
			{
				m_upMoveTopGrids[col] = LogicGrid(col, row);
				break;
			}
		}

		for (int row = ROWS_SIZE - 1; row >= 0; --row)
		{
			int starType = stageVec[ROWS_SIZE - 1 - row][col].starType;
			if (!isStaticStar(starType))
			{
				m_downMoveTopGrids[col] = LogicGrid(col, row);
				break;
			}
		}
	}


	for (int row = 0; row < ROWS_SIZE; ++row)
	{
		for (int col = COlUMNS_SIZE - 1; col >= 0; --col)
		{
			int starType = stageVec[ROWS_SIZE - 1 - row][col].starType;
			if (!isStaticStar(starType))
			{
				m_leftMoveTopGrids[row] = LogicGrid(col, row);
				break;
			}
		}

		for (int col = 0; col < COlUMNS_SIZE; ++col)
		{
			int starType = stageVec[ROWS_SIZE - 1 - row][col].starType;
			if (!isStaticStar(starType))
			{
				m_rightMoveTopGrids[row] = LogicGrid(col, row);
				break;
			}
		}
	}
}

void StarsMover::drop(StarNode *node)
{
	m_curNode = node;
	if (node->canNotMove()) return;
	if (dropDown()) return;
	if (dropLeftDown()) return;
	if (dropRightDown()) return;
}

//直下移动
bool StarsMover::dropDown()
{
	auto attr = m_curNode->getAttr();
	if (isBottom()) return false;//底部不掉落了
	auto nextGrid = attr.grid;
	auto offset = getDownOffset();
	nextGrid.x += offset.x;
	nextGrid.y += offset.y;

	if (!StarsController::theModel()->getStarNode(nextGrid) )
	{
		m_curNode->moveTo(nextGrid, m_direction);
		drop(m_curNode);
		return true;
	}
	return false;
}

//左下移动
bool StarsMover::dropLeftDown()
{
	auto leftGrid = m_curNode->getAttr().grid + getLeftOffset();
	auto node = StarsController::theModel()->getStarNode(leftGrid);

	if (node && node->canNotMove())
	{
		auto nextGrid = leftGrid + getDownOffset();
		if (isValidGrid(nextGrid) && !StarsController::theModel()->getStarNode(nextGrid))
		{
			m_curNode->moveTo(nextGrid, m_direction);
			drop(m_curNode);
			return true;
		}
	}
	return false;
}

//右下移动
bool StarsMover::dropRightDown()
{
	auto rightGrid = m_curNode->getAttr().grid + getRightOffset();
	auto node = StarsController::theModel()->getStarNode(rightGrid);

	if (node && node->canNotMove())
	{
		auto nextGrid = rightGrid + getDownOffset();
		if (isValidGrid(nextGrid) && !StarsController::theModel()->getStarNode(nextGrid))
		{
			m_curNode->moveTo(nextGrid, m_direction);
			drop(m_curNode);
			return true;
		}
	}
	return false;
}

///////////////////////////////////////////////////////////
void MoveStarsUp::moveStars()
{
	for (int row = ROWS_SIZE - 1; row >= 0; --row)
	{
		for (int col = COlUMNS_SIZE - 1; col >= 0; --col)
		{
			auto node = StarsController::theModel()->getStarNode(LogicGrid(col, row));
			if (node)
			{
				drop(node);
			}
		}
	}
}

void MoveStarsUp::genStars()
{
	for (int col = COlUMNS_SIZE - 1; col >= 0; --col)
	{
		if (m_upMoveTopGrids.find(col) == m_upMoveTopGrids.end()) continue;
		auto topGrid = m_upMoveTopGrids[col];
		StarNode *node = NULL;
		int offset = 1;
		while (!(node = StarsController::theModel()->getStarNode(topGrid)))
		{
			auto newStar = StarsController::theModel()->genNextStar(LogicGrid(col, topGrid.y - offset));
			offset++;
			newStar->moveTo(topGrid, m_direction);
			drop(newStar);
		}
	}
}

bool MoveStarsUp::isBottom()
{
	return m_curNode->getAttr().grid.y >= ROWS_SIZE - 1;
}

LogicGrid MoveStarsUp::getDownOffset()
{
	return LogicGrid(0, 1);
}

LogicGrid MoveStarsUp::getLeftOffset()
{
	return LogicGrid(1, 0);
}

LogicGrid MoveStarsUp::getRightOffset()
{
	return LogicGrid(-1, 0);
}


///////////////////////////////////////////////////////////
void MoveStarsDown::moveStars()
{
	for (int row = 0; row < ROWS_SIZE; ++row)
	{
		for (int col = 0; col < COlUMNS_SIZE; ++col)
		{
			auto node = StarsController::theModel()->getStarNode(LogicGrid(col, row));
			if (node)
			{
				drop(node);
			}
		}
	}
}

void MoveStarsDown::genStars()
{
	for (int col = 0; col < COlUMNS_SIZE; ++col)
	{
		if (m_downMoveTopGrids.find(col) == m_downMoveTopGrids.end()) continue;
		auto topGrid = m_downMoveTopGrids[col];
		StarNode *node = NULL;
		int offset = 1;
		while (!(node = StarsController::theModel()->getStarNode(topGrid)))
		{
			auto newStar = StarsController::theModel()->genNextStar(LogicGrid(col, topGrid.y + offset));
			offset++;
			newStar->moveTo(topGrid, m_direction);
			drop(newStar);
			int a = 0;
		}
	}
}

bool MoveStarsDown::isBottom()
{
	return m_curNode->getAttr().grid.y < 1;
}

LogicGrid MoveStarsDown::getDownOffset()
{
	return LogicGrid(0, -1);
}

LogicGrid MoveStarsDown::getLeftOffset()
{
	return LogicGrid(-1, 0);
}

LogicGrid MoveStarsDown::getRightOffset()
{
	return LogicGrid(1, 0);
}

///////////////////////////////////////////////////////////
void MoveStarsLeft::moveStars()
{
	for (int col = 0; col < COlUMNS_SIZE; ++col)
	{
		for (int row = ROWS_SIZE - 1; row >= 0; --row)
		{
			auto node = StarsController::theModel()->getStarNode(LogicGrid(col, row));
			if (node)
			{
				drop(node);
			}
		}
	}
}

void MoveStarsLeft::genStars()
{
	for (int row = ROWS_SIZE - 1; row >= 0; --row)
	{
		if (m_leftMoveTopGrids.find(row) == m_leftMoveTopGrids.end()) continue;
		auto topGrid = m_leftMoveTopGrids[row];
		StarNode *node = NULL;
		int offset = 1;
		while (!(node = StarsController::theModel()->getStarNode(topGrid)))
		{
			auto newStar = StarsController::theModel()->genNextStar(LogicGrid(topGrid.x + offset, row));
			offset++;
			newStar->moveTo(topGrid, m_direction);
			drop(newStar);
		}
	}
}

bool MoveStarsLeft::isBottom()
{
	return m_curNode->getAttr().grid.x < 1;
}

LogicGrid MoveStarsLeft::getDownOffset()
{
	return LogicGrid(-1, 0);
}

LogicGrid MoveStarsLeft::getLeftOffset()
{
	return LogicGrid(0, 1);
}

LogicGrid MoveStarsLeft::getRightOffset()
{
	return LogicGrid(0, -1);
}

///////////////////////////////////////////////////////////
void MoveStarsRight::moveStars()
{
	for (int col = COlUMNS_SIZE - 1; col >= 0; --col)
	{
		for (int row = 0; row < ROWS_SIZE; ++row)
		{
			auto node = StarsController::theModel()->getStarNode(LogicGrid(col, row));
			if (node)
			{
				drop(node);
			}
		}
	}
}

void MoveStarsRight::genStars()
{
	for (int row = 0; row < ROWS_SIZE; ++row)
	{
		if (m_rightMoveTopGrids.find(row) == m_rightMoveTopGrids.end()) continue;
		auto topGrid = m_rightMoveTopGrids[row];
		StarNode *node = NULL;
		int offset = 1;
		while (!(node = StarsController::theModel()->getStarNode(topGrid)))
		{
			auto newStar = StarsController::theModel()->genNextStar(LogicGrid(topGrid.x - offset, row));
			offset++;
			newStar->moveTo(topGrid, m_direction);
			drop(newStar);
		}
	}
}

bool MoveStarsRight::isBottom()
{
	return m_curNode->getAttr().grid.x >= COlUMNS_SIZE - 1;
}

LogicGrid MoveStarsRight::getDownOffset()
{
	return LogicGrid(1, 0);
}

LogicGrid MoveStarsRight::getLeftOffset()
{
	return LogicGrid(0, -1);
}

LogicGrid MoveStarsRight::getRightOffset()
{
	return LogicGrid(0, 1);
}

