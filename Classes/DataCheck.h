#ifndef __DATACHECK_H__ 
#define __DATACHECK_H__
#include "cocos2d.h"

class DataCheck
{
public:
	DataCheck();
	~DataCheck();
	
	void checkDataBase();
	
private:
	void checkStageData(int stageNum);
	void checkStagesConfig();

};
#endif