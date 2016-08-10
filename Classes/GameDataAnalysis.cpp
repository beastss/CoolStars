#include "GameDataAnalysis.h"
#include "DataManager.h"
#include "PropManager.h"
USING_NS_CC;
using namespace std;
GameDataAnalysis *GameDataAnalysis::theModel()
{
	static GameDataAnalysis model;
	return &model;
}

void GameDataAnalysis::consumeDiamond(int type, int param, int cost)
{
	string name;
	switch (type)
	{
	case kDiamondConsumePackage:
	{
		name = "tili_libao";
		break;
	}
	case kDiamondConsumeLottery:
	{
		name = "chou_jiang";
		break;
	}
	case kDiamondConsumePetUpgrade:
	{
		string petName = DataManagerSelf->getPetResConfig(param).name;
		name = "shengji_";
		name += petName;
	    break;
	}
	case kDiamondConsumeBuyPet:
	{
		string petName = DataManagerSelf->getPetResConfig(param).name;
	    name = "goumai_";
	    name += petName;
		break;
	}
	default:
		assert(false && "not valid item");
		break;
	}
	CCLOG("GameDataAnalysis::consumeDiamond name: %s cost: %d", name.c_str(), cost);
}

void GameDataAnalysis::consumeProps(int type)
{
	string name;
	switch (type)
	{
	case kPropReorder:
		name = "chong_pai";
		break;
	case kPropBrush:
		name = "shua_zi";
		break;
	case kPropBomb:
		name = "zha_dan";
		break;
	default:
		break;
	}
	CCLOG("GameDataAnalysis::consumeProps name: %s", name.c_str());

}