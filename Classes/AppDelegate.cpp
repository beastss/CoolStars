#include "AppDelegate.h"
#include "GameDataPreLoader.h"
#include "TestScene.h"
#include "MainScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;
using namespace CocosDenshion;
AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    CCDirector* pDirector = CCDirector::sharedDirector();
    CCEGLView* pEGLView = CCEGLView::sharedOpenGLView();
	
	pDirector->setOpenGLView(pEGLView);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 )
	pEGLView->setFrameSize(480, 800);
#endif
	CCSize designSize = CCSizeMake(480, 800);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS )
	CCEGLView::sharedOpenGLView()->setDesignResolutionSize(designSize.width, designSize.height, kResolutionFixedWidth);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_WP8 )
	CCEGLView::sharedOpenGLView()->setDesignResolutionSize(designSize.width, designSize.height, kResolutionFixedWidth);
#else
	CCEGLView::sharedOpenGLView()->setDesignResolutionSize(designSize.width, designSize.height, kResolutionFixedWidth);
#endif
	//加载数据并初始化游戏环境
	GameDataPreLoader loader;
	loader.initGameData();

    // turn on display FPS
    pDirector->setDisplayStats(false);

    // set FPS. the default value is 1.0/60 if you don't call this
    pDirector->setAnimationInterval(1.0 / 60);

    // create a scene. it's an autorelease object
//#define TEST_SCENE
#ifdef TEST_SCENE
	CCScene *pScene = TestScene::scene();
#else
	CCScene *pScene = MainScene::theScene();
#endif	

    // run
    pDirector->runWithScene(pScene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    CCDirector::sharedDirector()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    CCDirector::sharedDirector()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
}





