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
	kLotteryPet = 6, //��ӦLotteryOutput��
};
#define MaxStrength 50  //�������ֵ
#define StageStrengthConsum 5 //ÿ����������
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
	void doLottery(const LotteryData &data, bool consume = true);//consume �Ƿ����Ļ���
	
private:
	std::vector<int> getPetsCanNotOwn();
	
};
#endif