#include "GameData.h"

#define ARR_SIZE(arr) (sizeof(arr)/sizeof(arr[0]))
#define RETURN_ARR_ELEMENT(arr, i) do {if (i < arr##Count) return &arr[i]; return NULL;} while (0);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#define DATA_FILE(n) "data/data_"#n".json"
#else
#define DATA_FILE(n) "data/data_"#n".json"
#endif
GameData* GameData::s_Instance = NULL;

GameData::GameData():
m_nPassScoreSize(0)
,m_nPassScore(NULL)
,m_nDefaultCoin(0)
,m_nFuHuoUseCoin(5)
,m_nBombUseCoin(5)
,m_nPaintUseCoin(5)
,m_nReflashUseCoin(5)
,m_nDefaultBombCount(0)
,m_nDefaultPaintCount(0)
,m_nDefaultReflashCount(0)
,m_nUseProType(USE_PRO_TYPE_COINONLY)
,m_nHardType(USE_PRO_TYPE_NORMAL)
,m_nHardPre(1)
,m_nPassScoreBase(2000)
,m_nPassScoreUp(20)
,m_nCombo(NULL)
,m_nComboSize(0)
,m_nRadomPro(0)
,m_nRadomUnSame(0)
,m_nPassScoreMax(4000)
,m_nPassScoreLevel(-1)
,m_nPassScoreMaxBase(0)
,m_bResetScore(false)
,m_nLibaoStage(NULL)
,m_nLibaoStageSize(0)
,m_nLibaoBase(0)
,m_fLiBaoPer(0)
,m_nLibaoMsgColorR(4)
,m_nLibaoMsgColorG(20)
,m_nLibaoMsgColorB(65)
,m_bShowExitDig(1)
,m_nShowQuickBtn(1)
{
	memset(m_nLoginGift,0,sizeof(int)*7*2);
}

GameData* GameData::getInstance(){
	if (NULL == s_Instance) {
		s_Instance = new GameData();
	}
	return s_Instance;
}


std::string GameData::getDataFileName( const char* fileName ){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	return "data_"+std::string(fileName)+".json";
#else
    return "/data/data_"+std::string(fileName)+".json";
#endif
}

void GameData::preloadData(){
	loadGameData(DATA_FILE(config));
}

template <typename T>
void GameData::loadData( const char* fileName, T*& data, unsigned int& dataCount){
	unsigned long bufSize = 0;
	unsigned char* buf = CCFileUtils::sharedFileUtils()->getFileData(fileName, "r", &bufSize);
	buf[bufSize] = '\0';
	Document root;
	root.Parse<0>((const char*)buf);
	if (!root.HasParseError()) {
		const Value &rootValue = root;
		dataCount = rootValue.Size();
		data = new T[dataCount];
		for (unsigned int i = 0; i < dataCount; i++) {
			data[i].Load(rootValue[i]);
			data[i].idx = i;
		}
	}

	delete buf;
}

GameData::~GameData(){
	CC_SAFE_FREE(m_nPassScore);
	CC_SAFE_FREE(m_nCombo);
}

const void GameData::loadGameData(std::string fileName){
	unsigned long bufSize = 0;
	unsigned char* buf = CCFileUtils::sharedFileUtils()->getFileData(fileName.c_str(), "r", &bufSize);
	Document root;
	buf[bufSize] = '\0';
	root.Parse<0>((const char*)buf);
	if (!root.HasParseError()) {
		//过关分数
		Value &value = root["score"];
		if (value.IsArray()){
			int size = value.Size();
			m_nPassScoreSize = size;
			m_nPassScore = (int *) malloc(size * sizeof(int));			
			for (int i = 0; i < size; i++){
				m_nPassScore[i] = value[i].GetInt();
			}
		}
		m_nPassScoreBase = root["base"].GetInt();
		if (m_nPassScoreBase < 20){
			m_nPassScoreBase = 20;
		}
		m_nPassScoreUp = root["up"].GetInt();
		if (m_nPassScoreUp < 0){
			m_nPassScoreUp = 0;
		}
		m_nDefaultCoin = root["default"].GetInt();
		m_nFuHuoUseCoin = root["use"].GetInt();
		value = root["combo"];
		if (value.IsArray()){
			int size = value.Size();
			m_nComboSize = size;
			m_nCombo = (int *) malloc(size * sizeof(int));			
			for (int i = 0; i < size; i++){
				m_nCombo[i] = value[i].GetInt();
			}
		}
		m_nRadomPro = root["ex"].GetDouble();
		if (m_nRadomPro > 70){
			m_nRadomPro = 70;
		}
		m_bResetScore = root["resetscore"].GetBool();
		m_nRadomUnSame = root["unex"].GetDouble();
		if (m_nRadomUnSame > 70){
			m_nRadomUnSame = 70;
		}
		m_nPassScoreMax = root["max"].GetInt();
		if (m_nPassScoreMax > m_nPassScoreBase && m_nPassScoreUp > 0){
			m_nPassScoreLevel = (m_nPassScoreMax - m_nPassScoreBase) / m_nPassScoreUp;
			if (m_nPassScoreLevel == 0){
				m_nPassScoreLevel = 1;
			}
			m_nPassScoreMaxBase = m_nPassScore[m_nPassScoreSize - 1] + 
				(m_nPassScoreBase + m_nPassScoreUp + (m_nPassScoreBase + m_nPassScoreLevel * m_nPassScoreUp)) * m_nPassScoreLevel / 2;
		}
		m_nBombUseCoin = root["bomuse"].GetInt();
		m_nPaintUseCoin = root["paiuse"].GetInt();
		m_nReflashUseCoin = root["flsuse"].GetInt();
		m_nDefaultBombCount = root["bomdef"].GetInt();
		m_nDefaultPaintCount = root["paidef"].GetInt();
		m_nDefaultReflashCount = root["flsdef"].GetInt();
		m_nUseProType = root["protype"].GetInt();
		value = root["libao"];
		if (value.IsArray()){
			int size = value.Size();
			m_nLibaoStageSize = size;
			m_nLibaoStage = (int *) malloc(size * sizeof(int));			
			for (int i = 0; i < size; i++){
				m_nLibaoStage[i] = value[i].GetInt();
			}
		}
		m_nLibaoBase = root["randlibao"].GetInt();
		m_fLiBaoPer = root["libaoper"].GetDouble();
		m_nHardType = root["hardtype"].GetInt();
		m_nHardPre  = root["hardper"].GetInt();
		if (m_nHardPre < 1){
			m_nHardPre = 1;
		}
		m_bResetHard = root["resethard"].GetBool();
		
		m_nLibaoMsgColorR = root["libaoR"].GetInt();
		m_nLibaoMsgColorG = root["libaoG"].GetInt();
		m_nLibaoMsgColorB = root["libaoB"].GetInt();
		Value &loginGift = root["logingift"];
		if (loginGift.IsArray()){
			for (int i = 0;i < 7; i++){
				Value &dayGifg = loginGift[i];
				if (!dayGifg.IsArray()){
					break;
				}
				for (int j= 0;j < 2;j++){
					m_nLoginGift[i][j]=dayGifg[j].GetInt();
				}
			}
		}
		m_bShowExitDig = root["showExit"].GetInt();
		m_nShowQuickBtn = root["showbtn"].GetInt();
	}
}