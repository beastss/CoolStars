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
	return !node || !node->canNotMove() || node->canBeRemoved(); //�ո� �����ƶ��� ������
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
	//�Ź��������ĳ����İ˸�����ƫ����
	//�ϣ����ϣ������£��£����£��ң�����  �����ñ����� 0 ~ 7
	int offset[8][2] = { { 0, 1 }, { 1, 1 }, { 1, 0 }, { 1, -1 }, { 0, -1 }, { -1, -1 }, { -1, 0 }, { -1, 1 } };
	auto configs = DataManagerSelf->getBkGridConfig();

	for (int side = kGridtop; side <= kGridLeft; ++side)//side ��ǰ��������������ĸ�����Ķ������α�
	{
		for (auto data : configs)
		{
			vector<LogicGrid> existGrids;
			vector<LogicGrid> notExistGrids;

			for (auto direction : data.exist)//directionΪ���ñ��ϰ˸������id 0~7
			{
				auto tempGrid = m_grid;
				direction = (direction + side * 2) % 8;//���ڷ����Ӧ�İ˸������ƫ��2
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
				//������Դ�Ƿ����Ϸ����г��ֵ���������������������ͨ����ת����
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