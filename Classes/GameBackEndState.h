#ifndef __BACKENDSTATE_H__
#define __BACKENDSTATE_H__
#include "cocos2d.h"
class GameBackEndState
{
public:
	static GameBackEndState *theModel();
	void setBusinessMode(bool isBusinessMode){ m_isBusinessMode = isBusinessMode; }
	bool isBusinessMode(){ return m_isBusinessMode; }

	void recordStageStart();
	void recordStageEnd(bool win);
private:
	GameBackEndState();
private:
	bool m_isBusinessMode;
};
#endif