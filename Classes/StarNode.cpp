#include "StarNode.h"
#include "StarsController.h"
#include "StarViewNode.h"
#include "CommonUtil.h"
#include <algorithm>
#include "CommonMacros.h"
#include "StageOperator.h"

using namespace cocos2d;
using namespace std;

StarNode *StarNode::createNodeFatory(const StarAttr &attr)
{
	switch (attr.type)
	{
	case kColorStar:
		return new ColorStar(attr);
	case kBounceBall:
		return new BounceBallNode(attr);
	case kDeadVine:
		return new DeadVineNode(attr);
	case kLiveVine:
		return new LiveVineNode(attr);
	case kStump:
		return new StumpNode(attr);
	case kBomb:
		return new BombNode(attr);
	case kIron:
		return new IronNode(attr);
	case kDiamond:
		return new DiamondNode(attr);
	case kKey:
		return new KeyNode(attr);
	case kStone1:
		return new Stone1Node(attr);
	case kStone2:
		return new Stone2Node(attr);
	case kStone3:
		return new Stone3Node(attr);
	case kTerrain:
		return new TerrainNode(attr);
	default:
		assert("no this node type!");
		return NULL;
	}
}
StarNode::StarNode(const StarAttr &attr)
    : m_attr(attr)
	, m_view(NULL)
{
}

const StarsConfig &StarNode::getConfig()
{
	return DataManagerSelf->getStarsConfig(m_attr.type);
}

bool StarNode::canLink(int type, int color)
{ 
	auto linkTypes = getConfig().linkStarTypes;
	if (find(linkTypes.begin(), linkTypes.end(), type) != linkTypes.end())
	{
		return m_attr.color == color;
	}
	return false;
}

string StarNode::getResPath()
{
	return getConfig().resPath;
}

string StarNode::getExplosionPath()
{
	return getConfig().explosionRes;
}

string StarNode::getBornAnimationPath()
{
	return getConfig().bornAnimationRes;
}

bool StarNode::canClickErase()
{
	std::vector<StarNode *> connectedNodes;
	getConnectedStars(this, connectedNodes);
	size_t count = connectedNodes.size();
	for (size_t i = 0; i < count; ++i)
	{
		auto grid = connectedNodes[i]->getAttr().grid;
	}
	return count >= CONNECT_COUNT;
}

void StarNode::doRemove(bool addScore)
{
	if (m_view)
	{
		m_view->removeSelf(addScore);
	}
	if (addScore)
	{
		int myScore = getConfig().score;
		StarsController::theModel()->addScore(myScore);//爆炸则加分
		onRemove();
	}
	StarsController::theModel()->removeStarNode(this);
}

void StarNode::removeNeighbours()
{
	auto neightbours = getNeighbours();
	for (size_t i = 0; i < neightbours.size(); ++i)
	{
		auto node = neightbours[i];
		auto eraseTypes = node->getConfig().eraseTypes;
		if (find(eraseTypes.begin(), eraseTypes.end(), kNeightbourErase) != eraseTypes.end())
		{
			node->doRemove();
		}
	}
}

void StarNode::getConnectedStars(StarNode *node, std::vector<StarNode *> &connectedNodes)
{
     auto grid = node->getAttr().grid;
	if (find(connectedNodes.begin(), connectedNodes.end(), node) != connectedNodes.end())
	{
		return;
	}

	connectedNodes.push_back(node);
    
	auto neighbours = node->getLinkNeighbours();
	if (!neighbours.empty())
	{
        for (size_t i = 0; i < neighbours.size(); ++i)
        {
            auto temp = neighbours[i]->getAttr().grid;
        }
		for (size_t i = 0; i < neighbours.size(); ++i)
		{
			getConnectedStars(neighbours[i], connectedNodes);
		}
	}
}

vector<StarNode *> StarNode::getLinkNeighbours()
{
	vector<StarNode *> linkNodes;
	auto neighbours = getNeighbours();
	for (size_t i = 0; i < neighbours.size(); ++i)
	{

		StarNode *neighbour = neighbours[i];

		if (neighbour != NULL)
		{
			if (neighbour->canLink(m_attr.type, m_attr.color))
			{
				linkNodes.push_back(neighbour);
			}
		}
	}
	return linkNodes;
}

vector<StarNode *> StarNode::getNeighbours()
{
	vector<StarNode *> neighbours;
	LogicGrid grid = m_attr.grid;
	int arr[4][2] = { { 1, 0 }, { -1, 0 }, { 0, -1 }, { 0, 1 } };
	for (int i = 0; i < 4; ++i)
	{
        auto temp = LogicGrid(grid.x + arr[i][0], grid.y + arr[i][1]);
		StarNode *neighbour = StarsController::theModel()->getStarNode(temp);
		if (neighbour)
		{
			neighbours.push_back(neighbour);
		}
	}
	return neighbours;
}


void StarNode::moveTo(LogicGrid grid)
{
	if (m_view)
	{
		m_view->doMove(grid);
	}
}

/////////////////////////////////////////////////////////////////////////////////
ColorStar::ColorStar(const StarAttr &attr)
: StarNode(attr)
{ 
	if (m_attr.color == kColorRandom)
	{
		m_attr.color = CommonUtil::getRandomValue(kColorRed, kColorPurple);
	}
}

ColorStar::~ColorStar()
{
}

std::string ColorStar::getResPath()
{
	auto config = DataManagerSelf->getStarsColorConfig(m_attr.color);
	return config.colorStarRes;
}

std::string ColorStar::getExplosionPath()
{
	return "";

	//没有爆炸特效
	auto config = DataManagerSelf->getStarsColorConfig(m_attr.color);
	return config.colorExplosionRes;
}

/////////////////////////////////////////////////////////////////////////////////
BounceBallNode::BounceBallNode(const StarAttr &attr)
: StarNode(attr)
{
	if (m_attr.color == kColorRandom)
	{
		m_attr.color = CommonUtil::getRandomValue(kColorRed, kColorPurple);
	}
}

std::string BounceBallNode::getResPath()
{
	auto config = DataManagerSelf->getStarsColorConfig(m_attr.color);
	return config.bounceBallRes;
}

std::string BounceBallNode::getExplosionPath()
{
	return "";


	auto config = DataManagerSelf->getStarsColorConfig(m_attr.color);
	return config.bounceBallExplosionRes;
}
/////////////////////////////////////////////////////////////////////////////////
DeadVineNode::DeadVineNode(const StarAttr &attr)
: StarNode(attr)
{
	if (m_attr.color == kColorRandom)
	{
		m_attr.color = CommonUtil::getRandomValue(kColorRed, kColorPurple);
	}
}

void DeadVineNode::onRemove()
{
	auto attr = m_attr;
	attr.type = kColorStar;
	StarsController::theModel()->genStar(attr);
}

std::string DeadVineNode::getResPath()
{
	auto config = DataManagerSelf->getStarsColorConfig(m_attr.color);
	return config.colorStarRes;
}

std::string DeadVineNode::getExtraResPath()
{
	return getConfig().resPath;
}

/////////////////////////////////////////////////////////////////////////////////
LiveVineNode::LiveVineNode(const StarAttr &attr)
: StarNode(attr)
{
	if (m_attr.color == kColorRandom)
	{
		m_attr.color = CommonUtil::getRandomValue(kColorRed, kColorPurple);
	}
}

void LiveVineNode::onRemove()
{
	auto attr = m_attr;
	attr.type = kColorStar;
	StarsController::theModel()->genStar(attr);
}

std::string LiveVineNode::getResPath()
{
	auto config = DataManagerSelf->getStarsColorConfig(m_attr.color);
	return config.colorStarRes;
}

std::string LiveVineNode::getExtraResPath()
{
	return getConfig().resPath;
}

/////////////////////////////////////////////////////////////////////////////////
void Stone1Node::onRemove()
{
	auto attr = m_attr;
	attr.type = kStone2;
	StarsController::theModel()->genStar(attr);
}
/////////////////////////////////////////////////////////////////////////////////
void Stone2Node::onRemove()
{
	auto attr = m_attr;
	attr.type = kStone3;
	StarsController::theModel()->genStar(attr);
}
/////////////////////////////////////////////////////////////////////////////////
void BombNode::onRemove()
{
	return;
}
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
