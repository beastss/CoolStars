#ifndef __RANKINGMODEL_H__
#define __RANKINGMODEL_H__
#include "cocos2d.h"
#include "RankingOpponent.h"
#include <map>
enum RankingType
{
	kPlayer,
	kOpponent,
	kData,
};

struct RankingData
{
	int type;
	std::string name;
	int stage;
	int ownPetPercent;
	
	bool operator >(RankingData data) const
	{
		if (stage == data.stage)
		{
			return ownPetPercent > data.ownPetPercent;
		}
		else
		{
			return stage > data.stage;
		}
	}
	
};

class RankingModel
{
public:
	static RankingModel *theModel();
	void init();

	std::string getRandomName();
	std::string getRandomCHName();
	bool isValidName(std::string name);
	
	bool alreadyOpenRanking(){ return m_alreadyOpenRanking; }
	void initFirstOpenRanking(std::string myName);
	bool isOverOpponent();
	void getOverOpponentReward();
	std::map<int, RankingData> getNeighboursRanking();
	RankingData getMyRankingData();
	RankingData getDataByRank(int rank);
	int getOpponetRank();
	int getMyRank();
	std::vector<RankingData> getCurRanking();
private:
	RankingModel();
	void loadData();
private:
	static const int kMaxLength = 4;
	bool m_alreadyOpenRanking;
	int m_daysOverOpponent;
	std::string m_myName;
};
#endif