#ifndef __DATACONFIG_H__
#define __DATACONFIG_H__

#include "cocos2d.h"
#include <vector>
#include <string.h>

using namespace std;
USING_NS_CC;

struct StarsConfig
{
	int id;
	string desc;
	int score;
	string resPath;
	string bornAnimationRes;
	string explosionRes;
	vector<int> linkStarTypes;
	vector<int> eraseTypes;
};

struct PetCommonConfig
{
	int id;
	vector<int> maxEnergy;
	vector<int> skillPower;
	vector<int> foodToUpgrade;
	int skillTarget;
	int maxLevel;
	std::string skillDescRes;
	string desc;
};

struct PetResConfig
{
	int id;
	int commonId;
	int color;
	string petImgRes;
	string skillRes;
	string petAnimationRes;
	string petNameRes;
};

struct PetColorConfig
{
	int id;
	string preStageSlotBg;
	string skillTitle;
	string skillLvLabel;
	string numRes;
};

struct StageConfig
{
	int id;
	int tagetType;
	int targetScore;
	vector<int> targetParam;
	int step;
	vector<int> direction;
	vector<int> recommendPets;
	vector<int> rewardWin;//饲料， 钻石，钥匙
	vector<int> rewardFail;//饲料， 钻石，钥匙
};

struct StarsLoaderConfig
{
	int id;
	int stageId;
	int starType;
	int score;
	int percent;
	int num;
};

struct SystemConfig
{
	int id;
	int stageAmount;
	int starPetEnergy;
	std::vector<int> runeStoneReward; //饲料， 钻石，钥匙
	int lotteryCanGetPetStage;
	int strengthMax;
	int strengthOneRound;
	int strengthAddMins;
	int diamondsForOneKey;
	int foodsByOneDiamond;
	int packagePetStage;//新手宠物礼包里 宠物在某关后出现
	std::vector<int> openingPets;
};

struct StarsColorConfig
{
	int id;
	std::string colorStarRes;
	std::string colorExplosionRes;
	std::string bounceBallRes;
	std::string bounceBallExplosionRes;
	std::string desc;
};

struct StageStarInfo
{
	int starType;
	int color;
};

struct PropsConfig
{
	int id;
	std::string resPath;
};

struct RankingConfig
{
	int id;
	std::string name;
	int stage;
	int ownPetPercent;
};

struct ShopConfig
{
	int id;
	int purchaseId;
	int diamond;
	std::string iconPath;
};

struct GoodsData
{
	int type;
	int goodsId;
	int amount;
	GoodsData(){}
	GoodsData(std::vector<int> datas): type(datas[0]), goodsId(datas[1]), amount(datas[2]){}
};

struct PackageConfig
{
	int id;
	std::vector<int> cost;
	std::vector<GoodsData> goods;
	std::string textPath;
};

struct LotteryPetConfig
{
	int id;
	int stage;
	int petId;
};

struct LotteryOutputConfig
{
	int id;
	int amount;
	float percent;
	std::string resPath;
	std::string desc;
};

struct GuideConfig
{
	int id;
	int stage;
	int startAction;
	int endAction;
	int param;
	bool showGuideView;
	int rectType;
	std::vector<float>targetRect;
	bool showMask;
	int fingerType;
	std::string dialogText;
	bool showTextDialog;
	std::string desc;
};

struct RewardsConfig
{
	int id;
	GoodsData rankingOverOpponent;
	GoodsData thiefReward;
	GoodsData redPackage;
};

struct ThiefConfig
{
	int id;
	int percent;
};

struct PurchaseConfig
{
	int id;
	int moneyCost;
};

struct SoundConfig
{
	int id;
	std::string background;
	std::string button;
	std::string bomb;
	std::string starErase;
};
#endif