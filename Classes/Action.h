#ifndef MH_UTIL_ACTION_H__
#define MH_UTIL_ACTION_H__

#include <cstddef>
#include <vector>
#include <functional>
#include "cocos2d.h"
using namespace cocos2d;

struct IAction : cocos2d::CCObject
{
	virtual bool isDone() = 0;
	virtual float step(float delta) = 0;
	virtual void reset() = 0;
	virtual void stop() { m_stop = true; };

	IAction() : m_stop(false) {}
	~IAction() {}
protected:
	bool m_stop;
};

class SequenseAction : public IAction
{
public:
	~SequenseAction();

	bool isDone();
	float step(float delta);
	void reset();

	void addAction(IAction *action);

	static SequenseAction *withActions(IAction *action1, ...);
private:
	SequenseAction();
	std::vector<IAction *> m_actions;
	size_t m_currentActionIndex;
};

class ParallelAction : public IAction
{
public:
	~ParallelAction();

	bool isDone();
	float step(float delta);
	void reset();

	void addAction(IAction *action);

	static ParallelAction *withActions(IAction *action1, ...);
private:
	ParallelAction();
	std::vector<IAction *> m_actions;
};

class CallFuncAction : public IAction
{
public:
	~CallFuncAction();

	bool isDone();
	float step(float delta);
	void reset();

	static CallFuncAction *withFunctor(std::function<void ()> func);
private:
	CallFuncAction(std::function<void ()> func);
	bool m_called;
	std::function<void ()> m_func;
};

class RepeatNAction : public IAction
{
public:
	~RepeatNAction();

	bool isDone();
	float step(float delta);
	void reset();

	static RepeatNAction *withAction(IAction *action, int n);
private:
	RepeatNAction(IAction *action, int n);
	IAction *m_action;
	int m_repeatTimes;
	int m_repeated;
};

class RepeatForever : public IAction
{
public:
	~RepeatForever();

	bool isDone();
	float step(float delta);
	void reset();

	static RepeatForever *withAction(IAction *action);
private:
	RepeatForever(IAction *action);
	IAction *m_action;
};

class DelayAction : public IAction
{
public:
	~DelayAction();

	bool isDone();
	float step(float delta);
	void reset();

	static DelayAction *withDelay(float delay);
private:
	DelayAction(float delay);
	float m_remain;
	float m_delayTime;
};

class DelayNFrames : public IAction
{
public:
    bool isDone() { return m_stop || m_remain == 0; }
    float step(float delta);
    void reset() { m_remain = m_nframes; }

    static DelayNFrames *delay(int nframes);
private:
    DelayNFrames(int nframes) : m_remain(nframes), m_nframes(nframes) {}
    int m_remain;
    int m_nframes;
};

class XSlowDown : public CCActionInterval
{
public:
	static XSlowDown* create(float duration,float speed,const CCPoint& range);
	virtual void update(float time);
	virtual bool initWithDuration(float d);
	virtual void startWithTarget(CCNode *pTarget);
private:
	XSlowDown(float speed,const CCPoint& range);
	~XSlowDown();
private:
	CCPoint m_range;
	float m_initX;
	float m_speed;
	float m_acc;
	float m_lastEscape;
};

#endif