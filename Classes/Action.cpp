#include "Action.h"
#include <stdarg.h>

SequenseAction::SequenseAction() : m_currentActionIndex(0)
{
}

SequenseAction::~SequenseAction()
{
	for(size_t i = 0; i < m_actions.size(); ++i)
	{
		m_actions[i]->release();
	}
}

bool SequenseAction::isDone()
{
	return m_stop || m_currentActionIndex == m_actions.size();
}


float SequenseAction::step(float delta)
{
	float remain = delta;
	while(remain > 0.0001f)
	{
		float used = m_actions[m_currentActionIndex]->step(remain);
		remain -= used;

		if(m_actions[m_currentActionIndex]->isDone())
			++m_currentActionIndex;
		if(m_currentActionIndex >= m_actions.size())
			return delta - remain;
	}
	
	return delta;
}

void SequenseAction::reset()
{
	m_currentActionIndex = 0;
	m_stop = false;
	for(size_t i = 0; i < m_actions.size(); ++i)
	{
		m_actions[i]->reset();
	}
}

void SequenseAction::addAction(IAction *action)
{
	action->retain();
	m_actions.push_back(action);
}

SequenseAction *SequenseAction::withActions(IAction *action1, ...)
{
	va_list params;
	va_start(params, action1);

	SequenseAction *sa = new SequenseAction();
	sa->autorelease();
	sa->addAction(action1);

	while (action1)
	{
		IAction *action = va_arg(params, IAction *);
		if(!action)
			break;
		sa->addAction(action);
	}

	va_end(params);
	return sa;
}



ParallelAction::ParallelAction() {}

ParallelAction::~ParallelAction()
{
	for(size_t i = 0; i < m_actions.size(); ++i)
	{
		m_actions[i]->release();
	}
}

bool ParallelAction::isDone()
{
	if(m_stop)
		return true;

	for(size_t i = 0; i < m_actions.size(); ++i)
	{
		if(!m_actions[i]->isDone())
			return false;
	}

	return true;
}

float ParallelAction::step(float delta)
{
	float maxUsed = 0.0f;
	for(size_t i = 0; i < m_actions.size(); ++i)
	{
		if(!m_actions[i]->isDone())
		{
			float used = m_actions[i]->step(delta);
			if(used > maxUsed)
				maxUsed = used;
		}
	}

	return maxUsed;
}

void ParallelAction::reset()
{
	m_stop = false;
	for(size_t i = 0; i < m_actions.size(); ++i)
	{
		m_actions[i]->reset();
	}
}

void ParallelAction::addAction(IAction *action)
{
	action->retain();
	m_actions.push_back(action);
}

ParallelAction *ParallelAction::withActions(IAction *action1, ...)
{
	va_list params;
	va_start(params, action1);

	ParallelAction *pa = new ParallelAction();
	pa->autorelease();
	pa->addAction(action1);

	while (action1)
	{
		IAction *action = va_arg(params, IAction *);
		if(!action)
			break;
		pa->addAction(action);
	}

	va_end(params);
	return pa;
}

CallFuncAction::CallFuncAction(std::function<void ()> func) : m_func(func), m_called(false)
{
}

CallFuncAction::~CallFuncAction() {}

bool CallFuncAction::isDone() { return m_called; }
float CallFuncAction::step(float delta)
{
	m_called = true;
	m_func();
	return 0;
}

void CallFuncAction::reset() { m_called = false; }

CallFuncAction *CallFuncAction::withFunctor(std::function<void ()> func)
{
	CallFuncAction *action = new CallFuncAction(func);
	action->autorelease();

	return action;
}

RepeatNAction::RepeatNAction(IAction *action, int n) : m_action(action), m_repeatTimes(n), m_repeated(0)
{
	m_action->retain();
}

RepeatNAction::~RepeatNAction()
{
	m_action->release();
}

bool RepeatNAction::isDone()
{
	return m_repeated == m_repeatTimes && m_action->isDone();
}

float RepeatNAction::step(float delta)
{
	float remain = delta;
	while(remain > 0)
	{
		if(!m_action->isDone())
		{
			remain -= m_action->step(remain);
			if(m_action->isDone())
			{
				++m_repeated;
				if(m_repeated == m_repeatTimes)
					return delta - remain;
				m_action->reset();
			}
		}
	}

	return delta;
}

void RepeatNAction::reset()
{
	m_stop = false;
	m_action->reset();
	m_repeated = 0;
}

RepeatNAction *RepeatNAction::withAction(IAction *action, int n)
{
	RepeatNAction *ra = new RepeatNAction(action, n);
	ra->autorelease();
	return ra;
}

RepeatForever::RepeatForever(IAction *action) : m_action(action)
{
	m_action->retain();
}

RepeatForever::~RepeatForever()
{
	m_action->release();
}

bool RepeatForever::isDone() { return m_stop; }
float RepeatForever::step(float delta)
{
	float remain = delta;
	while(remain > 0)
	{
		if(m_action->isDone())
			m_action->reset();
		remain -= m_action->step(remain);
	}

	return delta;
}

void RepeatForever::reset() { m_stop = false; }

RepeatForever *RepeatForever::withAction(IAction *action)
{
	RepeatForever *ra = new RepeatForever(action);
	ra->autorelease();
	return ra;
}

DelayAction::DelayAction(float delay) : m_remain(delay), m_delayTime(delay)
{
}

DelayAction::~DelayAction() {}

bool DelayAction::isDone() { return m_stop || m_remain == 0.0f; }

float DelayAction::step(float delta)
{
	if(m_remain > delta)
	{
		m_remain -= delta;
		return delta;
	}

	delta = m_remain;
	m_remain = 0.0f;
	return delta;
}

void DelayAction::reset() { m_stop = false; m_remain = m_delayTime; }

DelayAction *DelayAction::withDelay(float delay)
{
	DelayAction *action = new DelayAction(delay);
	action->autorelease();
	return action;
}

float DelayNFrames::step(float delta)
{
    --m_remain;
    return delta;
}

DelayNFrames *DelayNFrames::delay(int nframes)
{
    DelayNFrames *action = new DelayNFrames(nframes);
    action->autorelease();
    return action;
}


XSlowDown* XSlowDown::create(float duration,float speed,const CCPoint& range)
{
	XSlowDown* slowdown = new XSlowDown(speed,range);
	slowdown->initWithDuration(duration);
	slowdown->autorelease();
	return slowdown;
}
XSlowDown::XSlowDown(float speed,const CCPoint& range) : m_speed(speed),m_range(range),m_lastEscape(0)
{
}
XSlowDown::~XSlowDown()
{}

bool XSlowDown::initWithDuration(float d)
{
	CCActionInterval::initWithDuration(d);
	m_acc = m_speed / d;
	return true;
}

void XSlowDown::startWithTarget(CCNode *pTarget)
{
	CCActionInterval::startWithTarget(pTarget);
	m_initX = pTarget->getPositionX();
}

void XSlowDown::update(float t)
{
	float newx = m_pTarget->getPositionX() + (m_speed - getElapsed() * m_acc) * (getElapsed()-m_lastEscape);

	if(newx < m_range.x)
	{
		m_pTarget->setPositionX(m_range.x);
		m_elapsed = getDuration();
	}
	else if(newx > m_range.y)
	{
		m_pTarget->setPositionX(m_range.y);
		m_elapsed = getDuration();
	}
	else
	{
		m_pTarget->setPositionX(newx);

		float curSpeed = m_speed - getElapsed() * m_acc;
	}
	m_lastEscape = getElapsed();
}


