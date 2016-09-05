#include "StarsUtil.h"
#include "DataManager.h"
#include "CommonMacros.h"
#include "StageDataMgr.h"
USING_NS_CC;
using namespace std;

bool StarsUtil::isStaticStar(int starType)
{
	//静态星星 不能消除 也不能移动
	auto config = DataManagerSelf->getStarsConfig(starType);
	auto eraseTypes = config.eraseTypes;
	bool canErase = find(eraseTypes.begin(), eraseTypes.end(), kCanNotErase) == eraseTypes.end();
	bool canMove = config.canMove;
	return !canErase && !canMove;
}

bool StarsUtil::hasMoveStarsInColumn(int col)
{
	vector<vector<StageStarInfo>> stageVec;
	StageDataMgr::theMgr()->getStageStars(stageVec);
	for (size_t i = 0; i < stageVec.size(); ++i)
	{
		int starType = stageVec[i][col].starType;
		if (!isStaticStar(starType))
		{
			return true;
		}
	}
	return false;
}

bool StarsUtil::hasMoveStarsInRow(int row)
{
	vector<vector<StageStarInfo>> stageVec;
	StageDataMgr::theMgr()->getStageStars(stageVec);
	for (size_t i = 0; i < COlUMNS_SIZE; ++i)
	{
		int starType = stageVec[ROWS_SIZE - 1 - row][i].starType;
		if (!isStaticStar(starType))
		{
			return true;
		}
	}
	return false;
}