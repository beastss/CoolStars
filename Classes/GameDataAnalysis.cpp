#include "GameDataAnalysis.h"
#include "DataManager.h"
#include "PropManager.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) 
#include <jni.h> 
#include "platform/android/jni/JniHelper.h" 
#endif 
USING_NS_CC;
using namespace std;
GameDataAnalysis *GameDataAnalysis::theModel()
{
	static GameDataAnalysis model;
	return &model;
}

void GameDataAnalysis::consumeDiamond(int type, int param, int cost)
{
	string name;
	switch (type)
	{
	case kDiamondConsumePackage:
	{
		name = "tili_libao";
		break;
	}
	case kDiamondConsumeLottery:
	{
		name = "chou_jiang";
		break;
	}
	case kDiamondConsumePetUpgrade:
	{
		string petName = DataManagerSelf->getPetResConfig(param).name;
		name = "shengji_";
		name += petName;
	    break;
	}
	case kDiamondConsumeBuyPet:
	{
		string petName = DataManagerSelf->getPetResConfig(param).name;
	    name = "goumai_";
	    name += petName;
		break;
	}
	default:
		assert(false && "not valid item");
		break;
	}
	CCLOG("GameDataAnalysis::consumeDiamond name: %s cost: %d", name.c_str(), cost);

#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) 
	const char* funstr = "org/cocos2dx/lib/PayAndroidApi";
	JniMethodInfo minfo;
	bool isHave = JniHelper::getStaticMethodInfo(minfo,
		funstr,
		"rtnActivity",
		"()Ljava/lang/Object;");
	jobject jobj;
	if (isHave) {
		jobj = minfo.env->CallStaticObjectMethod(minfo.classID, minfo.methodID);
	}

	isHave = JniHelper::getMethodInfo(minfo,
		funstr,
		"buyItemByDiamond",
		"(Ljava/lang/String;I)V");
	if (isHave) {
		jstring stringArg = minfo.env->NewStringUTF(name.c_str());
		minfo.env->CallVoidMethod(jobj, minfo.methodID, stringArg, cost);
	}
#endif
}

void GameDataAnalysis::consumeProps(int type)
{
	string name;
	switch (type)
	{
	case kPropReorder:
		name = "chong_pai";
		break;
	case kPropBrush:
		name = "shua_zi";
		break;
	case kPropBomb:
		name = "zha_dan";
		break;
	default:
		break;
	}
	CCLOG("GameDataAnalysis::consumeProps name: %s", name.c_str());

#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) 
	const char* funstr = "org/cocos2dx/lib/PayAndroidApi";
	JniMethodInfo minfo;
	bool isHave = JniHelper::getStaticMethodInfo(minfo,
		funstr,
		"rtnActivity",
		"()Ljava/lang/Object;");
	jobject jobj;
	if (isHave) {
		jobj = minfo.env->CallStaticObjectMethod(minfo.classID, minfo.methodID);
	}

	isHave = JniHelper::getMethodInfo(minfo,
		funstr,
		"useItem",
		"(Ljava/lang/String;)V");
	if (isHave) {
		jstring stringArg = minfo.env->NewStringUTF(name.c_str());
		minfo.env->CallVoidMethod(jobj, minfo.methodID, stringArg);
	}
#endif
}