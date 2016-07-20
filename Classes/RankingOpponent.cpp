#include "RankingOpponent.h"
#include "DataManager.h"
#include "SqliteHelper.h"
#include "UserInfo.h"
#include "CommonUtil.h"
#include "CommonMacros.h"
#include "RankingModel.h"
#include "StarsController.h"
using namespace std;
using namespace CommonUtil;
USING_NS_CC;

RankingOpponent::RankingOpponent()
: m_opponentId(0)
, m_lastDays(0)
, m_ownPetPercent(0)
, m_stage(0)
{

}

RankingOpponent *RankingOpponent::theOpponent()
{
	static RankingOpponent opponent;
	return &opponent;
}

void RankingOpponent::loadLastSavedData()
{
	SqliteHelper helper(DB_SAVING);
	string sql = "select * from save_opponent_ranking";
	auto result = helper.readRecord(sql.c_str());
	auto data = result[0];

	m_opponentId = atoi(data[1]);
	m_lastDays = atoi(data[2]);

	auto config = DataManagerSelf->getRankingConfig(m_opponentId);
	m_stage = atoi(data[3]);
	m_ownPetPercent = atoi(data[4]);
}

void RankingOpponent::firstInitData(int rankId)
{
	m_opponentId = rankId;

	SqliteHelper helper(DB_SAVING);
	char str[256] = { 0 };
	sprintf(str, "update save_opponent_ranking set %s = '%d' where id = 1;", "rank_id", m_opponentId);
	helper.executeSql(str);
	helper.closeDB();

	auto config = DataManagerSelf->getRankingConfig(m_opponentId);
	m_lastDays = 1;
	m_stage = config.stage;
	m_ownPetPercent = config.ownPetPercent;
	saveData();
}

void RankingOpponent::saveData()
{
	SqliteHelper helper(DB_SAVING);
	string sql;
	char str[256] = { 0 };
	sprintf(str, "update save_opponent_ranking set %s = '%d'", "last_days", m_lastDays);
	sql += str;
	sprintf(str, ",%s = '%d'", "last_stage", m_stage);
	sql += str;
	sprintf(str, ",%s = '%d' where id = 1;", "last_own_pet_percent", m_ownPetPercent);
	sql += str;
	helper.executeSql(sql.c_str());
	helper.closeDB();
}

void RankingOpponent::update()
{
	if (!needUpdate()) return;

	int curdays = UserInfo::theInfo()->getDaysFromFirstPlay();
	int days = curdays - m_lastDays;
	for (int i = 0; i < days; ++i)
	{
		m_stage += getRandomValue(1, 5);
		m_stage = min(StarsController::theModel()->getStageAmount(), m_stage);
		m_ownPetPercent += (int)((float)getRandomValue(0, 3) / PETS_AMOUNT * 100);
		m_ownPetPercent = min(m_ownPetPercent, 100);
	}
	m_lastDays = curdays;
	saveData();
}

bool RankingOpponent::needUpdate()
{
	//没开启过排行榜时，对手信息不需要update
	if (!RankingModel::theModel()->alreadyOpenRanking()) return false;

	int days = UserInfo::theInfo()->getDaysFromFirstPlay();
	return m_lastDays < days;
}

RankingData RankingOpponent::getRankingData()
{
	RankingData data;
	auto config = DataManagerSelf->getRankingConfig(m_opponentId);

	data.name = config.name;
	data.type = kOpponent;
	data.ownPetPercent = m_ownPetPercent;
	data.stage = m_stage;

	return data;
}

