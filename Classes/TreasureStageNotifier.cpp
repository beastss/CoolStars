#include "TreasureStageNotifier.h"
#include <algorithm>

TreasureStageNotifier *TreasureStageNotifier::theNotifier()
{
	static TreasureStageNotifier model;
	return &model;
}

void TreasureStageNotifier::addView(ITreasureStageView *view)
{
	auto iter = find(m_views.begin(), m_views.end(), view);
	if (iter == m_views.end())
	{
		m_views.push_back(view);
	}
}

void TreasureStageNotifier::removeView(ITreasureStageView *view)
{
	auto iter = find(m_views.begin(), m_views.end(), view);
	if (iter != m_views.end())
	{
		m_views.erase(iter);
	}
}
