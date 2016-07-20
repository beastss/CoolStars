#ifndef __GAMEDATAPRELOADER_H__
#define __GAMEDATAPRELOADER_H__

class GameDataPreLoader
{
public:
	GameDataPreLoader(){}
	~GameDataPreLoader(){}

	void initGameData();
private:
	void LoadGameConfigs();
	void intModels();
	void copySqlFiles();
private:
};
#endif



