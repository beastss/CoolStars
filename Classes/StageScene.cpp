#include "StageScene.h"
#include "StarsLayer.h"
#include "StageUiLayer.h"
#include "VisibleRect.h"
#include "EmptyBox.h"
#include "BackgroundLayer.h"
#include "StageMaskLayer.h"
#include "GuideMgr.h"
#include "PauseDialog.h"
#include "MainScene.h"
#include "SoundMgr.h"
using namespace cocos2d;

StageScene *StageScene::s_scene = NULL;

StageScene::StageScene()
{

}

StageScene::~StageScene()
{

}

void StageScene::onEnter()
{
	BasePanel::onEnter();
	SoundMgr::theMgr()->playBackground(kBackGroundMusicStage);
	return;
	// ¹«¸æ
	MainScene::theScene()->removeAnnouncement();
	MainScene::theScene()->setAnnouncementLoop(true);
	MainScene::theScene()->addAnnouncement("announcement/game_picture_1.png");
	MainScene::theScene()->addAnnouncement("announcement/game_picture_3.png");
}

void StageScene::onExit()
{
	BasePanel::onExit();
	s_scene = NULL;

	SoundMgr::theMgr()->playBackground(kBackGroundMusicMainMenu);
	MainScene::theScene()->removeAnnouncement();
}

StageScene* StageScene::theScene()
{
	if (!s_scene)
	{
		s_scene = new StageScene;
		s_scene->init();
		s_scene->autorelease();
	}
	return s_scene;
}

bool StageScene::init()
{
	setPanelId(kStageView);

	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	setContentSize(winSize);

	m_starsLayer = StarsLayer::create();
	m_starsLayer->setAnchorPoint(ccp(0.5f, 0.5f));
	m_starsLayer->setPosition(ccpMult(winSize, 0.5f));
	addChild(m_starsLayer);

	m_stageUiLayer = StageUiLayer::create();
	addChild(m_stageUiLayer);
	
	m_maskLayer = StageMaskLayer::create();
	m_maskLayer->initPetViewsInfo(m_stageUiLayer->getPetViewsInfo());
	addChild(m_maskLayer);

	m_stateOwner.setStarsLayer(m_starsLayer);
	m_stateOwner.setUiLayer(m_stageUiLayer);
	m_stateOwner.init();

	GuideMgr::theMgr()->endGuide(kGuideEnd_stage_in);
	return true;
}

