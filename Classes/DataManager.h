#ifndef __DATAMANAGER__
#define __DATAMANAGER__

#include "sqlite3.h"
#include "DataConfig.h"
#include "cocos2d.h"
#include "ConfData.h"

using namespace std;
USING_NS_CC;

#define DataManagerSelf DataManager::GetSelf() 

class DataManager
{
public:
	DataManager(void);
	virtual ~DataManager(void);
	DataManager* m_pSelf;

	static DataManager* GetSelf()
	{
		static DataManager mgr;
		return &mgr;
	}

	void LoadData();

public:
	void loadStageConfig();
	const StageConfig &getStageConfig(int stage);

	void loadStarsLoaderConfig();
	const std::vector<StarsLoaderConfig> getStarsLoaderConfig(int stage);

	void getNewStageStarsData(std::vector< std::vector<StageStarInfo> > &, int stageNum);

	void loadStarsConfig();
	const StarsConfig &getStarsConfig(int starType);

	void loadStarsColorConfig();
	const StarsColorConfig &getStarsColorConfig(int color);

	void loadGameWinBonusConfig();
	const vector<GameWinBonusConfig> &getGameWinBonusConfigs();

	void loadPetCommonConfig();
	const PetCommonConfig &getPetCommonConfig(int petCommonId);

	void loadPetResConfig();
	const PetResConfig &getPetResConfig(int petId);
	vector<int> getOpeningPetIds();

	void loadPetColorConfig();
	const PetColorConfig &getPetColorConfig(int color);

	void loadPetPurchaseConfig();
	const PetPurchaseConfig &getPetPurchaseConfig();

	void loadSystemConfig();
	const SystemConfig &getSystemConfig();

	void loadPropsConfig();
	const PropsConfig &getPropsConfig(int propsId);

	void loadRankingConfig();
	const std::vector<RankingConfig> &getRankingConfigs();
	const RankingConfig &getRankingConfig(int id);

	void loadShopConfig();
	const ShopConfig &getShopConfig(int id);
	const std::vector<ShopConfig> &getShopConfigs();

	void loadPackageConfig();
	const PackageConfig &getPackageConfig(int type);

	void loadLotteryPet();
	const std::vector<LotteryPetConfig> &getLotteryPetConfig();
	const LotteryPetConfig *getLotteryPetConfigByStage(int stage);

	void loadLotteryOutput();
	const LotteryOutputConfig &getLotteryOutputConfig(int id);
	const std::vector<LotteryOutputConfig> &getLotteryOutputConfigs();

	void loadGuideConfig();
	const std::vector<GuideConfig> &getGuideConfig();
	const GuideConfig *getGuideConfigById(int guideId);

	void loadRewardsConfig();
	const RewardsConfig &getRewardsConfig();

	void loadThiefConfig();
	const std::vector<ThiefConfig> &getThiefConfigs();

	void loadPurchase();
	const std::vector<PurchaseConfig> &getPurchaseConfigs();
	const PurchaseConfig &getPurchaseConfig(int id);

	void loadSound();
	const SoundConfig &getSoundConfig();
private:
	vector<StageConfig> m_stagesConfig;
	vector<StarsConfig> m_starsConfig;
	vector<StarsLoaderConfig> m_starsLoaderConfig;
	vector<PetCommonConfig> m_petCommonConfig;
	vector<PetResConfig> m_petResConfig;
	vector<PetColorConfig> m_petColorConfig;
	PetPurchaseConfig m_petPurchaseConfig;
	vector<StarsColorConfig> m_starsColorConfig;
	SystemConfig m_systemConfig;
	vector<PropsConfig> m_propsConfig;
	vector<RankingConfig> m_rankingConfig;
	vector<ShopConfig> m_shopConfig;
	vector<PackageConfig> m_packageConfig;
	vector<LotteryPetConfig> m_lotteryPetConfig;
	vector<LotteryOutputConfig> m_lotteryOutputConfig;
	vector<GuideConfig> m_guideConfig;
	RewardsConfig m_rewardsConfig;
	vector<ThiefConfig> m_thiefConfig;
	vector<PurchaseConfig> m_purchaseConfig;
	SoundConfig m_soundConfig;
	vector<GameWinBonusConfig> m_gameWinBonusConfig;
};

#endif