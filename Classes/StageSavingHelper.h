#ifndef __STAGESAVINGHELPER_H__
#define __STAGESAVINGHELPER_H__
#include "cocos2d.h"
#include "sqlite3.h"

class StageSavingHelper
{
public:
	static void saveCurStars();
	static bool getLastSavedStars(std::vector<std::vector<int>> &stars);
	static void saveCurStageData();
	static void LoadLastSavedStageData();
};

#endif
