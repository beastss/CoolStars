#include "GuideMgr.h"
#include "SqliteHelper.h"
#include "CommonUtil.h"
#include "DataManager.h"
#include "StarsController.h"
#include "StageDataMgr.h"
#include "GuideMacros.h"
#include "MainScene.h"
#include <algorithm>
using namespace std;
USING_NS_CC;
using namespace CommonUtil;

GuideMgr *GuideMgr::theMgr()
{
	static GuideMgr mgr;
	return &mgr;
}

GuideMgr::GuideMgr()
: m_curGuideId(kNotTriggerGuide)
, m_guideEnable(true)
, m_pauseGuide(false)
, m_isRunning(false)
{
}

void GuideMgr::startGuide(int startAction, std::function<void()> callback)
{
	if (!m_guideEnable || m_pauseGuide) return;

	int topStage = StageDataMgr::theMgr()->getTopStage();

	auto configs = DataManagerSelf->getGuideConfig();
	auto iter = find_if(configs.begin(), configs.end(), [=](GuideConfig data)
	{
		//如果已经完成，放弃
		if (find(m_finishedGuides.begin(), m_finishedGuides.end(), data.id) != m_finishedGuides.end())
		{
			return false;
		}
		//如果配置里的关卡数为0，表示不关心当前所在关卡
		if (data.stage == 0)
		{
			data.stage = topStage;
		}
		return data.stage == topStage 
			&& data.startAction == startAction;
	});
	
	if (iter != configs.end())
	{
		m_curGuideId = iter->id;
		m_isRunning = true;

		if (callback)
		{
			callback();
		}
		if (iter->showGuideView)
		{
			MainScene::theScene()->showGuideView(m_curGuideId);
		}
		if (iter->endAction == kGuideEnd_Default)
		{
			endGuide(kGuideEnd_Default);
		}
	}
}

void GuideMgr::endGuide(int endAction, int param)
{
	if (!m_guideEnable) return;

	if (m_curGuideId == kNotTriggerGuide) return;
	
	auto config = DataManagerSelf->getGuideConfigById(m_curGuideId);
	if (config->endAction == endAction && config->param == param)
	{
		finishGuide();
	}
}

void GuideMgr::finishGuide()
{
	auto iter = find(m_finishedGuides.begin(), m_finishedGuides.end(), m_curGuideId);
	if (iter != m_finishedGuides.end()) return;

	m_finishedGuides.push_back(m_curGuideId);
	m_curGuideId = kNotTriggerGuide;
	m_isRunning = false;

	MainScene::theScene()->removeGuideView();
	onSave();
}

void GuideMgr::init()
{
	SqliteHelper sqlHelper(DB_SAVING);
	auto result = sqlHelper.readRecord("select * from save_guide");
	for (auto iter = result.begin(); iter != result.end(); ++iter)
	{
		auto data = *iter;
		m_finishedGuides = parseStrToInts(data[1]);
		m_guideEnable = atoi(data[2]) == 1;
	}
}

void GuideMgr::onSave()
{
	string sql;
	char str[100] = { 0 };
	SqliteHelper helper(DB_SAVING);

	sprintf(str, "replace into save_guide values(1, \"%s\", %d);", parseIntsToStr(m_finishedGuides).c_str(), m_guideEnable ? 1 : 0);

	helper.executeSql(str);
	helper.closeDB();
}

void GuideMgr::setGuideEnable(bool enable)
{
	m_guideEnable = enable;
	onSave();
}