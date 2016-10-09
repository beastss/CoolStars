#ifndef __REDPACKAGEMODULE_H__
#define __REDPACKAGEMODULE_H__
#include "cocos2d.h"

enum RedPackageType
{
	kRedPackageDiamond,
	kRedPackageDiamondPackage,
	kRedPackageBomb,
};

class RedPackageModule
{
public:
	RedPackageModule();
	bool isShowUp();
	void openRedPackage();
private:
	void init();
private:
	bool m_everGetRedPackage;//过去有没有领过红包
	bool m_alreadyGetThisRedPackage;//当前关卡的红包有没领过
};
#endif