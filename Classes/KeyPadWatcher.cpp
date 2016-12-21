#include "KeyPadWatcher.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) 
#include <jni.h> 
#include "platform/android/jni/JniHelper.h" 
#endif 
#include "MainScene.h"
#include "GuideMgr.h"
#include "SoundMgr.h"

USING_NS_CC;
using namespace std;

bool KeyPadWatcher::init()
{
	setKeypadEnabled(true);
	return true;
}

void KeyPadWatcher::keyBackClicked()
{
	if (!GuideMgr::theMgr()->isRunning())
	{
		SoundMgr::theMgr()->playEffect(kEffectMusicButton);
		MainScene::theScene()->handleKeyBackTouch();
	}

}

void KeyPadWatcher::exitGame()
{
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
		"exitGame",
		"()V");
	if (isHave) {
		minfo.env->CallVoidMethod(jobj, minfo.methodID);
	}
#endif
}

void KeyPadWatcher::keyMenuClicked()
{

}
