#include "ThiefModel.h"
#include "TimeUtil.h"
#include "DataManager.h"
#include "CommonUtil.h"
#include "GoodsMgr.h"
#include "MsgNotifier.h"
USING_NS_CC;
using namespace std;
using namespace CommonUtil;

ThiefModel::ThiefModel()
: m_lastTriggerTime(0)
, m_curPanelType(kThiefEmpty)
{
	autorelease();
	retain();
}

ThiefModel *ThiefModel::theModel()
{
	static ThiefModel mgr;
	return &mgr;
}

bool ThiefModel::init()
{
	m_lastTriggerTime = time_util::getCurTime();
	CCDirector::sharedDirector()->getScheduler()->scheduleUpdateForTarget(this, 0, false);
	return true;
}

void ThiefModel::update(float delta)
{
	if (m_curPanelType == kThiefEmpty)
	{
		int curTime = time_util::getCurTime();
		kTriggerDuration = 5;
		if (curTime - m_lastTriggerTime >= kTriggerDuration)
		{
			auto configs = DataManagerSelf->getThiefConfigs();
			vector<float> percents;
			for (size_t i = 0; i < configs.size(); ++i)
			{
				percents.push_back(configs[i].percent);
			}

			m_curPanelType = getResultByPercent(percents);
			MsgNotifier::theModel()->onThiefShowUp();
		}
	}
}

bool ThiefModel::isThisPanel(int type)
{
	return m_curPanelType == type;
}

void ThiefModel::triggerThief()
{
	m_lastTriggerTime = time_util::getCurTime();
	m_curPanelType = kThiefEmpty;

	auto reward = DataManagerSelf->getRewardsConfig().thiefReward;
	GoodsMgr::theMgr()->addGoods(reward);
	MsgNotifier::theModel()->onThiefDisappear();
}