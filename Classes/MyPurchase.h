#ifndef __MY_PURCHASE_H__
#define __MY_PURCHASE_H__

#include "cocos2d.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) 
#include <jni.h> 
#include "platform/android/jni/JniHelper.h" 
#endif 
#include <functional>

USING_NS_CC;

enum BuyType
{
	BuyType_Diamond_200 = 1,
	BuyType_Diamond_960,
	BuyType_Diamond_2250,
	BuyType_Diamond_3600,
	BuyType_Diamond_6000,
	BuyType_Props,
	BuyType_Stage_Step,
	BuyType_Pet_First_Get,
	BuyType_Pet_Recommend,
};

enum ReturnCode
{
	kReturnCodeSucceed,
	kReturnCodeFail,
	kReturnCodeCanceled, 
};
class MyPurchaseResult
{
public:
	void onPayResult(int ret, const char* msg){}
	void setHandle(){}
};

class MyPurchase
{
public:
	static MyPurchase* sharedPurchase();
	void buyItem(int id, std::function<void()> callback);

	static void onPayResult(int ret);
private:
	MyPurchase();
	bool checkBuyType(int type);
private:
	static std::function<void()> s_callback;
};
#endif