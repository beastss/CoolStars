#include "StarBkGrid.h"
#include "UiLayout.h"
#include "StarsController.h"
USING_NS_CC;
using namespace std;

StarBkGrid *StarBkGrid::create(const GridRound &round)
{
	auto grid = new StarBkGrid(round);
	grid->init();
	grid->autorelease();
	return grid;
}

bool StarBkGrid::init()
{
	m_layout = UiLayout::create("layout/star_bk_grid.xml");
	addChild(m_layout);

	int offset[4][2] = { { 0, 1 },{ 1, 0 }, { 0, -1 }, { -1, 0 }};
	int offset[4][2] = {{ 1, 1 }, { 1, -1 }, { -1, -1 }, { -1, 1 }};
	StarNode *node = StarsController::theModel()->getStarNode(LogicGrid(col, row));

	int inIds[] = { 20, 21, 22, 23 };
	int outIds[] = { 10, 11, 12, 13 };
	for (int id : inIds)
	{
		m_layout->getChildById(id)->setVisible(false);
	}
	for (int id : outIds)
	{
		m_layout->getChildById(id)->setVisible(false);
	}

	setContentSize(m_layout->getContentSize());
	return true;
}