#include "GuideDelegate.h"
#include "GuideMgr.h"
#include "GuideMacros.h"
#include "MainScene.h"
using namespace std;

/*
void GuideDelegate::setPanelId(int panelId)
{
	m_panelId = panelId;
}

void GuideDelegate::registerCallback(int guideType, std::function<void()> callback)
{
	m_callbacks[guideType] = callback;
}

void GuideDelegate::guideCheck(int guideType)
{
	int guideId = GuideMgr::theMgr()->startGuide(m_panelId, guideType);
	if (guideId == kNotTriggerGuide) return;

	if (m_callbacks.find(guideType) != m_callbacks.end())
	{
		m_callbacks[guideType]();
		MainScene::theScene()->showGuideView(guideId);
	}
}
*/