#include "MyPurchase.h"
#if  (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID )
#include "org_cocos2dx_lib_MiGuSdk.h"
#endif
#include "ActionRunner.h"
#include "DataManager.h"
using namespace cocos2d;
using namespace std;

function<void()> MyPurchase::s_callback = function<void()>();
ActionRunner* MyPurchase::s_runner = NULL;

MyPurchase::MyPurchase()
{
	s_runner = ActionRunner::create();
	s_runner->retain();
}

MyPurchase::~MyPurchase()
{
	s_runner->clear();
	s_runner->release();
}

MyPurchase* MyPurchase::sharedPurchase()
{
	static MyPurchase model;
	return &model;
}

void MyPurchase::buyItem(int id, std::function<void()> callback)
{
	if (!checkBuyType(id)) return;

	s_callback = callback;
#if(CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) 
	onPayResult(kReturnCodeSucceed);
#elif(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
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
		"Purchase",
		"(I)V");
	if (isHave) {
		minfo.env->CallVoidMethod(jobj, minfo.methodID, id);
	}
#endif
}

void MyPurchase::onPayResult(int ret)
{
	CCLOG("payResult:   %d", ret);
	switch (ret)
	{
	case kReturnCodeSucceed:
		//ÑÓÊ±0.5f SdkºóÌ¨
		if (s_callback)
		{
			s_runner->queueAction(DelayAction::withDelay(0.5f));
			s_runner->queueAction(CallFuncAction::withFunctor([=]()
			{
				s_callback();
			}));
		}
		break;
	case kReturnCodeFail:
		s_callback = function<void()>();
		break;
	case kReturnCodeCanceled:
		s_callback = function<void()>();
		break;
	default:
		s_callback = function<void()>();

		break;
	}
	
}

bool MyPurchase::checkBuyType(int type)
{
	return type >= 1 && type <= DataManagerSelf->getPurchaseConfigs().size();
}

void MyPurchase::showToast(int index)
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
		"showToast",
		"(I)V");  
	if (isHave) {
		minfo.env->CallVoidMethod(jobj, minfo.methodID, index);
	}
#endif
}