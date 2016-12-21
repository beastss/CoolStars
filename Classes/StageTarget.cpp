#include "StageTarget.h"
#include "StarsController.h"
#include "StageDataMgr.h"
#include "CommonUtil.h"
using namespace std;
USING_NS_CC; 

StageTarget::StageTarget()
{
	reset();
}

StageTarget::~StageTarget()
{

}

void StageTarget::starErased(int starType, int color)
{
	m_record[starType]++;

	//所有树藤星星色都设置为随机色
	if (starType == kLiveVine || starType == kDeadVine)
	{
		color = kColorRandom;
	}
	for (auto iter = m_starsLeft.begin(); iter != m_starsLeft.end(); ++iter)
	{
		auto data = *iter;
		if (data.starType == starType && data.color == color)
		{
			int left = data.num - 1;
			iter->num = max(left, 0);
		}
	}
}

void StageTarget::init()
{
	reset();

	auto info = StageDataMgr::theMgr();
	auto config = DataManagerSelf->getStageConfig(info->getCurStage());
	m_winType = config.tagetType;
	m_targetScore = config.targetScore;
	auto param = config.targetParam;

	switch (m_winType)
	{
	case kEraseStars:
	{
		assert(param.size() % 3 == 0);
		for (size_t i = 0; i < param.size(); i += 3)
		{
			EraseStarsData target;
			target.starType = param[i];
			target.color = param[i + 1];
			target.num = param[i + 2];

			m_starsLeft.push_back(target);
		}
		break;
	}
	case kTargetGrid:
	{
		assert(param.size() == 2);
		m_targetGrid.x = param[0];
		m_targetGrid.x = param[1];
		break;
	}
	default:
		assert(false && "on this kind!");
	}
}


void StageTarget::reset()
{
	m_winType = kEraseStars;

	m_targetScore = 0;
	m_targetGrid.x = 0; 
	m_targetGrid.y = 0;

	m_starsLeft.clear();
	m_record.clear();
}

bool StageTarget::isGameOver()
{
	if (isReachTarget()) return true;

	auto stageInfo = StageDataMgr::theMgr();
	return stageInfo->getLeftSteps() <= 0;

}

bool StageTarget::isReachTarget()
{
	switch (m_winType)
	{
	case kEraseStars:
		return isErasedEnoughStars() && isGetEnoughScore();
	case kTargetGrid:
		return isReachTargetGrid();
	default:
		return false;
	}
}

bool StageTarget::isGetEnoughScore()
{
	auto stageInfo = StageDataMgr::theMgr();
	return stageInfo->getCurScore() >= m_targetScore;
}

bool StageTarget::isErasedEnoughStars()
{
	for (size_t i = 0; i < m_starsLeft.size(); ++i)
	{
		if (m_starsLeft[i].num > 0)
		{
			return false;
		}
	}
	return true;
}

bool StageTarget::isReachTargetGrid()
{
	return false;
}

vector<EraseStarsData> StageTarget::getEraseStarsLeft()
{
	return m_starsLeft;
}