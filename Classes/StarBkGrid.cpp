#include "StarBkGrid.h"
#include "UiLayout.h"
#include "StarsController.h"
#include "StageDataMgr.h"
USING_NS_CC;
using namespace std;

StarBkGrid *StarBkGrid::create(const LogicGrid &grid)
{
	auto node = new StarBkGrid(grid);
	node->init();
	node->autorelease();
	return node;
}

static bool containStar(const LogicGrid &grid)
{
	if (!isValidGrid(grid)) return false;

	StarNode *node = StarsController::theModel()->getStarNode(grid);
	return !node || !node->canNotMove() || node->canBeRemoved(); //空格， 可以移动， 可消除
}

static bool checkAllGridsExist(const vector<LogicGrid> grids)
{
	for (size_t i = 0; i < grids.size(); ++i)
	{
		if (!containStar(grids[i]))
		{
			return false;
		}
	}
	return true;
}


static bool checkAllGridsNotExist(const vector<LogicGrid> grids)
{
	for (size_t i = 0; i < grids.size(); ++i)
	{
		if (containStar(grids[i]))
		{
			return false;
		}
	}
	return true;
}

bool StarBkGrid::init()
{
	auto bk = CCSprite::create("stage/bk_grid/center.png");
	auto size = bk->getContentSize();
	setContentSize(size);
	bk->setPosition(ccpMult(size, 0.5f));
	if (!containStar(m_grid))
	{
		return true;
	}
	addChild(bk);
	//九宫格里中心出发的八个方向偏移量
	//上，左上，左，左下，下，右下，右，右上  在配置表上是 0 ~ 7
	int offset[8][2] = { { 0, 1 }, { 1, 1 }, { 1, 0 }, { 1, -1 }, { 0, -1 }, { -1, -1 }, { -1, 0 }, { -1, 1 } };
	auto configs = DataManagerSelf->getBkGridConfig();

	for (int side = kGridtop; side <= kGridLeft; ++side)//side 当前方格的上下左右四个方向的额外修饰边
	{
		for (auto data : configs)
		{
			vector<LogicGrid> existGrids;
			vector<LogicGrid> notExistGrids;

			for (auto direction : data.exist)//direction为配置表上八个方向的id 0~7
			{
				auto tempGrid = m_grid;
				direction = (direction + side * 2) % 8;//相邻方向对应的八个方向会偏移2
				tempGrid.x += offset[direction][0];
				tempGrid.y += offset[direction][1];
				existGrids.push_back(tempGrid);
			}

			for (auto direction : data.notExist)
			{
				auto tempGrid = m_grid;
				direction = (direction + side * 2) % 8;
				tempGrid.x += offset[direction][0];
				tempGrid.y += offset[direction][1];
				notExistGrids.push_back(tempGrid);
			}

			if (checkAllGridsExist(existGrids) && checkAllGridsNotExist(notExistGrids))
			{
				auto spr = CCSprite::create(data.path.c_str());
				auto sprSize = spr->getContentSize();
				//美术资源是方格上方所有出现的情况，其他三个方向的则通过旋转即可
				spr->setRotation(side * 90);
				addChild(spr);
				switch (side)
				{
				case kGridtop:
					spr->setPosition(ccp(size.width * 0.5f, size.height + sprSize.height * 0.5f));
					break;
				case kGridRight:
					spr->setPosition(ccp(size.width + sprSize.height * 0.5f, size.height * 0.5f));
					break;
				case kGridBottom:
					spr->setPosition(ccp(size.width * 0.5f, -sprSize.height * 0.5f));
					break;
				case kGridLeft:
					spr->setPosition(ccp(-sprSize.height * 0.5f, size.height * 0.5f));
					break;
				default:
					break;
				}
				char str[100] = { 0 };
				sprintf(str, "gird : x = %d, y = %d\n", m_grid.x, m_grid.y);
				break;
			}
		}
	
	}
	return true;
}