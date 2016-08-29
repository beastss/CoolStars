#include "StarsMover.h"
#include "ConfData.h"
#include "StarsController.h"
#include "StarNode.h"
#include "StageDataMgr.h"
USING_NS_CC;
using namespace std;

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

void StarsMover::drop(StarNode *node)
{
	m_curNode = node;
	if (node->isStill()) return;
	if (dropDown()) return;
	if (dropLeftDown()) return;
	if (dropRightDown()) return;
}

//直下移动
bool StarsMover::dropDown()
{
	auto attr = m_curNode->getAttr();
	if (isBottom()) return false;
	auto nextGrid = attr.grid;
	auto offset = getDownOffset();
	nextGrid.x += offset.x;
	nextGrid.y += offset.y;

	if (!StarsController::theModel()->getStarNode(nextGrid))
	{
		m_curNode->moveTo(nextGrid);
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

	if (node && node->isStill())
	{
		auto nextGrid = leftGrid + getDownOffset();
		if (isValidGrid(nextGrid) && !StarsController::theModel()->getStarNode(nextGrid))
		{
			m_curNode->moveTo(nextGrid);
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

	if (node && node->isStill())
	{
		auto nextGrid = rightGrid + getDownOffset();
		if (isValidGrid(nextGrid) && !StarsController::theModel()->getStarNode(nextGrid))
		{
			m_curNode->moveTo(nextGrid);
			drop(m_curNode);
			return true;
		}
	}
	return false;
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
		StarNode *node = NULL;
		int yOffset = 0;
		while (!(node = StarsController::theModel()->getStarNode(LogicGrid(col, ROWS_SIZE - 1))))
		{
			StarsController::theModel()->genNextStar(LogicGrid(col, ROWS_SIZE + yOffset));
			yOffset++;
			drop(node);
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
