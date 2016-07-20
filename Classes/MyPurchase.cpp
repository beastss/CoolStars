#include "MyPurchase.h"
#if  (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID )
#include "org_cocos2dx_lib_MiGuSdk.h"
#endif
using namespace cocos2d;
using namespace std;

function<void()> MyPurchase::s_callback = function<void()>();

MyPurchase::MyPurchase()
{

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
#else(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
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
	CCLOG("payResult:   ", ret);
	switch (ret)
	{
	case kReturnCodeSucceed:
		if (s_callback)
		{
			s_callback();
		}
		break;
	case kReturnCodeFail:
		break;
	case kReturnCodeCanceled:
		break;
	default:
		break;
	}
	
	s_callback = function<void()>();
}

bool MyPurchase::checkBuyType(int type)
{
	return type >= BuyType_Diamond_200 && type <= BuyType_Pet_Recommend;
}