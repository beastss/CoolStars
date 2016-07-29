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
	kPackagePetFirstGet,//���ֳ������
	kPackagePetRecommend,//�����Ƽ����
	kPackageProps,
	kPackPause,
};

class PackageModel
{
public:
	static PackageModel *theModel();
	bool buyPackage(int id, std::function<void ()> callback);
	int getPackageCost(int id);

	//���ֳ������ֻ�ܹ���һ�� Ҫ����״̬
	int getPetPacakagePetId();//���ֳ�������ĳ���id
	bool canBuyPetPackage();
private:
	void onBuyPackageSucceed(std::vector<GoodsData> goods, std::function<void()> callback);
	PackageModel(){}
private:
};
#endif