#include "StarsUtil.h"
#include "DataManager.h"
#include "CommonMacros.h"
#include "StageDataMgr.h"
USING_NS_CC;
using namespace std;
using namespace StarsUtil;

bool StarsUtil::isStaticStar(int starType)
{
	//��̬���� �������� Ҳ�����ƶ�
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

const StarsUtil::StarsRange StarsUtil::usedRange()
{
	StarsRange range;
	int emptyCols = 0;
	int emptyRows = 0;

	int index = 0;
	int endIndex = 0;
	//������
	for (index = 0; index < COlUMNS_SIZE; ++index)
	{
		if (!StarsUtil::hasMoveStarsInColumn(index)) emptyCols++;
		else break;
	}
	range.x = emptyCols;

	//���ҵ���
	endIndex = index;
	for (index = COlUMNS_SIZE - 1; index > endIndex; --index)
	{
		if (!StarsUtil::hasMoveStarsInColumn(index)) emptyCols++;
		else break;
	}

	//���µ���
	for (index = 0; index < ROWS_SIZE; ++index)
	{
		if (!StarsUtil::hasMoveStarsInRow(index)) emptyRows++;
		else break;
	}
	range.y = emptyRows;

	//���ϵ���
	endIndex = index;
	for (index = ROWS_SIZE - 1; index > endIndex; --index)
	{
		if (!StarsUtil::hasMoveStarsInRow(index)) emptyRows++;
		else break;
	}

	range.cols = COlUMNS_SIZE - emptyCols;
	range.rows = ROWS_SIZE - emptyRows;
	return range;
}