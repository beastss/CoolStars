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
#include "MyPurchase.h"

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
	if (lotteryPetConfig 
		&& !PetManager::petMgr()->ownedThisPet(lotteryPetConfig->petId) 
		&& !petSelected(lotteryPetConfig->petId))
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
			int petId = getOneNotOwndPetId();
			if (petId == kNoNewPets)//已经抽不到宠物了
			{
				return getLotteryResult();
			}
			else
			{
				data.param = petId;
			}
		}
		else
		{
			data.param = lotteryOutputConfig[result].amount;
		}
	}

	return data;
}

int LotteryModel::getOneNotOwndPetId()
{
	auto curStage = StageDataMgr::theMgr()->getTopStage() - 1;//对应于前一
	//未获得的宠物 - 不能获取的宠物（后面关卡必然要获取的宠物,所以现在不能获得，在lotteryPet表里）
	auto notOwnedPets = PetManager::petMgr()->getNotOwnedPetIds();
	auto canOwnPets = getDifference(notOwnedPets, getPetsCanNotOwn());
	canOwnPets = getDifference(canOwnPets, m_selectedPetIds);//m_selectedPetIds 是当前抽奖已经预抽到的宠物，不能重复
	//bool canFreeGetPet = DataManagerSelf->getSystemConfig().lotteryCanGetPetStage <= curStage;
	bool canFreeGetPet = true;
	if (canOwnPets.empty() || !canFreeGetPet)
	{
		return kNoNewPets;
	}
	
	float allWeights = 0;
	for (size_t i = 0; i < canOwnPets.size(); ++i)
	{
		int commonId = PetManager::petMgr()->getPetById(canOwnPets[i])->getPetData().commonid;
		allWeights += DataManagerSelf->getPetCommonConfig(commonId).lotteryWeight;	
	}

	//计算宠物获取的百分比
	vector<float> percents;
	for (size_t i = 0; i < canOwnPets.size(); ++i)
	{
		int commonId = PetManager::petMgr()->getPetById(canOwnPets[i])->getPetData().commonid;
		float weight = DataManagerSelf->getPetCommonConfig(commonId).lotteryWeight / allWeights * 100.0f;
		percents.push_back(weight);
	}
	
	auto index = getResultByPercent(percents);
	return canOwnPets[index];	
}

void LotteryModel::getReward(const LotteryData &data)
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
}

LotteryData LotteryModel::buyOneBox()
{
	assert(canOpenOneBox());
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
	return getLotteryResult();
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

bool LotteryModel::canOpenOneBox()
{
	int key = UserInfo::theInfo()->getKey();
	int cost = DataManagerSelf->getSystemConfig().diamondsForOneKey;
	return key > 0 || UserInfo::theInfo()->hasEnoughDiamond(cost);
}

void LotteryModel::buyAllBox(std::function<void(std::vector<LotteryData>)> callback)
{
	m_selectedPetIds.clear();//保存当前已选的宠物id
	const int kOpenAllBoxesPurchaseId = 9;//计费点id
	const int kBoxNum = 9;
	MyPurchase::sharedPurchase()->buyItem(kOpenAllBoxesPurchaseId, [=]()
	{
		vector<LotteryData>datas;
		for (int i = 0; i < kBoxNum; ++i)
		{
			LotteryData data = getLotteryResult();
			datas.push_back(data);
			if (data.type == kLotteryPet)
			{
				if (!petSelected(data.param))
				{
					m_selectedPetIds.push_back(data.param);
				}
			}
		}
		//一键开箱必有有个宠物
		int petId = getOneNotOwndPetId();
		if (petId != kNoNewPets)
		{
			LotteryData data;
			data.type = kLotteryPet;
			data.param = petId;
			int index = CommonUtil::getRandomValue(0, kBoxNum - 1);
			datas[index] = data;		
		}
		m_selectedPetIds.clear();
		callback(datas);
	});
}

bool LotteryModel::petSelected(int petId)
{
	return find(m_selectedPetIds.begin(), m_selectedPetIds.end(), petId) != m_selectedPetIds.end();
}