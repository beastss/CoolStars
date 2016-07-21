#include "GreenGameScene.h"
#include "CCFunctionAction.h"
#include "MainScene.h"

USING_NS_CC;
using namespace std;

bool GreenGameScene::init()
{
	auto bg = CCSprite::create("common/green_game.jpg");
	auto size = bg->getContentSize();
	bg->setPosition(ccpMult(size, 0.5f));
	addChild(bg);

	auto func = CCFunctionAction::create([=]()
	{
		CCScene *pScene = MainScene::theScene();
		auto reScene = CCTransitionCrossFade::create(1.0f, pScene);
		CCDirector::sharedDirector()->replaceScene(reScene);
	});
	
	runAction(CCSequence::create(CCDelayTime::create(3.0f), func, NULL));
	return true;
}