#include "DataManager.h"
#include "SqliteHelper.h"
#include "StarNode.h"
#include "cocos2d.h"
#include "CommonUtil.h"
#include "PropManager.h"
#include <algorithm>
using namespace CommonUtil;
DataManager::DataManager(void)
{

}

DataManager::~DataManager(void)
{

}

void DataManager::LoadData()
{
	loadSystemConfig();
	loadStageConfig();
	loadStarsConfig();
	loadStarsColorConfig();
	loadPetCommonConfig();
	loadPetResConfig();
	loadPetColorConfig();
	loadPetPurchaseConfig();
	loadPropsConfig();
	loadRankingConfig();
	loadShopConfig();
	loadPackageConfig();
	loadLotteryPet();
	loadLotteryOutput();
	loadStarsLoaderConfig();
	loadGuideConfig();
	loadRewardsConfig();
	loadThiefConfig();
	loadPurchase();
	loadSound();
	loadGameWinBonusConfig();
}

void DataManager::loadStarsConfig()
{
	SqliteHelper helper(DB_CONFIG);
	auto result = helper.readRecord("select * from stars");
	for (auto iter = result.begin(); iter != result.end(); ++iter)
	{
		StarsConfig config;
		auto data = (*iter);
		assert(data.size() == 9);

		config.id = atoi(data[0]);
		config.desc = data[1];
		config.score = atoi(data[2]);
		config.bornAnimationRes = data[3];
		config.resPath = data[4];
		config.explosionRes = data[5];
		config.linkStarTypes = CommonUtil::parseStrToInts(data[6]);
		config.eraseTypes = CommonUtil::parseStrToInts(data[7]);
		config.canMove = atoi(data[8]) == 1;

		m_starsConfig.push_back(config);
	}
}

const StarsConfig &DataManager::getStarsConfig(int starType)
{
	assert(starType >= kEmpty && starType < kStarTypeCount);
	if (starType >= kEmpty && starType < kStarTypeCount)
	{
		return m_starsConfig[starType];
	}
	else
	{
		return m_starsConfig[0];
	}
}

void DataManager::loadPetCommonConfig()
{
	SqliteHelper helper(DB_CONFIG);
	auto result = helper.readRecord("select * from pets_common");

	for (auto iter = result.begin(); iter != result.end(); ++iter)
	{
		PetCommonConfig config;

		auto data = *iter;
		config.id = atoi(data[0]);
		config.maxLevel = atoi(data[1]);
		config.maxEnergy = parseStrToInts(data[2]);
		assert(config.maxEnergy.size() == config.maxLevel);
		config.skillPower = parseStrToInts(data[3]);
		assert(config.skillPower.size() == config.maxLevel);
		config.foodToUpgrade = parseStrToInts(data[4]);
		assert(config.foodToUpgrade.size() == config.maxLevel);
		config.skillTarget = atoi(data[5]);
		config.skillDescRes = data[6];
		config.desc = data[7];
		m_petCommonConfig.push_back(config);
	}
}

const PetCommonConfig &DataManager::getPetCommonConfig(int petCommonId)
{
	assert(petCommonId > 0 && petCommonId <= COMMON_PETS_AMOUNT);
	return m_petCommonConfig[petCommonId - 1];
}

void DataManager::loadPetResConfig()
{
	SqliteHelper helper(DB_CONFIG);
	auto result = helper.readRecord("select * from pets_res");

	for (auto iter = result.begin(); iter != result.end(); ++iter)
	{
		PetResConfig config;

		auto data = *iter;
		config.id = atoi(data[0]);
		config.commonId = atoi(data[1]);
		config.color = atoi(data[2]);
		config.petImgRes = data[3];
		config.skillRes = data[4];
		config.petAnimationRes = data[5];
		config.petNameRes = data[6];
		config.isOpening = atoi(data[7]) == 1;
		config.name = data[8];
		m_petResConfig.push_back(config);
	}
	sort(m_petResConfig.begin(), m_petResConfig.end(), [=](PetResConfig config1, PetResConfig config2)->bool
	{
		return config1.id < config2.id;
	});
}

vector<int> DataManager::getOpeningPetIds()
{
	vector<int> petsId;
	for (auto iter = m_petResConfig.begin(); iter != m_petResConfig.end(); ++iter)
	{
		if (iter->isOpening)
		{
			petsId.push_back(iter->id);
		}
	}
	return petsId;
}

const PetResConfig &DataManager::getPetResConfig(int petId)
{
	auto openingPets = getOpeningPetIds();
	auto iter = find(openingPets.begin(), openingPets.end(), petId);
	assert(iter != openingPets.end());

	if (iter != openingPets.end())
	{
		auto resIter = find_if(m_petResConfig.begin(), m_petResConfig.end(), [=](PetResConfig config)->bool
		{
			return config.id == petId;
		});
		assert(resIter != m_petResConfig.end());
		if (resIter != m_petResConfig.end())
		{
			return *resIter;
		}
	}
	//失败则返回第一个宠物
	return m_petResConfig[0];
}

void DataManager::loadPetColorConfig()
{
	SqliteHelper helper(DB_CONFIG);
	auto result = helper.readRecord("select * from pets_color");

	for (auto iter = result.begin(); iter != result.end(); ++iter)
	{
		auto data = *iter;
		PetColorConfig config;
		config.id = atoi(data[0]);
		config.preStageSlotBg = data[1];
		config.skillTitle = data[2];
		config.skillLvLabel = data[3];
		config.numRes = data[4];

		m_petColorConfig.push_back(config);
	}
}

const PetColorConfig &DataManager::getPetColorConfig(int color)
{
	assert(color >= kColorRed && color <= kColorPurple);
	return m_petColorConfig[color - 1];
}

void DataManager::loadPetPurchaseConfig()
{
	SqliteHelper helper(DB_CONFIG);
	auto result = helper.readRecord("select * from pets_purchase");

	auto data = result[0];
	m_petPurchaseConfig.guidePets = parseStrToInts(data[1]);
	m_petPurchaseConfig.petDiamondCost = atoi(data[2]);
	m_petPurchaseConfig.packagePet = atoi(data[3]);
}

const PetPurchaseConfig &DataManager::getPetPurchaseConfig()
{
	return m_petPurchaseConfig;
}

void DataManager::loadStageConfig()
{
	SqliteHelper sqlHelper(DB_STAGE);
	auto result = sqlHelper.readRecord("select * from stages_config");
	assert(result.size() >= m_systemConfig.stageAmount);

	for (auto iter = result.begin(); iter != result.end(); ++iter)
	{
		auto rowData = *iter;
		StageConfig stage;
		stage.id = atoi(rowData[0]);
		stage.tagetType = atoi(rowData[1]);
		stage.targetScore = atoi(rowData[2]);
		stage.targetParam = CommonUtil::parseStrToInts(rowData[3]);
		stage.step = atoi(rowData[4]);
		stage.direction = CommonUtil::parseStrToInts(rowData[5]);
		stage.recommendPets = CommonUtil::parseStrToInts(rowData[6]);
		stage.rewardWin = CommonUtil::parseStrToInts(rowData[7]);
		stage.rewardFail = CommonUtil::parseStrToInts(rowData[8]);

		m_stagesConfig.push_back(stage);
	}
}

const StageConfig &DataManager::getStageConfig(int stage)
{
	assert(stage > 0 && stage <= m_systemConfig.stageAmount);
	return m_stagesConfig[stage - 1];
}

void DataManager::loadStarsLoaderConfig()
{
	SqliteHelper sqlHelper(DB_STAGE);
	auto result = sqlHelper.readRecord("select * from stars_loader");

	for (auto iter = result.begin(); iter != result.end(); ++iter)
	{
		auto data = *iter;
		StarsLoaderConfig config;
		config.id = atoi(data[0]);
		config.stageId = atoi(data[1]);
		config.starType = atoi(data[2]);
		config.score = atoi(data[3]);
		config.percent = atoi(data[4]);
		config.num = atoi(data[5]);

		m_starsLoaderConfig.push_back(config);
	}
}

const vector<StarsLoaderConfig> DataManager::getStarsLoaderConfig(int stage)
{
	vector<StarsLoaderConfig> datas;
	for (size_t i = 0; i < m_starsLoaderConfig.size(); ++i)
	{
		if (m_starsLoaderConfig[i].stageId == stage)
		{
			datas.push_back(m_starsLoaderConfig[i]);
		}
	}
	return datas;
}

void DataManager::getNewStageStarsData(std::vector<std::vector<StageStarInfo>> &stars, int stageNum)
{
	vector<StageConfig> m_stagesConfig;
	SqliteHelper sqlHelper(DB_STAGE);

	char str[100] = { 0 };
	sprintf(str, "select * from stage%d_stars", stageNum);
	auto result = sqlHelper.readRecord(str);

	assert(result.size() == ROWS_SIZE);
	for (auto iter = result.begin(); iter != result.end(); ++iter)
	{
		assert((*iter).size() == COlUMNS_SIZE);

		vector<StageStarInfo> oneRow;
		for (size_t i = 0; i < (*iter).size(); ++i)
		{
			StageStarInfo info;
			auto data = (*iter)[i];
			auto reInts = CommonUtil::parseStrToInts(data);
			info.starType = reInts[0];
			info.color = reInts[1];
			oneRow.push_back(info);
		}
		stars.push_back(oneRow);
	}
}

void DataManager::loadSystemConfig()
{
	SqliteHelper sqlHelper(DB_CONFIG);
	auto result = sqlHelper.readRecord("select * from system");
	assert(result.size() == 1);

	auto data = result[0];
	m_systemConfig.stageAmount = atoi(data[0]);
	m_systemConfig.starPetEnergy = atoi(data[1]);
	m_systemConfig.runeStoneReward = CommonUtil::parseStrToInts(data[2]);
	m_systemConfig.lotteryCanGetPetStage = atoi(data[3]);
	m_systemConfig.strengthMax = atoi(data[4]);
	m_systemConfig.strengthOneRound = atoi(data[5]);
	m_systemConfig.strengthAddMins = atoi(data[6]);
	m_systemConfig.diamondsForOneKey = atoi(data[7]);
	m_systemConfig.foodsByOneDiamond = atoi(data[8]);
	m_systemConfig.packagePetStage = atoi(data[9]);
}

const SystemConfig &DataManager::getSystemConfig()
{
	return m_systemConfig;
}

void DataManager::loadStarsColorConfig()
{
	SqliteHelper sqlHelper(DB_CONFIG);
	auto result = sqlHelper.readRecord("select * from stars_color");
	assert(!result.empty());

	for (auto iter = result.begin(); iter != result.end(); ++iter)
	{
		auto data = *iter;
		StarsColorConfig config;
		config.id = atoi(data[0]);
		config.colorStarRes = data[1];
		config.colorExplosionRes = data[2];
		config.bounceBallRes = data[3];
		config.bounceBallExplosionRes = data[4];
		config.desc = data[5];

		m_starsColorConfig.push_back(config);
	}
}

const StarsColorConfig &DataManager::getStarsColorConfig(int color)
{
	assert(color > kColorRandom && color <= kColorPurple);
	return m_starsColorConfig[color];
}

void DataManager::loadGameWinBonusConfig()
{
	SqliteHelper sqlHelper(DB_STAGE);
	auto result = sqlHelper.readRecord("select * from game_win_bonus");
	assert(!result.empty());

	for (auto iter = result.begin(); iter != result.end(); ++iter)
	{
		auto data = *iter;
		GameWinBonusConfig config;
		config.id = atoi(data[0]);
		config.goodsType = atoi(data[1]);
		config.percent = atoi(data[2]);
		config.amount = atoi(data[3]);

		m_gameWinBonusConfig.push_back(config);
	}
}

const vector<GameWinBonusConfig> &DataManager::getGameWinBonusConfigs()
{
	return m_gameWinBonusConfig;
}

void DataManager::loadPropsConfig()
{
	SqliteHelper sqlHelper(DB_CONFIG);
	auto result = sqlHelper.readRecord("select * from props");
	for (auto iter = result.begin(); iter != result.end(); ++iter)
	{
		auto data = *iter;
		PropsConfig config;
		config.id = atoi(data[0]);
		config.resPath = data[1];
		m_propsConfig.push_back(config);
	}
}

const PropsConfig &DataManager::getPropsConfig(int propsId)
{
	assert(0 <= propsId && propsId < kPorpTypeAmount);
	return m_propsConfig[propsId];
}

void DataManager::loadRankingConfig()
{
	SqliteHelper sqlHelper(DB_CONFIG);
	auto result = sqlHelper.readRecord("select * from ranking");
	for (auto iter = result.begin(); iter != result.end(); ++iter)
	{
		auto data = *iter;
		RankingConfig config;
		config.id = atoi(data[0]);
		config.name = data[1];
		config.stage = atoi(data[2]);
		config.ownPetPercent = atoi(data[3]);
		m_rankingConfig.push_back(config);
	}

	sort(m_rankingConfig.begin(), m_rankingConfig.end(), [=](RankingConfig data1, RankingConfig data2)->bool
	{
		if (data1.stage > data2.stage) return true;
		else if (data1.stage == data2.stage)
		{
			return data1.ownPetPercent > data2.ownPetPercent;
		}
		return false;
	});
}

const RankingConfig &DataManager::getRankingConfig(int id)
{
	assert(id > 0 && id <= (int)m_rankingConfig.size());
	return m_rankingConfig[id - 1];
}

const std::vector<RankingConfig> &DataManager::getRankingConfigs()
{
	return m_rankingConfig;
}

void DataManager::loadShopConfig()
{
	SqliteHelper sqlHelper(DB_CONFIG);
	auto result = sqlHelper.readRecord("select * from shop");
	for (auto iter = result.begin(); iter != result.end(); ++iter)
	{
		auto data = *iter;
		ShopConfig config;
		config.id = atoi(data[0]);
		config.purchaseId = atoi(data[1]);
		config.diamond = atoi(data[2]);
		config.iconPath = data[3];
		m_shopConfig.push_back(config);
	}
}

const ShopConfig &DataManager::getShopConfig(int id)
{
	assert(id > 0 && id <= m_shopConfig.size());
	return m_shopConfig[id - 1];
}

const std::vector<ShopConfig> &DataManager::getShopConfigs()
{
	return m_shopConfig;
}

void DataManager::loadPackageConfig()
{
	SqliteHelper sqlHelper(DB_CONFIG);
	auto result = sqlHelper.readRecord("select * from package");
	for (auto iter = result.begin(); iter != result.end(); ++iter)
	{
		auto data = *iter;
		PackageConfig config;
		config.id = atoi(data[0]);
		config.cost = parseStrToInts(data[1]);
		config.goods = getGoodsDatas(data[2]);
		config.textPath = data[3];
		m_packageConfig.push_back(config);
	}
}

const PackageConfig &DataManager::getPackageConfig(int type)
{
	assert(type >= 0 && type < (int)m_packageConfig.size());
	return m_packageConfig[type];
}

void DataManager::loadLotteryPet()
{
	SqliteHelper sqlHelper(DB_CONFIG);
	auto result = sqlHelper.readRecord("select * from lottery_pet");
	for (auto iter = result.begin(); iter != result.end(); ++iter)
	{
		auto data = *iter;
		LotteryPetConfig config;
		config.id = atoi(data[0]);
		config.stage = atoi(data[1]);
		config.petId = atoi(data[2]);

		m_lotteryPetConfig.push_back(config);
	}
	sort(m_lotteryPetConfig.begin(), m_lotteryPetConfig.end(), [=](LotteryPetConfig config1, LotteryPetConfig config2)->bool
	{
		return config1.stage < config2.stage;
	});
}

const LotteryPetConfig *DataManager::getLotteryPetConfigByStage(int stage)
{
	for (auto iter = m_lotteryPetConfig.begin(); iter != m_lotteryPetConfig.end(); ++iter)
	{
		if (iter->stage == stage)
		{
			return &(*iter);
		}
	}
	return NULL;
}

const std::vector<LotteryPetConfig> &DataManager::getLotteryPetConfig()
{
	return m_lotteryPetConfig;
}

void DataManager::loadLotteryOutput()
{
	SqliteHelper sqlHelper(DB_CONFIG);
	auto result = sqlHelper.readRecord("select * from lottery_output");
	for (auto iter = result.begin(); iter != result.end(); ++iter)
	{
		auto data = *iter;
		LotteryOutputConfig config;
		config.id = atoi(data[0]);
		config.amount = atoi(data[1]);
		config.percent = atof(data[2]);
		config.resPath = data[3];
		config.desc = data[4];

		m_lotteryOutputConfig.push_back(config);
	}
}

const LotteryOutputConfig &DataManager::getLotteryOutputConfig(int id)
{
	assert(id >= 0 && id < m_lotteryOutputConfig.size());
	return m_lotteryOutputConfig[id];
}

const vector<LotteryOutputConfig> &DataManager::getLotteryOutputConfigs()
{
	return m_lotteryOutputConfig;
}

void DataManager::loadGuideConfig()
{
	SqliteHelper sqlHelper(DB_CONFIG);
	auto result = sqlHelper.readRecord("select * from guide");
	for (auto iter = result.begin(); iter != result.end(); ++iter)
	{
		auto data = *iter;
		GuideConfig config;
		config.id = atoi(data[0]);
		config.stage = atoi(data[1]);
		config.startAction = atoi(data[2]);
		config.endAction = atoi(data[3]);
		config.param = atoi(data[4]);
		config.showGuideView = atoi(data[5]) == 1;
		config.rectType = atoi(data[6]);

		auto rectVec = parseStrToFloats(data[7]);
		assert(rectVec.size() == 4 || rectVec.empty());
		config.targetRect = rectVec;

		config.showMask = atoi(data[8]) == 1;
		config.fingerType = atoi(data[9]);
		config.showTextDialog = atoi(data[10]) == 1;
		config.dialogText = data[11];
		config.desc = data[12];

		m_guideConfig.push_back(config);
	}
}

const vector<GuideConfig> &DataManager::getGuideConfig()
{
	return m_guideConfig;
}

const GuideConfig *DataManager::getGuideConfigById(int guideId)
{
	auto iter = find_if(m_guideConfig.begin(), m_guideConfig.end(), [=](GuideConfig data)
	{
		return data.id == guideId;
	});
	assert(iter != m_guideConfig.end());

	return iter != m_guideConfig.end() ? &(*iter) : NULL;
}

void DataManager::loadRewardsConfig()
{
	SqliteHelper sqlHelper(DB_CONFIG);
	auto result = sqlHelper.readRecord("select * from rewards");
	assert(result.size() == 1);
	auto data = result[0];
	m_rewardsConfig.id = atoi(data[0]);
	m_rewardsConfig.rankingOverOpponent = parseStrToInts(data[1]);
	m_rewardsConfig.thiefReward = parseStrToInts(data[2]);
	m_rewardsConfig.redPackage = parseStrToInts(data[3]);
}

const RewardsConfig &DataManager::getRewardsConfig()
{
	return m_rewardsConfig;
}

void DataManager::loadThiefConfig()
{
	SqliteHelper sqlHelper(DB_CONFIG);
	auto result = sqlHelper.readRecord("select * from thief");

	for (int i = 0; i < result.size(); ++i)
	{
		auto data = result[i];
		ThiefConfig config;
		config.id = atoi(data[0]);
		config.percent = atoi(data[1]);
		m_thiefConfig.push_back(config);
	}
}

const vector<ThiefConfig> &DataManager::getThiefConfigs()
{
	return m_thiefConfig;
}


void DataManager::loadPurchase()
{
	SqliteHelper sqlHelper(DB_CONFIG);
	auto result = sqlHelper.readRecord("select * from purchase");

	for (int i = 0; i < result.size(); ++i)
	{
		auto data = result[i];
		PurchaseConfig config;
		config.id = atoi(data[0]);
		config.moneyCost = atof(data[1]);
		m_purchaseConfig.push_back(config);
	}
}

const std::vector<PurchaseConfig> &DataManager::getPurchaseConfigs()
{
	return m_purchaseConfig;
}

const PurchaseConfig &DataManager::getPurchaseConfig(int id)
{
	assert(id >= 0 && id < m_purchaseConfig.size());
	return m_purchaseConfig[id - 1];
}

void DataManager::loadSound()
{
	SqliteHelper sqlHelper(DB_CONFIG);
	auto result = sqlHelper.readRecord("select * from sound");
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	auto data = result[0];
#else
	auto data = result[1];
#endif

	m_soundConfig.id = atoi(data[0]);
	m_soundConfig.background = data[1];
	m_soundConfig.button = data[2];
	m_soundConfig.bomb = data[3];
	m_soundConfig.starErase = data[4];
}

const SoundConfig &DataManager::getSoundConfig()
{
	return m_soundConfig;
}