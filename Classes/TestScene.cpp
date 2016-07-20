#include "TestScene.h"
#include "UiLayout.h"
#include "MenuScene.h"
#include "ConfData.h"
#include "ListView.h"
#include "ListPetView.h"
#include "StageOperator.h"
#include "StarNode.h"
#include "TimeUtil.h"
#include "SqliteHelper.h"
#include <fstream>

USING_NS_CC;

CCScene* TestScene::scene()
{
	CCScene *scene = CCScene::create();
	TestScene *layer = TestScene::create();
	scene->addChild(layer);
	return scene;
}

bool TestScene::init()
{
	if (!CCLayer::init())
	{
		return false;
	}
	initPanel();
	//test();
	progressTest();
	return true;
}

void TestScene::test()
{
	int curTime = time_util::getCurTime();
	auto date = time_util::getDate(curTime);
}

void TestScene::progressTest()
{

	CCProgressTimer* pt2 = CCProgressTimer::create(CCSprite::create("stage/ui/yxjm_fenshucao2.png"));
	pt2->setType(kCCProgressTimerTypeBar);// 设置成横向的
	//可以看作是按矩形显示效果的进度条类型
	pt2->setMidpoint(ccp(0, 1));   // 设置进度方向
	pt2->setBarChangeRate(ccp(1, 0));  // 设置进度为水平还是垂直方向
	//重新设置锚点
	pt2->setPosition(ccp(200, 200));
	pt2->setPercentage(0);
	this->addChild(pt2);
	CCProgressTo *t2 = CCProgressTo::create(1.5f, 100);
	pt2->runAction(t2);

	return;
	//radial
	CCProgressTimer *progress1 = CCProgressTimer::create(CCSprite::create("pet_skill_icon/jntb_hou4.png"));

	progress1->setPosition(ccp(100, 100));

	//设置进度条的样式

	progress1->setType(kCCProgressTimerTypeRadial);
	progress1->setPercentage(40);
	addChild(progress1);

	CCProgressTo *t = CCProgressTo::create(1.5, 100);
	progress1->runAction(t);
}

void TestScene::animationTest()
{
	/*
	CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo("test/chuangguanchenggong.ExportJson");
	auto ani = CCArmature::create("chuangguanchenggong");
	ani->getAnimation()->play("tg_xing3");
	//ani->getAnimation()->playWithIndex(0, -1, -1, 0);
	ani->setPosition(ccp(100, 100));
	addChild(ani, 10);
	*/

	CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo("Dragon.png", "Dragon.plist", "Dragon.xml");

	cocos2d::extension::CCArmature *armature = NULL;
	armature = cocos2d::extension::CCArmature::create("Dragon");
	auto data = armature->getAnimation()->getAnimationData()->getMovementNames();

	armature->getAnimation()->play("walk");
	//armature->getAnimation()->playWithIndex(0);
	armature->getAnimation()->setSpeedScale(0.4f);
	armature->setPosition(ccp(200, 200));
	armature->setScale(0.6f);
	addChild(armature);
}

void TestScene::animationTest2()
{
	CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo("pet_animation/gou_lv/gou_lv.ExportJson");
	auto ani2 = CCArmature::create("gou_lv");
	ani2->getAnimation()->playWithIndex(1);
	//ani->getAnimation()->playWithIndex(0, -1, -1, 0);
	ani2->setPosition(ccp(300, 300));
	ani2->setScale(0.2f);
	addChild(ani2, 10);

	CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo("pet_animation/gou_huang/gou_huang.ExportJson");
	auto ani = CCArmature::create("gou_huang");
	ani->getAnimation()->playWithIndex(0);
	//ani->getAnimation()->playWithIndex(0, -1, -1, 0);
	ani->setPosition(ccp(100, 100));
	ani->setScale(0.2f);
	addChild(ani, 10);


	
}

void TestScene::imageNumtest()
{
	CCLabelAtlas* diceCount = CCLabelAtlas::create("100:98;5", "pet/cwjm_shuzi7.png", 18, 23, '0');
	diceCount->setPosition(ccp(100, 100));
	addChild(diceCount);
}

void TestScene::listViewTest()
{
	/*
	m_spr = CCSprite::create("shop/sd_goumai.png");
	addChild(m_spr);
	m_spr->setPosition(ccp(200, 200));
	return;
	*/
	auto size = CCSize(350, 500);
	ListPetView *listView = ListPetView::create(CCSize(350, 500));
	addChild(listView);
	listView->setPosition(ccp(30, 100));
	listView->setSpacing(20);

	for (int i = 0; i < 30; i++)
	{
		CCSprite *spr = CCSprite::create("shop/sd_di2.png");
		spr->ignoreAnchorPointForPosition(false);
		spr->setAnchorPoint(ccp(0, 0));
		listView->addNode(spr);
		
		char str[100] = { 0 };
		sprintf(str, "%d", i + 1);
		CCLabelTTF *label = CCLabelTTF::create(str, "Arial", 24);
		label->setPosition(ccp(50, 50));
		spr->addChild(label);
		
	}
	listView->setCurItem(29);
}

void TestScene::initPanel()
{
	auto winSize = CCDirector::sharedDirector()->getWinSize();
	CCMenuItemImage *pCloseItem = CCMenuItemImage::create(
		"main_menu/zcd_anniu5.png",
		"main_menu/zcd_anniu5.png",
		this,
		menu_selector(TestScene::testCallback));

	pCloseItem->setPosition(ccp(50, 50));
	CCMenu *menu = CCMenu::create(pCloseItem, NULL);
	menu->setPosition(ccp(0, 0));
	addChild(menu);
}

void TestScene::addClippingNode()
{
	/*
	CCLayerColor *back = CCLayerColor::create(ccc4(125, 0, 0, 255));
	CCSize size = CCSize(ccp(200, 200));
	back->setContentSize(size);
	//addChild(back);

	m_clippingNode = CCClippingNode::create();
	m_clippingNode->setInverted(false);
	m_clippingNode->setAlphaThreshold(0.0f);

	CCSprite *sp = CCSprite::create("shop/sd_zuanshi2.png");
	sp->setAnchorPoint(ccp(0, 0));
	m_clippingNode->setStencil(back);
	addChild(m_clippingNode);

	auto spr = CCSprite::create("shop/sd_di.png");
	//spr->ignoreAnchorPointForPosition(true);
	spr->setAnchorPoint(ccp(0, 0));
	m_clippingNode->addChild(spr);

	*/
}

void TestScene::closeCallback(CCObject* pSender)
{
	//CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5f, MenuScene::scene()));
}

void TestScene::printDBInfos()
{
	SqliteHelper helper;

	string dbName[] = { DB_SAVING, DB_STAGE, DB_CONFIG };
	for (int i = 0; i < 3; ++i)
	{
		string outName = dbName[i].substr(dbName[i].rfind("/") + 1) + ".txt";
		ofstream out(outName.c_str());

		out << "db Name: " << dbName[i] << endl<<endl;
		helper.openDB(dbName[i].c_str());
		auto info = helper.getDBInfo();
		for (auto iter = info.begin(); iter != info.end(); ++iter)
		{
			out << "---------------------------------" << endl;
			out << "table Name: "<<iter->first << endl;
			auto colNames = iter->second;
			for (size_t j = 0; j < colNames.size(); ++j)
			{
				out << colNames[j] << endl;
			}
			out << endl;
		}
		//out << "/////////////////////////////////" << endl;
	}
}

void TestScene::testCallback(CCObject* pSender)
{
	//m_spr->stopAllActions();
	//m_spr->runAction(CCEaseExponentialOut::create(CCMoveTo::create(1.0f, ccp(200, 600))));
	//m_spr->runAction(CCMoveTo::create(0.5, ccp(300, 200)));
}


