#include "UserInfo.h"
#include "SqliteHelper.h"
#include "CommonMacros.h"
#include "TimeUtil.h"
#include <algorithm>
#include "GuideMgr.h"
#include "DataManager.h"
USING_NS_CC;
using namespace std;

UserInfo *UserInfo::theInfo()
{
	static UserInfo info;
	return &info;
}

void UserInfo::loadUserInfo()
{
	char str[100] = { 0 };
	SqliteHelper helper(DB_SAVING);
	string sql = "select * from save_user_info";
	auto result = helper.readRecord(sql.c_str());
	assert(result.size() == 1);

	auto data = result[0];
	m_diamond = atoi(data[1]);
	m_food = atoi(data[2]);
	m_strength = atoi(data[3]);
	m_runeStone = atoi(data[4]);
	m_key = atoi(data[5]);
	m_isFirstPlay = atoi(data[6]) == 1;
	m_firstPlayTime = atoi(data[7]);
	m_lastLoginInTime = atoi(data[8]);
	m_lastSaveStrengthTime = atoi(data[9]);
}

void UserInfo::init()
{
	loadUserInfo();
	
	if (m_isFirstPlay)
	{
		setFirstPlay(false);
		saveFirstPlayTime();
	}
	saveCurLoginInTime();

	m_clock.setTickHandle(bind(&UserInfo::onTick, this, placeholders::_1));
}

void UserInfo::onTick(float dt)
{
	int curTime = time_util::getCurTime();
	int secsInOneStrength = DataManagerSelf->getSystemConfig().strengthAddMins * 60;
	int strengthAdd = (curTime - m_lastSaveStrengthTime) / secsInOneStrength;
	if (strengthAdd > 0)
	{
		setStrength(m_strength + strengthAdd);
	}
	int secsLeft = 0;
	if (!isFullStrength())
	{
		secsLeft = secsInOneStrength - ((curTime - m_lastSaveStrengthTime) % secsInOneStrength);
	}
	NOTIFY_VIEWS(onStrengthLeftTimeChanged, secsLeft);
}

void UserInfo::setDiamond(int value)
{ 
	m_diamond = max(0, value); 

	SqliteHelper sqlHelper(DB_SAVING);
	char str[100] = { 0 };
	sprintf(str, "update save_user_info set %s = '%d' where id = 1;", "diamond", m_diamond);
	sqlHelper.executeSql(str);

	NOTIFY_VIEWS(onDiamondChanged);
}

bool UserInfo::consumeDiamond(int value)
{
	if (m_diamond < value) return false;
	
	setDiamond(m_diamond - value);
	return true;
}

bool UserInfo::hasEnoughDiamond(int value)
{
	return m_diamond >= value;
}

void UserInfo::setFood(int value)
{
	m_food = max(0, value);
	
	SqliteHelper sqlHelper(DB_SAVING);
	char str[100] = { 0 };
	sprintf(str, "update save_user_info set %s = '%d' where id = 1;", "food", m_food);
	sqlHelper.executeSql(str);

	NOTIFY_VIEWS(onFoodChanged);
}

void UserInfo::setStrength(int value)
{
	int maxStrength = DataManagerSelf->getSystemConfig().strengthMax;
	m_strength = min(maxStrength, max(0, value));

	SqliteHelper sqlHelper(DB_SAVING);
	char str[100] = { 0 };
	m_lastSaveStrengthTime = time_util::getCurTime();
	sprintf(str, "update save_user_info set %s = '%d', %s = '%d' where id = 1;", "strength", m_strength, "strength_last_save_time", m_lastSaveStrengthTime);
	sqlHelper.executeSql(str);

	NOTIFY_VIEWS(onStrengthChanged);
}

int UserInfo::getMaxStrength()
{
	return DataManagerSelf->getSystemConfig().strengthMax;
}

bool UserInfo::isFullStrength()
{
	return m_strength >= getMaxStrength();
}

int UserInfo::getOneRoundStrength()
{
	return DataManagerSelf->getSystemConfig().strengthOneRound;
}

bool UserInfo::consumeStrength()
{
	int strengthNeed = DataManagerSelf->getSystemConfig().strengthOneRound;
	if (m_strength < strengthNeed) return false;

	setStrength(m_strength - strengthNeed);
	return true;
}

void UserInfo::setRuneStone(int value)
{
	m_runeStone = max(0, min(value, MAX_RUNESTORN_AMOUNT));

	SqliteHelper sqlHelper(DB_SAVING);
	char str[100] = { 0 };
	sprintf(str, "update save_user_info set %s = '%d' where id = 1;", "rune_stone", m_runeStone);
	sqlHelper.executeSql(str);

	NOTIFY_VIEWS(onRuneStoneChanged);
}

bool UserInfo::canGetRuneStoneReward()
{ 
	return m_runeStone >= MAX_RUNESTORN_AMOUNT; 
}

void UserInfo::setKey(int value)
{
	m_key = max(0, value);

	SqliteHelper sqlHelper(DB_SAVING);
	char str[100] = { 0 };
	sprintf(str, "update save_user_info set %s = '%d' where id = 1;", "key", m_key);
	sqlHelper.executeSql(str);

	NOTIFY_VIEWS(onKeyChanged);
	if (m_key == 0)
	{
		GuideMgr::theMgr()->startGuide(kGuideStart_key_run_out);
	}
}

void UserInfo::setFirstPlay(bool isFirstPlay)
{
	m_isFirstPlay = isFirstPlay;

	SqliteHelper sqlHelper(DB_SAVING);
	char str[100] = { 0 };
	sprintf(str, "update save_user_info set %s = '%d' where id = 1;", "first_play", m_isFirstPlay ? 1 : 0);
	sqlHelper.executeSql(str);
}

void UserInfo::saveFirstPlayTime()
{
	m_firstPlayTime = time_util::getCurTime();

	SqliteHelper sqlHelper(DB_SAVING);
	char str[100] = { 0 };
	sprintf(str, "update save_user_info set %s = '%d' where id = 1;", "first_play_time", m_firstPlayTime);
	sqlHelper.executeSql(str);
}

void UserInfo::saveCurLoginInTime()
{
	m_lastLoginInTime = time_util::getCurTime();
	
	SqliteHelper sqlHelper(DB_SAVING);
	char str[100] = { 0 };
	sprintf(str, "update save_user_info set %s = '%d' where id = 1;", "last_login_in", m_lastLoginInTime);
	sqlHelper.executeSql(str);
}

void UserInfo::addView(IUserInfoView *view)
{
	auto iter = find(m_views.begin(), m_views.end(), view);
	if (iter == m_views.end())
	{
		m_views.push_back(view);
	}
}

void UserInfo::removeView(IUserInfoView *view)
{
	auto iter = find(m_views.begin(), m_views.end(), view);
	if (iter != m_views.end())
	{
		m_views.erase(iter);
	}
}

bool UserInfo::isFirstLoginToday()
{
	int curTime = time_util::getCurTime();
	auto curData = time_util::getDate(curTime);
	auto lastLoginInDate = time_util::getDate(m_lastLoginInTime);

	return !(curData->tm_year == lastLoginInDate->tm_year
		&& curData->tm_mon == lastLoginInDate->tm_mon
		&& curData->tm_mday == lastLoginInDate->tm_mday);
}

int UserInfo::getDaysFromFirstPlay()
{
	static const int kDaySecs = 24 * 3600;
	int startTime = m_firstPlayTime / kDaySecs * kDaySecs;
	
	int days = (m_lastLoginInTime - startTime) / kDaySecs + 1;
	return days;
}

