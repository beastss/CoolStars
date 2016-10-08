#include "LotteryModel.h"
#include "DataManager.h"
#include "StarsController.h"
#include "PetManager.h"
#include "CommonUtil.h"
#include "StageDataMgr.h"
#include "UserInfo.h"
#include "PackageModel.h"
#include <algorithm>
#include "GameDataAnalysis.h"
using namespace std;
using namespace CommonUtil;

LotteryModel *LotteryModel::theModel()
{
	static LotteryModel model;
	return &model;
}

LotteryData LotteryModel::getLotteryResult()
{
	LotteryData data;
	//在指定关卡里,必然抽到某宠物（对应lotteryPet表）
	auto curStage = StageDataMgr::theMgr()->getTopStage() - 1;//对应于前一关
	curStage = max(curStage, 1);
	auto lotteryPetConfig = DataManagerSelf->getLotteryPetConfigByStage(curStage);
	if (lotteryPetConfig && !PetManager::petMgr()->ownedThisPet(lotteryPetConfig->petId))
	{
		data.type = kLotteryPet;
		data.param = lotteryPetConfig->petId;
	}
	else
	{
		//按照百分比获取宝箱内容（对应于lotteryOutput表）
		vector<float> percents;
		auto lotteryOutputConfig = DataManagerSelf->getLotteryOutputConfigs();
		for (size_t i = 0; i < lotteryOutputConfig.size(); ++i)
		{
			percents.push_back(lotteryOutputConfig[i].percent);
		}
		int result = getResultByPercent(percents);
		data.type = result;
		if (result == kLotteryPet)
		{
			//未获得的宠物 - 不能获取的宠物（后面关卡必然要获取的宠物,所以现在不能获得，在lotteryPet表里）
			auto notOwnedPets = PetManager::petMgr()->getNotOwnedPetIds();
			auto canOwnPets = getDifference(notOwnedPets, getPetsCanNotOwn());
			bool canFreeGetPet = DataManagerSelf->getSystemConfig().lotteryCanGetPetStage <= curStage;
			if (canOwnPets.empty() || !canFreeGetPet)
			{
				return getLotteryResult();
			}
			auto index = getRandomValue(0, canOwnPets.size() - 1);
			data.param = canOwnPets[index];
		}
		else
		{
			data.param = lotteryOutputConfig[result].amount;
		}
	}

	return data;
}

void LotteryModel::doLottery(const LotteryData &data, bool consume)
{
	switch (data.type)
	{
	case kLotteryLittleStrength:
	case kLotteryBagStrength:
	{
		int oldValue = UserInfo::theInfo()->getStrength();
		UserInfo::theInfo()->setStrength(oldValue + data.param);
		break;
	}
	case kLotteryLittleDiamond:
	case kLotteryBagDiamond:
	{
	   int oldValue = UserInfo::theInfo()->getDiamond();
	   UserInfo::theInfo()->setDiamond(oldValue + data.param);
	   break;
	}
	case kLotteryLittleFood:
	case kLotteryBagFood:
	{
		int oldValue = UserInfo::theInfo()->getFood();
		UserInfo::theInfo()->setFood(oldValue + data.param);
		break;
	}
	case kLotteryPet:
		PetManager::petMgr()->addNewPet(data.param);
		break;
	}

	if (consume)
	{
		int oldValue = UserInfo::theInfo()->getKey();
		if (oldValue > 0)
		{
			UserInfo::theInfo()->setKey(oldValue - 1);
		}
		else
		{
			int cost = DataManagerSelf->getSystemConfig().diamondsForOneKey;
			UserInfo::theInfo()->consumeDiamond(cost);
			GameDataAnalysis::theModel()->consumeDiamond(kDiamondConsumeLottery, 0, cost);
		}
	}
}

//不能获取的宠物id
std::vector<int> LotteryModel::getPetsCanNotOwn()
{
	vector<int> ids;
	auto curStage = StageDataMgr::theMgr()->getTopStage() - 1;
	curStage = max(1, curStage);
	//不能拥有后面关卡抽到宠物
	auto config = DataManagerSelf->getLotteryPetConfig();
	for (auto iter = config.begin(); iter != config.end(); ++iter)
	{
		if (iter->stage > curStage)
		{
			ids.push_back(iter->petId);
		}
	}
	//不能拥有新手宠物礼包的宠物

	int petId = PackageModel::theModel()->getPetPacakagePetId();
	if (find(ids.begin(), ids.end(), petId) == ids.end())
	{
		ids.push_back(petId);
	}
	return ids;
}