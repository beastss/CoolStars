#ifndef __LOTTERYMODEL_H__
#define __LOTTERYMODEL_H__
#include "cocos2d.h"
enum LotteryReward
{
	kLotteryLittleStrength,
	kLotteryBagStrength,
	kLotteryLittleDiamond,
	kLotteryBagDiamond,
	kLotteryLittleFood,
	kLotteryBagFood,
	kLotteryPet = 6, //对应LotteryOutput表
};
#define MaxStrength 50  //体力最大值
#define StageStrengthConsum 5 //每局消耗体力
struct LotteryData
{
	int type;
	int param;
};

class LotteryModel
{
public:
	static LotteryModel *theModel();
	bool canOpenOneBox();
	LotteryData buyOneBox();

	void buyAllBox(std::function<void(std::vector<LotteryData>)> callback);
	void getReward(const LotteryData &data);
private:
	LotteryData getLotteryResult();
	std::vector<int> getPetsCanNotOwn();
	int getOneNotOwndPetId();
	bool petSelected(int petId);
private:
	static const int kNoNewPets = -1;
	std::vector<int> m_selectedPetIds;
	
};
#endif