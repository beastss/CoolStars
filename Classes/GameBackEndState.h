#ifndef __BACKENDSTATE_H__
#define __BACKENDSTATE_H__
#include "cocos2d.h"
class GameBackEndState
{
public:
	static GameBackEndState *theModel();
	bool isBusinessMode();

	void recordStageStart();
	void recordStageEnd(bool win);	

private:
	GameBackEndState();
private:
};
#endif