#ifndef _USERINFO_H_
#define _USERINFO_H_
#include "cocos2d.h"
#include <functional>
#include "GameClock.h"
struct IUserInfoView
{
	virtual void onDiamondChanged(){}
	virtual void onFoodChanged(){}
	virtual void onStrengthChanged(){}
	virtual void onRuneStoneChanged(){}
	virtual void onKeyChanged(){}
	virtual void onStrengthChanged(int leftSecs){}
};
class UserInfo
{
public: 
	static UserInfo *theInfo();

	void loadUserInfo();
	void init();

	void addView(IUserInfoView *view);
	void removeView(IUserInfoView *view);
public:
	void setDiamond(int value);
	int getDiamond(){ return m_diamond; }
	bool consumeDiamond(int value);
	bool hasEnoughDiamond(int value);

	void setFood(int value);
	int getFood(){ return m_food; }

	void setStrength(int value);
	int getStrength(){ return m_strength; }
	int getMaxStrength();
	int getOneRoundStrength();
	bool consumeStrength();//进入关卡使用体力

	void setRuneStone(int value);
	int getRuneStone(){ return m_runeStone; }
	bool canGetRuneStoneReward();
	
	void setKey(int value);
	int getKey(){ return m_key; }

	void setFirstPlay(bool isFirstPlay);
	bool isFirstPlay(){ return m_isFirstPlay; }

	void saveFirstPlayTime();
	int getFirstPlayTime(){ return m_firstPlayTime; }
	int getDaysFromFirstPlay();

	void saveCurLoginInTime();
	int getLastLoginInTime(){ return m_lastLoginInTime; }
	bool isFirstLoginToday();
private:
	UserInfo(){}
	void onTick(float dt);
private:
	int m_diamond;
	int m_food;
	int m_strength;
	int m_runeStone; //符石
	int m_key;

	bool m_isFirstPlay;
	int m_firstPlayTime;
	int m_lastLoginInTime;
	int m_lastSaveStrengthTime;

	std::vector<IUserInfoView *>m_views;
	TickClock m_clock;
};
#endif