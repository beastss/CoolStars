#include "StageOperator.h"
#include "PetManager.h"
#include "PetEntity.h"
#include "CommonUtil.h"
#include "LogicGridUtil.h"
#include "StageDataMgr.h"
#include "StageScene.h"
#include "StageUiLayer.h"
#include <algorithm>
#include "ActionRunner.h"
#include "StarsEraseModule.h"
#include "MainScene.h"
#include "DataManager.h"
#include <algorithm>
using namespace std;
using namespace CommonUtil;
USING_NS_CC; 

StageOperator::StageOperator()
{
	m_runner = ActionRunner::create();
	m_runner->retain();
}

StageOperator::~StageOperator()
{
	m_runner->clear();
	m_runner->release();
}

//道具和宠物对stars的操作
StageOperator *StageOperator::theOperator()
{
	static StageOperator op;
	return &op;
}

void StageOperator::eraseStars(vector<LogicGrid> grids)
{
	for (size_t i = 0; i < grids.size(); ++i)
	{
		auto node = StarsController::theModel()->getStarNode(grids[i]);
		if (node && node->canBeRemoved())
		{
			node->doRemove();
		}
	}
	StarsController::theModel()->genNewStars();
}

void StageOperator::petScaleErase(int petId, const LogicGrid &center, int xRadius, int yRadius)
{
	vector<LogicGrid> grids;
	grids.push_back(center);
	StageLayersMgr::theMgr()->petSpreadStar(petId, grids, [=]()
	{
		StarsEraseModule::theModel()->scaleErase(center, xRadius, yRadius);
	});
}

void StageOperator::petRandomErase(int petId, int num)
{
	auto targetGrids = getRandomColorGrids(num);
	StageLayersMgr::theMgr()->petSpreadStar(petId, targetGrids, [=]()
	{
		StarsEraseModule::theModel()->listErase(targetGrids);

	});
}

void StageOperator::addSteps(int amount)
{
	auto stageInfo = StageDataMgr::theMgr();
	int curStep = stageInfo->getCurStep();
	stageInfo->setCurStep(curStep - amount);
}

void StageOperator::changeColor(const StarAttr &attr)
{
	StarsController::theModel()->replaceStar(attr);
}

void StageOperator::addPetEnergy(int petId, int value)
{
	PetManager::petMgr()->addPetEnergy(petId, value);
}

void StageOperator::removePetDebuff(int who)
{

}

void StageOperator::chageStarType(int type)
{
	StarAttr attr;
	//init
	StarsController::theModel()->replaceStar(attr);
}

void StageOperator::reOrderStars(std::function<void()> callback)
{
	MainScene::theScene()->showTips(DataManagerSelf->getText("no_link_stars").c_str(), true, [=]()
	{
		auto nodes = StarsController::theModel()->getStarNodes();

		vector<LogicGrid> targetGrids;
		vector<StarNode *> colorNodes;
		for (size_t i = 0; i < nodes.size(); ++i)
		{
			auto starType = nodes[i]->getAttr().type;
			auto grid = nodes[i]->getAttr().grid;
			if (starType == kColorStar)
			{
				colorNodes.push_back(nodes[i]);
				targetGrids.push_back(grid);
			}
		}
		assert(targetGrids.size() == colorNodes.size());
		auto seq = buildRandomSequence(colorNodes.size());

		for (size_t i = 0; i < colorNodes.size(); ++i)
		{
			auto node = colorNodes[i];
			int index = seq[i];
			LogicGrid targetGrid = targetGrids[index];

			node->moveTo(targetGrid);
			node->setLogicGrid(targetGrid);
		}
		if(callback) callback();
	});

}

void StageOperator::randomReplaceStars(int petId, int starType, int color, int num)
{
	auto targetGrids = getRandomColorGrids(num, true, color);
	StageLayersMgr::theMgr()->petSpreadStar(petId, targetGrids, [=]()
	{
		for (size_t i = 0; i < targetGrids.size(); ++i)
		{
			auto star = StarsController::theModel()->getStarNode(targetGrids[i]);
			if (star)
			{
				StarAttr targetStarAttr = star->getAttr();
				targetStarAttr.color = color;
				targetStarAttr.type = starType;
				StarsController::theModel()->replaceStar(targetStarAttr);
			}
		}
		StarsController::theModel()->preOneRound();
	});
}

void StageOperator::gameOverRandomReplace()
{
	int leftSteps = StageDataMgr::theMgr()->getLeftSteps();
	auto targetGrids = getRandomActiveGrids(leftSteps);
	GameResultReward rewardBonus;
	float kMaxDuration = 0.5f;
	float avgDuration = 3.0f / targetGrids.size();
	float duration = min(avgDuration, kMaxDuration);
	for (size_t i = 0; i < targetGrids.size(); ++i)
	{
		auto star = StarsController::theModel()->getStarNode(targetGrids[i]);
		if (star)
		{
			vector<float> percents;
			auto configs = DataManagerSelf->getGameWinBonusConfigs();
			for (size_t j = 0; j < configs.size(); ++j)
			{
				percents.push_back(configs[j].percent);
			}
			int index = getResultByPercent(percents);
			GoodsData data;
			data.type = configs[index].goodsType;
			data.amount = configs[index].amount;

			switch (data.type)
			{
			case kGoodsDiamond:
				rewardBonus.diamond += data.amount;
				break;
			case kGoodsFood:
				rewardBonus.food += data.amount;
				break;
			case kGoodsKey:
				rewardBonus.key += data.amount;
				break;
			}
	
			//步数变成钻石或者饲料
			m_runner->queueAction(CallFuncAction::withFunctor([=]()
			{
				auto uiLayer = StageScene::theScene()->getStageUiLayer();
				auto temp = targetGrids[i];
				
				uiLayer->gameOverSpreadStars(data, targetGrids[i], [=]()
				{
					StarsEraseModule::theModel()->removeStar(temp);
					StageDataMgr::theMgr()->addStep();
				});
			}));
			m_runner->queueAction(DelayAction::withDelay(duration));
		}
	}

	m_runner->queueAction(CallFuncAction::withFunctor([=]()
	{
		StageDataMgr::theMgr()->addResultBonus(rewardBonus);
		StarsController::theModel()->gameOver(true);
	}));
}

void StageOperator::loadDesignatedStar(int color, int rounds)
{
	StarsController::theModel()->loadDesignatedStar(kColorStar, color, rounds);
}

std::vector<LogicGrid> StageOperator::getRandomColorGrids(int num, bool hasExceptColor, int exceptColor)
{
	vector<LogicGrid> colorGrids;
	auto nodes = StarsController::theModel()->getStarNodes();

	for (size_t i = 0; i < nodes.size(); ++i)
	{
		auto attr = nodes[i]->getAttr();
		if (attr.type == kColorStar)
		{
			if (hasExceptColor && exceptColor == attr.color) continue;
			colorGrids.push_back(attr.grid);
		}
	}

	vector<LogicGrid> grids;
	auto seq = buildRandomSequence(colorGrids.size());
	int size = min(num, (int)seq.size());
	for (int i = 0; i < size; ++i)
	{
		grids.push_back(colorGrids[seq[i]]);
	}
	return grids;
}

std::vector<LogicGrid> StageOperator::getRandomActiveGrids(int num)
{
	vector<LogicGrid> colorGrids;
	auto nodes = StarsController::theModel()->getStarNodes();

	for (size_t i = 0; i < nodes.size(); ++i)
	{
		auto attr = nodes[i]->getAttr();
		if (attr.type != kEmpty)
		{
			colorGrids.push_back(attr.grid);
		}
	}

	vector<LogicGrid> grids;
	auto seq = buildRandomSequence(colorGrids.size());
	int size = min(num, (int)seq.size());
	for (int i = 0; i < size; ++i)
	{
		grids.push_back(colorGrids[seq[i]]);
	}
	return grids;
}