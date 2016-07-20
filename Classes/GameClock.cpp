#include "GameClock.h"
#include <algorithm>

USING_NS_CC;
using namespace std;

GameClock *GameClock::theClock()
{
	static GameClock model;
	model.init();
	return &model;
}

bool GameClock::init()
{
	CCDirector::sharedDirector()->getScheduler()->scheduleSelector(SEL_SCHEDULE(&GameClock::tick), this, 1, false);
	return true;
}

void GameClock::tick(float dt)
{
	for (size_t i = 0; i < m_listeners.size(); ++i)
	{
		m_listeners[i]->onTick(dt);
	}
}

void GameClock::addListener(IGameClock *iClock)
{
	auto iter = find(m_listeners.begin(), m_listeners.end(), iClock);
	if (iter == m_listeners.end())
	{
		m_listeners.push_back(iClock);
	}
}

void GameClock::removeListener(IGameClock *iClock)
{
	auto iter = find(m_listeners.begin(), m_listeners.end(), iClock);
	if (iter != m_listeners.end())
	{
		m_listeners.erase(iter);
	}
}
//////////////////////////////////////////////////////////
TickClock::TickClock()
{
	GameClock::theClock()->addListener(this);
}

TickClock::~TickClock()
{
	GameClock::theClock()->removeListener(this);

}

void TickClock::onTick(float dt)
{
	if (m_handle)
	{
		m_handle(dt);
	}
}