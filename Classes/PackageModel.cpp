#include "PackageModel.h"
#include "DataManager.h"
#include "CommonMacros.h"
#include "UserInfo.h"
#include "GoodsMgr.h"
#include "MyPurchase.h"
#include "SqliteHelper.h"
#include "StageDataMgr.h"
#include "PetManager.h"
USING_NS_CC;
using namespace std;

PackageModel *PackageModel::theModel()
{
	static PackageModel model;
	return &model;
}

bool PackageModel::buyPackage(int id, function<void()> callback)
{
	//���üƷ�sdk
	auto config = DataManagerSelf->getPackageConfig(id);
	int consumeType = config.cost[0];
	int param = config.cost[1];
	if (consumeType == kConsumeDiamond)
	{
		bool succeed = UserInfo::theInfo()->consumeDiamond(param);//paramΪ���ĵ���ʯ��
		if (!succeed) return false;
		onBuyPackageSucceed(config.goods, callback);
	}
	else
	{
		//param Ϊ�Ʒѵ�
		MyPurchase::sharedPurchase()->buyItem(param, [=]()
		{
			onBuyPackageSucceed(config.goods, callback);
		});
	}
	return true;
}

void PackageModel::onBuyPackageSucceed(std::vector<GoodsData> goods, function<void()> callback)
{
	GoodsMgr::theMgr()->addGoods(goods);
	if (callback)
	{
		callback();
	}
}

int PackageModel::getPetPacakagePetId()
{
	auto config = DataManagerSelf->getPackageConfig(kPackagePetFirstGet);
	return config.goods[0].goodsId;
}

bool PackageModel::canBuyPetPackage()
{
	bool ownThisPet = PetManager::petMgr()->ownedThisPet(getPetPacakagePetId());
	
	int minStage = DataManagerSelf->getSystemConfig().packagePetStage;
	int topStage = StageDataMgr::theMgr()->getTopStage();
	return !ownThisPet && minStage <= topStage;

}

int PackageModel::getPackageCost(int id)
{
	//���üƷ�sdk
	auto config = DataManagerSelf->getPackageConfig(id);
	int consumeType = config.cost[0];
	int param = config.cost[1];
	if (consumeType == kConsumeDiamond)
	{
		return param;
	}
	else
	{
		auto purchaseConfig = DataManagerSelf->getPurchaseConfig(param);
		return purchaseConfig.moneyCost;
	}
}
