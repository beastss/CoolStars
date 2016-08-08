#ifndef __PACKAGEMODEL_H__
#define __PACKAGEMODEL_H__
#include "cocos2d.h"
#include <functional>
#include "DataConfig.h"

enum PackageType
{
	kPackageStrength,
	kPackageDiamond,
	kPackageStep,
	kPackagePetFirstGet,//新手宠物礼包
	kPackagePetRecommend,//宠物推荐礼包
	kPackageProps,
	kPackPause,
};

class PackageModel
{
public:
	static PackageModel *theModel();
	bool buyPackage(int id, std::function<void ()> callback);
	int getPackageCost(int id);

	//新手宠物礼包只能购买一次 要保存状态
	int getPetPacakagePetId();//新手宠物礼包的宠物id
	bool canBuyPetPackage();
private:
	void onBuyPackageSucceed(std::vector<GoodsData> goods, std::function<void()> callback);
	PackageModel(){}
private:
};
#endif