#ifndef __STARSBEHAVIOR_H__
#define __STARSBEHAVIOR_H__
#include "cocos2d.h"
class StarNode;
class StarsBehavior
{
public:
	StarsBehavior();
	void onOneRoundBegin();
	void onOneRoundEnd();
private:
	std::vector<StarNode *> getLiveVineNodes();
private:
	int m_liveVineAmount;
	bool m_needAddLiveVine;
};
#endif