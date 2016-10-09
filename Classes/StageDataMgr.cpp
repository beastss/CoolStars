#include "StageDataMgr.h"
#include "DataManager.h"
#include "StageSavingHelper.h"
#include "CommonUtil.h"
#include <algorithm>
#include "GuideMgr.h"
#include "PropManager.h"

StageDataMgr::StageDataMgr()
{
}

StageDataMgr *StageDataMgr::theMgr()
{
	static StageDataMgr mgr;
	return &mgr;
}

int StageDataMgr::getCurDirection()
{
	int index = 0;
	auto config = DataManagerSelf->getStageConfig(m_curStage);
	auto dirs = config.direction;
	if (m_step > 0)
	{
		index = ((m_step - 1) % dirs.size());
	}
	return dirs[index] + 1;//配置上的0 等同方向上（1）
}

int StageDataMgr::getLeftSteps()
{
	auto config = DataManagerSelf->getStageConfig(m_curStage);
	int stepsReduce = DataManagerSelf->getSystemConfig().stepsReduce;
	int stageAmount = DataManagerSelf->getSystemConfig().stageAmount;
	int leftStep = config.step - m_step - ((m_curStage - 1)/ stageAmount) * stepsReduce;
	return max(leftStep, 0);
}

void StageDataMgr::reset(int gameType)
{
	m_step = 0;
	m_curScore = 0;
	m_curScoreBonus = 0;
	if (gameType == kNormalType)
	{
		m_curStage = m_topStage;
	}
	else if (gameType == kTreasureType && m_topStage > 1)
	{
		m_curStage = CommonUtil::getRandomValue(1, m_topStage - 1);
	}
	GuideMgr::theMgr()->pauseGuide(gameType != kNormalType);
	PropManager::propMgr()->setInfinite(false);
}

void StageDataMgr::init()
{
	StageSavingHelper::LoadLastSavedStageData();
	reset();
}

void StageDataMgr::toNextStage()
{
	reset();
	m_curStage++;
	m_topStage = m_curStage;
	StageSavingHelper::saveCurStageData();
}

void StageDataMgr::setCurScore(int score)
{
	if (score > m_topScore)
	{
		m_topScore = score;
	}

	m_curScore = score;
	NOTIFY_VIEWS(onScoreChanged);

}

int StageDataMgr::getTargetScore()
{
	auto config = DataManagerSelf->getStageConfig(m_curStage);
	return config.targetScore;
}

bool StageDataMgr::isTheLastStage()
{
	auto config = DataManagerSelf->getSystemConfig();
	return m_curStage >= config.stageAmount;
}

void StageDataMgr::getStageStars(std::vector<std::vector<StageStarInfo>> &stars)
{
	DataManagerSelf->getNewStageStarsData(stars, m_curStage);
}

void StageDataMgr::doSave()
{
	StageSavingHelper::saveCurStageData();
}

void StageDataMgr::setCurStep(int step)
{
	m_step = step;
	NOTIFY_VIEWS(onStepsChanged);
}

void StageDataMgr::addCurScore(int value)
{ 
	float score = value * (1 + m_curScoreBonus);
	setCurScore(m_curScore + score);
}

void StageDataMgr::addView(IStageDataView *view)
{
	auto iter = find(m_views.begin(), m_views.end(), view);
	if (iter == m_views.end())
	{
		m_views.push_back(view);
	}
}

void StageDataMgr::removeView(IStageDataView *view)
{
	auto iter = find(m_views.begin(), m_views.end(), view);
	if (iter != m_views.end())
	{
		m_views.erase(iter);
	}
}

//新回合
void StageDataMgr::newRound()
{
	m_curScoreBonus = 0;
}

void StageDataMgr::addResultBonus(const GameResultReward &reward)
{
	m_resultBouns.diamond += reward.diamond;
	m_resultBouns.food += reward.food;
	m_resultBouns.key += reward.key;
	//CCLog("add food : %d", reward.food);
	//CCLog("cur food : %d", m_resultBouns.food);
}

void StageDataMgr::resetResultBonus()
{
	m_resultBouns.diamond = 0;
	m_resultBouns.food = 0;
	m_resultBouns.key = 0;
}
