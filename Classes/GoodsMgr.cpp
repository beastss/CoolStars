#include "GoodsMgr.h"
#include "CommonMacros.h"
#include "UserInfo.h"
#include "PropManager.h"
#include "StarsController.h"
#include "StageDataMgr.h"
#include "PetManager.h"
using namespace std;
USING_NS_CC;

GoodsMgr *GoodsMgr::theMgr()
{
	static GoodsMgr mgr;
	return &mgr;
}

void GoodsMgr::addGoods(const GoodsData &data)
{
	int type = data.type;
	int goodsId = data.goodsId;
	int amount = data.amount;
	switch (type)
	{
		case kGoodsDiamond:
		{
		  int diamond = UserInfo::theInfo()->getDiamond();
		  UserInfo::theInfo()->setDiamond(diamond + amount);
		  break;
		}
		case kGoodsFood:
		{
			int food = UserInfo::theInfo()->getFood();
			UserInfo::theInfo()->setFood(food + amount);
			break;
		}
		case kGoodsStrength:
		{
			int strength = UserInfo::theInfo()->getStrength();
			UserInfo::theInfo()->setStrength(strength + amount);
			break;
		}
		case kGoodsRuneStone:
		{
			int runeStone = UserInfo::theInfo()->getRuneStone();
			UserInfo::theInfo()->setRuneStone(runeStone + amount);
			break;
		}
		case kGoodsKey:
		{
			int key = UserInfo::theInfo()->getKey();
			UserInfo::theInfo()->setKey(key + amount);
			break;
		}
		case kGoodsProps:
		{
			int prop = PropManager::propMgr()->getPropItemAmount(goodsId);
			PropManager::propMgr()->setPropItemAmount(goodsId, prop + amount);
			break;
		}
		case kGoodsPets:
		{
			PetManager::petMgr()->addNewPet(goodsId);
			break;
		}
		case kGoodsStep:
		{
			auto stageInfo = StageDataMgr::theMgr();
			int step = stageInfo->getCurStep() - amount;
			stageInfo->setCurStep(step);
		}
		default:
			break;
	}
}

void GoodsMgr::addGoods(const std::vector<GoodsData> &datas)
{
	for (size_t i = 0; i < datas.size(); ++i)
	{
		addGoods(datas[i]);
	}
}