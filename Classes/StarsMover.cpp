#include "StarsMover.h"
#include "ConfData.h"
USING_NS_CC;
using namespace std;

void StarsMover::moveStars()
{
	for (int col = 0; col < COlUMNS_SIZE; ++col)
	{
		move(LogicGrid(col, 0));
	}
}

void StarsMover::move(const LogicGrid grid)
{
	/*
	auto attr = m_starsLoader.genNewStars(newGrid[i]);
	StarNode *node = StarNode::createNodeFatory(attr);
	m_starNodes.push_back(node);
	NOTIFY_VIEWS(onCreateNewStar, node);
	*/

}