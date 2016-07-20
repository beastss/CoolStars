#include "MyPurchase.h"
/*


#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID && PAY_NO_PAY == 0) 
#include "PluginManager.h"
#endif
#include "cocos2d.h"
#include "UserInfo.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID && PAY_NO_PAY == 0) 
using namespace cocos2d::plugin;
#endif
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
#include "CCWinRTUtils.h"
#endif
using namespace cocos2d;

MyPurchase* MyPurchase::s_pPurchase = NULL;

MyPurchase::MyPurchase()
:s_pRetListener(NULL)
, m_nCarrietType(-1)
{

}

MyPurchase::~MyPurchase(){
	if (s_pRetListener){
		delete s_pRetListener;
		s_pRetListener = NULL;
	}
}

MyPurchase* MyPurchase::sharedPurchase(){
	if (s_pPurchase == NULL) {
		s_pPurchase = new MyPurchase();
	}
	return s_pPurchase;
}

void MyPurchase::loadIAPPlugin(){
	if (s_pRetListener == NULL){
		s_pRetListener = new MyPurchaseResult();
	}
}

void MyPurchase::payForProducts(CCObject *target, MyPayProducts product, SEL_CallFuncN3I selector ){
	s_pRetListener->setCallback(target, selector, product);
	CCLOG("payForProducts product = %d", product);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 || PAY_NO_PAY == 1)
	s_pRetListener->onPayResult(PAY_RESULT_SUCCESS, (const char*)"");
#else
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
		"Pay",
		"(I)V");
	if (isHave) {
		minfo.env->CallVoidMethod(jobj, minfo.methodID, product);
	}
#endif
}

void MyPurchase::showToast(const char * hint){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
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
		"(Ljava/lang/String;)V");  
	if (isHave) {
		jstring stringArg = minfo.env->NewStringUTF(hint);
		minfo.env->CallVoidMethod(jobj, minfo.methodID, stringArg);
	}

#endif
}

int MyPurchase::getCarrietType(){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

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
	jint carrietType = CARRIET_TYPE_UNKNOWN;
	isHave = JniHelper::getMethodInfo(minfo, 
		funstr,
		"getCarrietType",
		"()I");
	if (isHave) {
		carrietType = minfo.env->CallIntMethod(jobj, minfo.methodID);
	}
	return carrietType;
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	return CARRIET_TYPE_CM;
#else
	return CARRIET_TYPE_UNKNOWN;
#endif
}

bool MyPurchase::exitGame(){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID && (PAY_TYPE_JIDI == 1))
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
		"isExitGame",
		"()Z");
	jboolean isExit = true;
	if (isHave) {
		isExit = minfo.env->CallBooleanMethod(jobj, minfo.methodID);
	}
	return isExit;
#else
	return true;
#endif
}

bool MyPurchase::musicEnable(){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID && PAY_TYPE_JIDI == 1)
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
		"isMusicEnable",
		"()Z");
	CCLOG("musicEnable isMusicEnable isHave = %d", isHave);
	jboolean isEnable = true;
	if (isHave) {
		isEnable = minfo.env->CallBooleanMethod(jobj, minfo.methodID);
		CCLOG("musicEnable minfo isEnable = %d", isEnable);
	}
	return isEnable;
#else
	return true;
#endif
}

void MyPurchase::moreGame(){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID && (PAY_TYPE_JIDI == 1))
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
		"moreGame",
		"()V");  
	if (isHave) { 
		minfo.env->CallVoidMethod(jobj, minfo.methodID); 
	}
#endif
}

void MyPurchaseResult::onPayResult(int ret, const char* msg){
	CCLOG("onPayResult ret  = %d ,m_nCurrentPayType = %d", ret, m_nCurrentPayType);
	if (ret == PAY_RESULT_SUCCESS){
		int orderCoins = 0;
		CCDictionary *text = CCDictionary::createWithContentsOfFile(XML_DATA);
		CCString *buyCount = (CCString*)text->objectForKey("buy1");
		CCString *presentCount = (CCString*)text->objectForKey("pres1");
		switch (m_nCurrentPayType){
		case PAY_TYPE_DIAMOND1:
			break;
		case PAY_TYPE_DIAMOND2:
			buyCount = (CCString*)text->objectForKey("buy2");
			presentCount = (CCString*)text->objectForKey("pres2");
			break;
		case PAY_TYPE_DIAMOND3:
			buyCount = (CCString*)text->objectForKey("buy3");
			presentCount = (CCString*)text->objectForKey("pres3");
			break;
		case PAY_TYPE_DIAMOND4:
			buyCount = (CCString*)text->objectForKey("buy4");
			presentCount = (CCString*)text->objectForKey("pres4");
			break;
		case PAY_TYPE_DIAMOND5:
			buyCount = (CCString*)text->objectForKey("buy5");
			presentCount = (CCString*)text->objectForKey("pres5");
			break;
		case PAY_TYPE_FISH_LIBAO:
			buyCount = (CCString*)text->objectForKey("onceCount");
			presentCount = CCString::create("0");
			break;
		case PAY_TYPE_EXIT_LIBAO:
			CCString *bombCount = (CCString*)text->objectForKey("libaoBomb");
			CCString *paintCount = (CCString*)text->objectForKey("libaoPai");
			CCString *reflashCount = (CCString*)text->objectForKey("libaoRef");
			UserInfo::addProByType(USERINFO_PRO_TYPE_BOMB, bombCount->intValue());
			UserInfo::addProByType(USERINFO_PRO_TYPE_PAINT, paintCount->intValue());
			UserInfo::addProByType(USERINFO_PRO_TYPE_REFLASH, reflashCount->intValue());
			buyCount = (CCString*)text->objectForKey("libaoDia");
			presentCount = CCString::create("0");
			break;
		}
		orderCoins = buyCount->intValue() + presentCount->intValue();
		UserInfo::addCoins(orderCoins);
	}
	if (m_pCallbackTarget != NULL && m_pCallback != NULL) {
		(m_pCallbackTarget->*m_pCallback)(NULL, m_nCurrentPayType, ret, 0);
		m_pCallbackTarget = NULL;
		m_pCallback = NULL;
	}
}

MyPurchaseResult::MyPurchaseResult()
:m_pCallbackTarget(NULL)
, m_pCallback(NULL)
, m_nCurrentPayType(-1)
{

}

void MyPurchaseResult::setCallback(CCObject *target, SEL_CallFuncN3I selector, int payType){
	m_pCallbackTarget = target;
	m_pCallback = selector;
	m_nCurrentPayType = payType;
}
*/
