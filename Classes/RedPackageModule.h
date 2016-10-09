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
	bool m_everGetRedPackage;//��ȥ��û��������
	bool m_alreadyGetThisRedPackage;//��ǰ�ؿ��ĺ����û���
};
#endif