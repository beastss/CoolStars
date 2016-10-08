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
	//��ָ���ؿ���,��Ȼ�鵽ĳ�����ӦlotteryPet��
	auto curStage = StageDataMgr::theMgr()->getTopStage() - 1;//��Ӧ��ǰһ��
	curStage = max(curStage, 1);
	auto lotteryPetConfig = DataManagerSelf->getLotteryPetConfigByStage(curStage);
	if (lotteryPetConfig && !PetManager::petMgr()->ownedThisPet(lotteryPetConfig->petId))
	{
		data.type = kLotteryPet;
		data.param = lotteryPetConfig->petId;
	}
	else
	{
		//���հٷֱȻ�ȡ�������ݣ���Ӧ��lotteryOutput��
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
			//δ��õĳ��� - ���ܻ�ȡ�ĳ������ؿ���ȻҪ��ȡ�ĳ���,�������ڲ��ܻ�ã���lotteryPet���
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

//���ܻ�ȡ�ĳ���id
std::vector<int> LotteryModel::getPetsCanNotOwn()
{
	vector<int> ids;
	auto curStage = StageDataMgr::theMgr()->getTopStage() - 1;
	curStage = max(1, curStage);
	//����ӵ�к���ؿ��鵽����
	auto config = DataManagerSelf->getLotteryPetConfig();
	for (auto iter = config.begin(); iter != config.end(); ++iter)
	{
		if (iter->stage > curStage)
		{
			ids.push_back(iter->petId);
		}
	}
	//����ӵ�����ֳ�������ĳ���

	int petId = PackageModel::theModel()->getPetPacakagePetId();
	if (find(ids.begin(), ids.end(), petId) == ids.end())
	{
		ids.push_back(petId);
	}
	return ids;
}