#include "GameDataPreLoader.h"
#include "GameData.h"
#include "UserInfo.h"
#include "SoundMgr.h"
#include "MyPurchase.h"
#include "DataManager.h"
#include "DataCheck.h"
#include "PetManager.h"
#include "PropManager.h"
#include "StarsController.h"
#include "GuideMgr.h"
#include "RankingModel.h"
#include "StageDataMgr.h"
#include "ThiefModel.h"
#include "SqliteHelper.h"

static bool isFileExist(const char* pFileName)
{
	if (!pFileName)return false;
	std::string filePath = CCFileUtils::sharedFileUtils()->getWritablePath();
	filePath += pFileName;
	FILE *pFp = fopen(filePath.c_str(), "r");
	if (pFp)
	{
		fclose(pFp);
		return true;
	}
	return false;
}

static void copyData(const char* pFileName)
{
	string sourcePath = "data/";
	sourcePath += pFileName;
	std::string strPath = CCFileUtils::sharedFileUtils()->fullPathForFilename(sourcePath.c_str());
	unsigned long len = 0;
	unsigned char* data = NULL;
	data = CCFileUtils::sharedFileUtils()->getFileData(strPath.c_str(), "r", &len);

	std::string destPath = CCFileUtils::sharedFileUtils()->getWritablePath();
	destPath += pFileName;

	FILE *pFp = fopen(destPath.c_str(), "w+");
	fwrite(data, sizeof(char), len, pFp);
	fclose(pFp);
	delete[]data;
	data = NULL;
}

void GameDataPreLoader::copySqlFiles()
{
	vector<string> files;
	files.push_back(DB_CONFIG);
	files.push_back(DB_SAVING);
	files.push_back(DB_STAGE);

#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)//Android下需要复制数据文件
	//检查数据库文件是否已经提取
	for (size_t i = 0; i < files.size(); ++i)
	{
		string file = files[i];
		if (isFileExist(file.c_str()) == false)
		{
			
			copyData(file.c_str());//要使用的sqlite库文件
		}
	}
#endif
}

void GameDataPreLoader::initGameData()
{
	copySqlFiles();
	LoadGameConfigs();
	intModels();

}

void GameDataPreLoader::LoadGameConfigs()
{
	//GameData::getInstance()->preloadData();

	//对数据库数据进行数据校验 正式版本着取消校验
	DataCheck check;
	check.checkDataBase();

	DataManagerSelf->LoadData();

}

void GameDataPreLoader::intModels()
{
	UserInfo::theInfo()->init();
	SoundMgr::theMgr()->init();

	StageDataMgr::theMgr()->init();
	PropManager::propMgr()->loadPropData();
	PetManager::petMgr()->init();
	GuideMgr::theMgr()->init();
	RankingModel::theModel()->init();
	ThiefModel::theModel()->init();
}