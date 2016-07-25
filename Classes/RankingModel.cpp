#include "RankingModel.h"
#include "CommonUtil.h"
#include "SqliteHelper.h"
#include "DataManager.h"
#include "StarsController.h"
#include "StageDataMgr.h"
#include "PetManager.h"
#include "UserInfo.h"
#include "GoodsMgr.h"
#include <algorithm>
USING_NS_CC;
using namespace std;
using namespace CommonUtil;

RankingModel::RankingModel()
: m_alreadyOpenRanking(false)
, m_daysOverOpponent(0)
{

}

RankingModel *RankingModel::theModel()
{
	static RankingModel model;
	return &model;
}

static char getRandomLetter()
{
	bool isLowCase = getRandomValue(0, 1) == 1 ? true : false;
	int letterIndex = getRandomValue(0, 25);
	char letter = isLowCase ? letterIndex + 'a' : letterIndex + 'A';
	return letter;
}

string RankingModel::getRandomName()
{
	string name;

	//第一位必然是字母
	name += getRandomLetter();
	for (int i = 0; i < kMaxLength - 1; ++i)
	{
		bool isExit = getRandomValue(0, 1) == 1 ? true : false;
		if (isExit)
		{
			name += getRandomLetter();
		}
	}

	//字母不够kMaxLength的补数字
	for (int i = name.size(); i < kMaxLength; ++i)
	{
		bool isExit = getRandomValue(0, 1) == 1 ? true : false;
		char numLetter = getRandomValue(0, 9) + '0';
		if(isExit)
		{
			name += numLetter;
		}
	}
	
	return name;
}

static bool isLetter(char ch)
{
	return (ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z');
}

static bool isNumber(char ch)
{
	return ch >= '0' && ch <= '9';
}

bool RankingModel::isValidName(string name)
{
	if (name.empty()) return false;
	if (name.size() > kMaxLength) return false;

	for (auto i = 0; i < name.size(); ++i)
	{
		if (!isLetter(name[i]) && !isNumber(name[i]))
		{
			return false;
		}
	}
	return true;
}

void RankingModel::init()
{
	loadData();
}

void RankingModel::loadData()
{
	SqliteHelper helper(DB_SAVING);
	string sql = "select * from save_my_ranking";
	auto result = helper.readRecord(sql.c_str());
	auto data = result[0];

	m_alreadyOpenRanking = atoi(data[1]) == 1;

	if (m_alreadyOpenRanking)
	{
		m_myName = data[2];
		m_daysOverOpponent = atoi(data[3]);
		RankingOpponent::theOpponent()->loadLastSavedData();
	}
}

void RankingModel::initFirstOpenRanking(string myName)
{
	SqliteHelper sqlHelper(DB_SAVING);
	char str[100] = { 0 };

	m_alreadyOpenRanking = true;
	sprintf(str, "update save_my_ranking set %s = '%d' where id = 1;", "already_open_ranking", m_alreadyOpenRanking ? 1 : 0);
	sqlHelper.executeSql(str);

	m_myName = myName;
	sprintf(str, "update save_my_ranking set %s = '%s' where id = 1;", "my_name", m_myName.c_str());
	sqlHelper.executeSql(str);
	//排序表数据
	auto datas = DataManagerSelf->getRankingConfigs();
	
	//根据 最高得分 和 宠物拥有比 计算排名。对手的排名在玩家之前
	int topStage = StageDataMgr::theMgr()->getTopStage();
	int ownPetNum = PetManager::petMgr()->getOwnedPetIds().size();
	int petsAmount = PetManager::petMgr()->getMaxPetsAmount();
	int ownPetPercent = int((float)ownPetNum / petsAmount * 100);
	auto iter = find_if(datas.begin(), datas.end(), [=](RankingConfig data)->bool
	{
		if (topStage == data.stage)
		{
			return ownPetPercent > data.ownPetPercent;
		}
		return topStage > data.stage;
	});

	int opponentId = datas.size();
	//如果玩家不是最后一名
	if (iter != datas.end())
	{
		int temp = iter->id - 1;
		//如果玩家不是第一名
		if (temp != 0)
		{
			opponentId = temp;
		}
	}

	RankingOpponent::theOpponent()->firstInitData(opponentId);
}

bool RankingModel::isOverOpponent()
{
	int days = UserInfo::theInfo()->getDaysFromFirstPlay();
	if (days <= m_daysOverOpponent) return false;

	auto ranking = getCurRanking();
	auto playerIter = find_if(ranking.begin(), ranking.end(), [=](RankingData data)->bool
	{
		return data.type == kPlayer;
	});

	auto opponentIter = find_if(ranking.begin(), ranking.end(), [=](RankingData data)->bool
	{
		return data.type == kOpponent;
	});

	assert(playerIter != ranking.end() && opponentIter != ranking.end());
	return *playerIter > *opponentIter;
}

void RankingModel::getOverOpponentReward()
{
	SqliteHelper sqlHelper(DB_SAVING);
	m_daysOverOpponent = UserInfo::theInfo()->getDaysFromFirstPlay();
	char str[100] = { 0 };
	sprintf(str, "update save_my_ranking set %s = '%d' where id = 1;", "days_over_opponent", m_daysOverOpponent);
	sqlHelper.executeSql(str);
	sqlHelper.closeDB();

	auto rewards = DataManagerSelf->getRewardsConfig().rankingOverOpponent;
	GoodsMgr::theMgr()->addGoods(rewards);
}

RankingData RankingModel::getMyRankingData()
{
	int stage = StageDataMgr::theMgr()->getTopStage();
	int ownPetNum = PetManager::petMgr()->getOwnedPetIds().size();
	int petsAmount = PetManager::petMgr()->getMaxPetsAmount();
	int ownPetPercent = int((float)ownPetNum / petsAmount * 100);

	RankingData data;
	data.name = m_myName;
	data.stage = stage;
	data.type = kPlayer;
	data.ownPetPercent = ownPetPercent;

	return data;
}

vector<RankingData> RankingModel::getCurRanking()
{
	vector<RankingData> datas;
	auto configs = DataManagerSelf->getRankingConfigs();
	auto opponent = RankingOpponent::theOpponent();
	for (size_t i = 0; i < configs.size(); ++i)
	{
		if (configs[i].id == opponent->getRankId())
		{
			continue;
		}
		RankingData data;
		data.type = kData;
		data.name = configs[i].name;
		data.ownPetPercent = configs[i].ownPetPercent;
		data.stage = configs[i].stage;
		datas.push_back(data);
	}
	datas.push_back(getMyRankingData());
	datas.push_back(opponent->getRankingData());
	sort(datas.begin(), datas.end(), greater<RankingData>());
	return datas;
}

map<int, RankingData> RankingModel::getNeighboursRanking()
{
	auto ranking = getCurRanking();
	auto playerIter = find_if(ranking.begin(), ranking.end(), [=](RankingData data)->bool
	{
		return data.type == kPlayer;
	});
	
	static const int kAmount = 5;
	int startIndex = max(0, playerIter - ranking.begin() - kAmount + 1);
	map<int, RankingData> neighbours;
	for (int i = 0; i < kAmount;  ++i)
	{
		int index = startIndex + i;
		neighbours.insert(make_pair(index + 1, ranking[index]));
	}
	return neighbours;
}

int RankingModel::getOpponetRank()
{
	auto ranking = getCurRanking();
	auto iter = find_if(ranking.begin(), ranking.end(), [=](RankingData data)->bool
	{
		return data.type == kOpponent;
	});
	assert(iter != ranking.end());
	return iter - ranking.begin() + 1;
}

int RankingModel::getMyRank()
{
	auto ranking = getCurRanking();
	auto iter = find_if(ranking.begin(), ranking.end(), [=](RankingData data)->bool
	{
		return data.type == kPlayer;
	});
	assert(iter != ranking.end());
	return iter - ranking.begin() + 1;
}

RankingData RankingModel::getDataByRank(int rank)
{
	auto ranking = getCurRanking();
	assert(rank > 0 && rank <= ranking.size());
	return ranking[rank - 1];
}
