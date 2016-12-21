#ifndef _DAILYLOGINMODEL_H_
#define _DAILYLOGINMODEL_H_

#include "cocos2d.h"
class DailyLoginModel
{
public:
	static DailyLoginModel *theModel();
	int curLoginDays();
	void getReward();

	void init();
	int getRewardState(int day);
	bool hasRewad();
private:
	std::vector<int> rewardsCanGet();
private:
	DailyLoginModel();
	std::vector<int>m_alreadyGotReward;
public:
	enum 
	{
		kAlreadyGotReward,
		kCanGotReward,
		kCannotGotReward,
	};
};
#endif