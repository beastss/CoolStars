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
	LotteryData getLotteryResult();
	void doLottery(const LotteryData &data, bool consume = true);//consume 是否消耗货币
	
private:
	std::vector<int> getPetsCanNotOwn();
	
};
#endif