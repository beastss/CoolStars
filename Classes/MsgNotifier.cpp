#include "MsgNotifier.h"
#include <algorithm>

MsgNotifier *MsgNotifier::theModel()
{
	static MsgNotifier model;
	return &model;
}

void MsgNotifier::addView(IMsgWatcherView *view)
{
	auto iter = find(m_views.begin(), m_views.end(), view);
	if (iter == m_views.end())
	{
		m_views.push_back(view);
	}
}

void MsgNotifier::removeView(IMsgWatcherView *view)
{
	auto iter = find(m_views.begin(), m_views.end(), view);
	if (iter != m_views.end())
	{
		m_views.erase(iter);
	}
}
