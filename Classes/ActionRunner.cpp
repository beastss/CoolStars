#include "ActionRunner.h"

#include <CCScheduler.h>
#include "CCDirector.h"

ActionRunner::ActionRunner() : m_isUpdating(false) {}

ActionRunner::~ActionRunner()
{
	if (m_isUpdating) cocos2d::CCDirector::sharedDirector()->getScheduler()->unscheduleUpdateForTarget(this);
	for(size_t i = 0; i < m_actions.size(); ++i)
	{
		m_actions[i]->release();
	}
}

void ActionRunner::queueAction(IAction *action)
{
	if(m_actions.empty() && !m_isUpdating) 
	{
		cocos2d::CCDirector::sharedDirector()->getScheduler()->scheduleUpdateForTarget(this, 0, false);
		m_isUpdating = true;
	}
	
	action->retain();
	m_actions.push_back(action);
}

bool ActionRunner::isRunning() { return !m_actions.empty(); }

void ActionRunner::stopCurrent()
{
	assert(!m_actions.empty());
	if(m_actions.empty()) return;

	m_actions[0]->stop();
	m_actions[0]->release();
	m_actions.pop_front();

	if(m_actions.empty() && m_isUpdating)
	{
		cocos2d::CCDirector::sharedDirector()->getScheduler()->unscheduleUpdateForTarget(this);
		m_isUpdating = false;
	}
}

void ActionRunner::update(float delta)
{
	while(delta > 0.001 && !m_actions.empty())
	{
		delta -= m_actions[0]->step(delta);
		if (m_actions.empty())
		{
			break;
		}
		if(m_actions[0]->isDone())
		{
			m_actions[0]->release();
			m_actions.pop_front();
		}
	}

	if(m_actions.empty() && m_isUpdating)
	{
		cocos2d::CCDirector::sharedDirector()->getScheduler()->unscheduleUpdateForTarget(this);
		m_isUpdating = false;
	}
}

void ActionRunner::pause()
{
	if (m_isUpdating)
	{
		cocos2d::CCDirector::sharedDirector()->getScheduler()->unscheduleUpdateForTarget(this);
		m_isUpdating = false;
	}
}

void ActionRunner::resume()
{
	if (!m_isUpdating)
	{
		cocos2d::CCDirector::sharedDirector()->getScheduler()->scheduleUpdateForTarget(this, 0, false);
		m_isUpdating = true;
	}
}

void ActionRunner::clear()
{
	for (size_t i = 0; i < m_actions.size(); ++i)
	{
		m_actions[i]->release();
	}
	m_actions.clear();
	if (m_isUpdating)
	{
		cocos2d::CCDirector::sharedDirector()->getScheduler()->unscheduleUpdateForTarget(this);
		m_isUpdating = false;
	}
}

ActionRunner *ActionRunner::create()
{
	ActionRunner *runner = new ActionRunner();
	if (runner)
	{
		runner->autorelease();
		return runner;
	}

	CC_SAFE_DELETE(runner);
	return NULL;
}

