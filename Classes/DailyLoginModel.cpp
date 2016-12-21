#include "DailyLoginModel.h"
#include "UserInfo.h"
#include "DataManager.h"
#include "GoodsMgr.h"
#include "CommonMacros.h"
#include "PetManager.h"
#include "MsgNotifier.h"
#include "SqliteHelper.h"
#include "CommonUtil.h"
#include <algorithm>
using namespace std;

DailyLoginModel::DailyLoginModel()
{

}

DailyLoginModel *DailyLoginModel::theModel()
{
	static DailyLoginModel model;
	return &model;
}

int DailyLoginModel::curLoginDays()
{
	return UserInfo::theInfo()->getCurLoginDays();
}

void DailyLoginModel::getReward()
{
	auto days = rewardsCanGet();
	
	for (int i = 0; i < days.size(); ++i)
	{
		m_alreadyGotReward.push_back(days[i]);
		auto config = DataManagerSelf->getDailyLoginConfig(days[i]);
		if (config.goods.type == kGoodsPets)
		{
			int petId = config.goods.goodsId;
			bool exist = PetManager::petMgr()->ownedThisPet(petId);
			MsgNotifier::theModel()->onDailyLoginPetReward(petId, !exist);
			//如果宠物已经存在，送100钻石
			if (exist)
			{
				config.goods.type = kGoodsDiamond;
				config.goods.goodsId = 0;
				config.goods.amount = 500;//hard code
			}
		}

		GoodsMgr::theMgr()->addGoods(config.goods);
	}
	
	SqliteHelper sqlHelper(DB_SAVING);
	char str[100] = { 0 };
	sprintf(str, "update save_state set got_login_reward = '%s' where id = 1;", CommonUtil::parseIntsToStr(m_alreadyGotReward).c_str());
	sqlHelper.executeSql(str);
}

void DailyLoginModel::init()
{
	SqliteHelper sqlHelper(DB_SAVING);
	char str[100] = { 0 };
	sprintf(str, "select got_login_reward from save_state where id = 1;");
	auto result = sqlHelper.readRecord(str);
	m_alreadyGotReward = CommonUtil::parseStrToInts(result[0][0]);
}

int DailyLoginModel::getRewardState(int day)
{
	if (find(m_alreadyGotReward.begin(), m_alreadyGotReward.end(), day) != m_alreadyGotReward.end())
	{
		return kAlreadyGotReward;
	}

	return day <= curLoginDays() ? kCanGotReward : kCannotGotReward;
}

bool DailyLoginModel::hasRewad()
{
	return !rewardsCanGet().empty();
}

std::vector<int> DailyLoginModel::rewardsCanGet()
{
	vector<int> days;
	const int kMaxDays = 7;
	int rewardDays = min(curLoginDays(), kMaxDays);
	
	for (int day = 1; day <= rewardDays; ++day)
	{
		if (find(m_alreadyGotReward.begin(), m_alreadyGotReward.end(), day) == m_alreadyGotReward.end())
		{
			days.push_back(day);
		}
	}
	return days;
}