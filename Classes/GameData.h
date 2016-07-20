#ifndef __GAMEDATA_H__
#define __GAMEDATA_H__
#include "cocos2d.h"
#include "ConfData.h"

#define USE_PRO_TYPE_COUNTANDCOIN 1
#define USE_PRO_TYPE_COUNTONLY 2
#define USE_PRO_TYPE_COINONLY 3

#define USE_PRO_TYPE_NORMAL 1
#define USE_PRO_TYPE_HARD 2
#define USE_PRO_TYPE_EASY 3
#define USE_PRO_TYPE_EASYTOHARD 4


class GameData {
public:
	static GameData* getInstance();
	GameData();
	~GameData();
	void preloadData();
	static std::string getDataFileName(const char* fileName);
	template <typename T> static void loadData(const char* fileName, T*& data, unsigned int& dataCount);
	const void loadGameData(std::string fileName);
	int *m_nPassScore;
	int m_nPassScoreSize;
	int m_nPassScoreBase;
	int m_nPassScoreUp;
	int m_nPassScoreMax;//分数增加上限
	int m_nPassScoreLevel;//多少关后达到分数上线
	int m_nPassScoreMaxBase;//达到上限后分数的基准分数
	int m_nFuHuoUseCoin;
	int m_nBombUseCoin;
	int m_nPaintUseCoin;
	int m_nReflashUseCoin;
	int m_nDefaultBombCount;
	int m_nDefaultPaintCount;
	int m_nDefaultReflashCount;
	int m_nDefaultCoin;
	int m_nUseProType;
	int *m_nCombo;
	int m_nComboSize;
	int m_nRadomPro;//某种颜色的星星额外随机出现的概率
	int m_nRadomUnSame;//出现不同颜色的概率
	bool m_bResetScore;
	int *m_nLibaoStage;
	int m_nLibaoStageSize;
	int m_nLibaoBase;
	float m_fLiBaoPer;
	int m_nHardType;
	int m_nHardPre;
	bool m_bResetHard;	
	int m_nLibaoMsgColorR;
	int m_nLibaoMsgColorG;
	int m_nLibaoMsgColorB;
	int m_nLoginGift[7][2];
	int m_bShowExitDig;
	int m_nShowQuickBtn;
private:
	static GameData* s_Instance;
};


#endif