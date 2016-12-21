#include "LogicGridUtil.h"
#include "CommonUtil.h"
#include "ConfData.h"
#include <algorithm>

using namespace std;
using namespace CommonUtil;

bool isValidGrid(const LogicGrid &grid)
{
	bool isValidX = grid.x >= 0 && grid.x < COlUMNS_SIZE;
	bool isValidY = grid.y >= 0 && grid.y < ROWS_SIZE;
	return isValidX && isValidY;
}

//口字方形
vector<LogicGrid> getSquareRoundGrids(const LogicGrid& grid, int distance)
{
	vector<LogicGrid> grids;
	if (distance <= 0)
	{
		grids.push_back(grid);
	}
	{
		auto outSideSquare = getSquareGrids(grid, distance);
		auto inSideSqure = getSquareGrids(grid, distance - 1);
		for (auto iter = outSideSquare.begin(); iter != outSideSquare.end(); ++iter)
		{
			if (find(inSideSqure.begin(), inSideSqure.end(), *iter) == inSideSqure.end())
			{
				grids.push_back(*iter);
			}
		}
	}
	return grids;
}

//实心方形
vector<LogicGrid> getSquareGrids(const LogicGrid& grid, int distance)
{
	vector<LogicGrid> grids;
	int minX = grid.x - distance;
	int maxX = grid.x + distance;
	int minY = grid.y - distance;
	int maxY = grid.y + distance;
	for (int i = minX; i <= maxX; ++i)
	{
		for (int j = minY; j <= maxY; ++j)
		{
			auto temp = LogicGrid(i, j);
			if (isValidGrid(temp))
			{
				grids.push_back(temp);
			}
		}
	}

	return grids;
}

std::vector<LogicGrid> getRectGrids(const LogicGrid& grid, int xRadius, int yRadius)
{
	vector<LogicGrid> grids;
	int minX = grid.x - xRadius;
	int maxX = grid.x + xRadius;
	int minY = grid.y - yRadius;
	int maxY = grid.y + yRadius;
	for (int i = minX; i <= maxX; ++i)
	{
		for (int j = minY; j <= maxY; ++j)
		{
			auto temp = LogicGrid(i, j);
			if (isValidGrid(temp))
			{
				grids.push_back(temp);
			}
		}
	}

	return grids;
}

vector<LogicGrid> getStarsOnRow(const LogicGrid &grid, int num)
{
	vector<LogicGrid> grids;
	int beginX = grid.x - num / 2;
	int endX = beginX + num - 1;
	for (int i = beginX; i <= endX; ++i)
	{
		grids.push_back(LogicGrid(i, grid.y));
	}
	return grids;
}

vector<LogicGrid> getStarsOnColumn(const LogicGrid &grid, int num)
{
	vector<LogicGrid> grids;
	int beginY = grid.y- num / 2;
	int endY = beginY + num - 1;
	for (int i = beginY; i <= endY; ++i)
	{
		grids.push_back(LogicGrid(grid.x, i));
	}
	return grids;
}

vector<LogicGrid> getRandomGrids(const vector<LogicGrid> &range, int num)
{
	auto indexs = buildRandomSequence(range.size());
	int size = min((int)(indexs.size()), num);

	vector<LogicGrid> grids;
	for (int i = 0; i < size; ++i)
	{
		int index = indexs[i];
		grids.push_back(range[index]);
	}
	return grids;
}
