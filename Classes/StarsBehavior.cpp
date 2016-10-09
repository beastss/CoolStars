#include "StarsBehavior.h"
#include "StarsController.h"
#include <vector>
#include <algorithm>
using namespace std;
USING_NS_CC;

StarsBehavior::StarsBehavior()
{

}

void StarsBehavior::onOneRoundBegin()
{
	int amount = getLiveVineNodes().size();
	if (m_liveVineAmount > 0 && m_liveVineAmount == amount)
	{
		auto nodes = getLiveVineNodes();
		for (size_t i = 0; i < nodes.size(); ++i)
		{
			auto node = nodes[i];
			auto neighbours = node->getNeighbours();
			auto iter = find_if(neighbours.begin(), neighbours.end(), [=](StarNode *starNode)->bool
			{
				return starNode->getAttr().type == kColorStar;
			});
			if (iter != neighbours.end())
			{
				auto attr = (*iter)->getAttr();
				attr.type = kLiveVine;
				StarsController::theModel()->replaceStar(attr);
				break;
			}
		}
	}

	m_liveVineAmount = getLiveVineNodes().size();
}

vector<StarNode *> StarsBehavior::getLiveVineNodes()
{
	vector<StarNode *> liveVines;
	auto nodes = StarsController::theModel()->getStarNodes();
	for (size_t i = 0; i < nodes.size(); ++i)
	{
		if (nodes[i]->getAttr().type == kLiveVine)
		{
			liveVines.push_back(nodes[i]);
		}
	}
	return liveVines;
}