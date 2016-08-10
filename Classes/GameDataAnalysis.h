#ifndef __GAMEDATAANALYSIS_H__
#define __GAMEDATAANALYSIS_H__
#include "cocos2d.h"
#include <unordered_map>
enum DiamondConsumeType
{
	kDiamondConsumePackage, //�������
	kDiamondConsumeLottery, //�齱
	kDiamondConsumePetUpgrade,//��������
	kDiamondConsumeBuyPet,//�������
	//kDiamondConsume
};

class GameDataAnalysis
{
public:
	static GameDataAnalysis *theModel();
	void consumeDiamond(int type, int param, int cost);
	void consumeProps(int type);
private:
	GameDataAnalysis(){}
private:
	std::string m_packageTag;
	std::string m_lotteryTag;
	std::unordered_map<int, std::string>m_buyPetTags;
	std::unordered_map<int, std::string>m_upgradePetTags;
};


#endif