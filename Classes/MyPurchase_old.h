#ifndef __MY_PURCHASE_H__
#define __MY_PURCHASE_H__

/*
#include "cocos2d.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) 
#include <jni.h> 
#include "platform/android/jni/JniHelper.h" 
#endif 
#include "ConfData.h"

USING_NS_CC;

class MyPurchaseResult
{
public:
	void onPayResult(int ret, const char* msg);
	void setCallback(CCObject *target, SEL_CallFuncN3I selector, int payType);
	MyPurchaseResult();
private:
	SEL_CallFuncN3I m_pCallback;
	CCObject* m_pCallbackTarget;
	int m_nCurrentPayType;
};

class MyPurchase
{
public:
	static MyPurchase* sharedPurchase();
	static void purgePurchase();
	void loadIAPPlugin();
	void payForProducts(CCObject *target, MyPayProducts product, SEL_CallFuncN3I selector = NULL);
	void showToast(const char * hint);
	bool exitGame();
	void exitGameMM();
	bool musicEnable();
	void moreGame();
	int m_nCarrietType;
	int getCarrietType();
	MyPurchaseResult* s_pRetListener;
private:
	MyPurchase();
	virtual ~MyPurchase();
	static MyPurchase* s_pPurchase;
};
*/
#endif