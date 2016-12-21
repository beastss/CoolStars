#include "TreasureStarsLayer.h"
#include "UiLayout.h"
#include "CommonMacros.h"
#include "TreasureStarView.h"
#include "TreasureStageMacro.h"
#include "TreasureStageModel.h"
#include "TreasureStageDataMgr.h"

using namespace cocos2d;
using namespace std;

TreasureStarsLayer::TreasureStarsLayer()
{
}

TreasureStarsLayer::~TreasureStarsLayer()
{
}

TreasureStarsLayer *TreasureStarsLayer::create()
{
	TreasureStarsLayer* layer = new TreasureStarsLayer();
	layer->init();
	layer->autorelease();
	return layer;
}

void TreasureStarsLayer::onEnter()
{
	CCNode::onEnter();
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, kStageStarsTouchPriority, true);
	TreasureStageNotifier::theNotifier()->addView(this);
}

void TreasureStarsLayer::onExit()
{
	CCNode::onExit();
	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
	TreasureStageNotifier::theNotifier()->removeView(this);
}

bool TreasureStarsLayer::init()
{
	auto winSize = CCDirector::sharedDirector()->getWinSize();
	auto size = CCSize(winSize.width, winSize.height * 0.7f);
	setContentSize(size);

	addBackground();
	addClippingNode();
	return true;
}

cocos2d::CCPoint TreasureStarsLayer::getLeftLowerPt()
{
	CCPoint leftLower;
	leftLower.x = (getContentSize().width - TREASURE_STAR_SIZE * TREASURE_COLUMNS_NUM) * 0.5f;
	leftLower.y = 80;
	return leftLower;
}

void TreasureStarsLayer::addBackground()
{
	m_bkLayer = CCNode::create();
	addChild(m_bkLayer);

	CCSprite *bk = CCSprite::create("stage/yxjm_di1.png");	
	bk->setScale(TREASURE_STAR_SIZE * TREASURE_COLUMNS_NUM / bk->getContentSize().width);
	bk->setAnchorPoint(ccp(0, 0));
	bk->setPosition(getLeftLowerPt());
	m_bkLayer->addChild(bk);

	string gridPath = "stage/yxjm_di2.png";
	CCSpriteBatchNode* batchNode = CCSpriteBatchNode::create(gridPath.c_str());
	auto temp = CCSprite::create(gridPath.c_str());
	float scale = TREASURE_STAR_SIZE / temp->getContentSize().width * 0.94f;
	for (int i = 0; i < TREASURE_ROWS_NUM; ++i)
	{
		for (int j = 0; j < TREASURE_COLUMNS_NUM; ++j)
		{
			CCSprite *spr = CCSprite::createWithTexture(batchNode->getTexture());
			CCPoint pos;
			pos.x = TREASURE_STAR_SIZE * (0.5f + j);
			pos.y = TREASURE_STAR_SIZE * (0.5f + i);
			spr->setPosition(pos);
			spr->setScale(scale);
			batchNode->addChild(spr);
		}
	}
	batchNode->setPosition(getLeftLowerPt());
	m_bkLayer->addChild(batchNode);
}

void TreasureStarsLayer::refresh()
{
	m_clippingNode->removeAllChildrenWithCleanup(true);
	initStars();
}

void TreasureStarsLayer::addClippingNode()
{
	auto size = getContentSize();

	CCLayerColor *back = CCLayerColor::create(ccc4(125, 0, 0, 255));
	back->setContentSize(size);
	m_clippingNode = CCClippingNode::create();
	m_clippingNode->setInverted(false);
	m_clippingNode->setAlphaThreshold(1.0f);
	m_clippingNode->setStencil(back);
	addChild(m_clippingNode);

	auto maskBk = CCLayerColor::create(ccc4(255, 0, 0, 80));
	maskBk->setAnchorPoint(ccp(0, 0));
	maskBk->setContentSize(size);
	//addChild(maskBk);
}

void TreasureStarsLayer::initStars()
{
	TreasureStageModel::theModel()->initData();//temp;
	auto starsData = TreasureStageModel::theModel()->getStarsData();

	float startHeight = 3 * TREASURE_STAR_SIZE;
	float speed = 600;
	float finishTime = 0;

	for (size_t i = 0; i < starsData.size(); ++i)
	{
		int col = starsData[i]->getGrid().x;
		int row = starsData[i]->getGrid().y;

		auto view = TreasureStarView::create(starsData[i]);
		m_clippingNode->addChild(view);
		CCPoint targetPos = getPosByGrid(LogicGrid(col, row));
		CCPoint sourcePos = getPosByGrid(LogicGrid(col, row * 2 + (col % 2 == 1 ? 0 : 1)));
		sourcePos.y += startHeight;
		view->setPosition(sourcePos);
		float duration = (sourcePos.y - targetPos.y) / speed;
		auto *move = CCMoveTo::create(duration, targetPos);
		view->runAction(move);
		finishTime = max(finishTime, duration);
	}

	runAction(CCSequence::create(CCDelayTime::create(finishTime)
		, CCCallFunc::create(this, callfunc_selector(TreasureStarsLayer::starInitDone)), NULL));
}

cocos2d::CCPoint TreasureStarsLayer::getPosByGrid(LogicGrid grid)
{
	CCPoint leftLower = getLeftLowerPt();
	CCPoint pt;
	pt.x = leftLower.x + (grid.x + 0.5f) * TREASURE_STAR_SIZE;
	pt.y = leftLower.y + (grid.y + 0.5f) * TREASURE_STAR_SIZE;
	return pt;
}

void TreasureStarsLayer::starInitDone()
{
	//CCMessageBox("title", "done");
}

bool TreasureStarsLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
	return false;
}

void TreasureStarsLayer::onStarExplore(int color, int score, cocos2d::CCPoint pt)
{
	CCParticleExplosion *pEmitter = CCParticleExplosion::create();
	string fileImage = TreasureStageDataMgrSelf->getStarData(color).explosionPath;
	pEmitter->setTexture(CCTextureCache::sharedTextureCache()->addImage(fileImage.c_str()));
	pEmitter->setAutoRemoveOnFinish(true);
	pEmitter->setPosition(m_clippingNode->convertToNodeSpace(pt));

	ccColor4F c4Var = { 0, 0, 0, 0.0 };
	ccColor4F c4 = { 1, 1, 1, 1 };
	pEmitter->setStartColor(c4);
	pEmitter->setEndColor(c4);
	pEmitter->setStartColorVar(c4Var);
	pEmitter->setEndColorVar(c4Var);
	pEmitter->setTotalParticles(10);
	pEmitter->setRadialAccel(1);
	pEmitter->setRadialAccelVar(5);
	pEmitter->setSpeed(200);
	pEmitter->setScale(1.2f);
	pEmitter->setGravity(ccp(0, -200));

	m_clippingNode->addChild(pEmitter);
}