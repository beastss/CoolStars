#include "ShopModel.h"
#include "DataManager.h"
#include "MyPurchase.h"
#include "UserInfo.h"
USING_NS_CC;
using namespace std;

ShopModel *ShopModel::theModel()
{
	static ShopModel model;
	return &model;
}

void ShopModel::purchase(int id)
{
	//调用计费sdk
	MyPurchase::sharedPurchase()->buyItem(id, bind(&ShopModel::onPurchaseSucceed, this, id));
}

void ShopModel::onPurchaseSucceed(int id)
{
	auto data = DataManagerSelf->getShopConfig(id);
	auto theInfo = UserInfo::theInfo();
	theInfo->setDiamond(theInfo->getDiamond() + data.diamond);
}