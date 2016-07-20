#ifndef MH_ACTION_RUNNER_H__
#define MH_ACTION_RUNNER_H__

#include <deque>

#include "Action.h"

class ActionRunner : public cocos2d::CCObject
{
public:
	static ActionRunner *create();

	bool isRunning();
	void stopCurrent();
	void queueAction(IAction *action);

	void update(float delta);
	void clear();

	void pause();
	void resume();
private:
	ActionRunner();
	~ActionRunner();

private:
	bool m_isUpdating;
	std::deque<IAction *> m_actions;
   
};


#endif