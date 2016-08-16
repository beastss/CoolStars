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

void StageOperator::eraseSameColorStars(const LogicGrid &centerGrids, int distance)
{
	auto centerNode = StarsController::theModel()->getStarNode(centerGrids);
	auto grids = getSquareGrids(centerGrids, distance);

	vector<LogicGrid> targetGrids;
	for (size_t i = 0; i < grids.size(); ++i)
	{
		auto node = StarsController::theModel()->getStarNode(grids[i]);
		if (node && node->getAttr().color == centerNode->getAttr().color)
		{
			targetGrids.push_back(grids[i]);
		}
	}

	eraseStars(targetGrids);
}

void StageOperator::randomErase(int num)
{
	vector<LogicGrid> grids;
	for (size_t i = 0; i < ROWS_SIZE; ++i)
	{
		for (int j = 0; j < COlUMNS_SIZE; ++j)
		{
			grids.push_back(LogicGrid(j, i));
		}
	}

	auto targetGrids = getRandomGrids(grids, num);
	eraseStars(targetGrids);
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

void StageOperator::reOrderStars()
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
}

void StageOperator::randomReplaceStars(int petId, int starType, int color, int num)
{
	auto stars = StarsController::theModel()->getStarNodes();
	vector<LogicGrid> grids;
	for (size_t i = 0; i < stars.size(); ++i)
	{
		auto star = stars[i];
		auto attr = star->getAttr();
		if (attr.type == kColorStar)
		{
			grids.push_back(attr.grid);
		}
	}

	auto targetGrids = getRandomGrids(grids, num);
	for (size_t i = 0; i < targetGrids.size(); ++i)
	{
		auto star = StarsController::theModel()->getStarNode(targetGrids[i]);
		if (star)
		{
			StarAttr targetStarAttr = star->getAttr();
			targetStarAttr.color = color;
			targetStarAttr.type = starType;
			auto uiLayer = StageScene::theScene()->getStageUiLayer();
			uiLayer->showPetSpreadStarsAction(petId, targetStarAttr, [=]()
			{
				StarsController::theModel()->replaceStar(targetStarAttr);
			});
		}
	}
}

void StageOperator::gameOverRandomReplace()
{
	auto stars = StarsController::theModel()->getStarNodes();
	vector<LogicGrid> grids;
	for (int row = 0; row < ROWS_SIZE; row++)
	{
		for (int col = 0; col < COlUMNS_SIZE; col++)
		{
			grids.push_back(LogicGrid(col, row));
		}
	}
	
	int leftSteps = StageDataMgr::theMgr()->getLeftSteps();
	auto targetGrids = getRandomGrids(grids, leftSteps);
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
			vector<string> paths = { "common/title_diamond.png", "common/title_food.png" };
			
			//红包30%获得钻石，70%获取饲料
			percents.push_back(30);
			percents.push_back(70);

			const int kDiamondIndex = 0;
			const int kFoodIndex = 1;
			GoodsData data;
			data.amount = 1;
			int index = getResultByPercent(percents);
			string rewardPath = paths[index];
			if (index == kDiamondIndex)
			{
				data.type = kGoodsDiamond;
				rewardBonus.diamond++;
			}
			else
			{
				data.type = kGoodsFood;
				rewardBonus.food++;
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
		StageDataMgr::theMgr()->setResultBonus(rewardBonus);
		StarsController::theModel()->gameOver(true);
	}));
}

void StageOperator::loadDesignatedStar(int color, int rounds)
{
	StarsController::theModel()->loadDesignatedStar(kColorStar, color, rounds);
}

