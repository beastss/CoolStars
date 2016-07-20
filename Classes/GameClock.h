#ifndef __GAMECLOCK_H__
#define __GAMECLOCK_H__
#include "cocos2d.h"
struct IGameClock
{
	virtual void onTick(float dt){}
};

class GameClock
	: public cocos2d::CCNode
{
public :
	static GameClock *theClock();
	void addListener(IGameClock *iClock);
	void removeListener(IGameClock *iClock);
private:
	virtual bool init();
	void tick(float dt);
private:
	std::vector<IGameClock *>m_listeners;
};

class TickClock : public IGameClock
{
public:
	TickClock();
	~TickClock();
	void setTickHandle(std::function<void(float dt)> handle){ m_handle = handle; }
private:
	virtual void onTick(float dt);
private:
	std::function<void(float dt)> m_handle;
};
#endif