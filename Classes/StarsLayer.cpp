#include "StarsLayer.h"
#include "StarViewNode.h"
#include "StarsController.h"
#include "StageSceneState.h"
#include "GuideMgr.h"
#include "UiLayout.h"
#include "EmptyBox.h"
#include "StageScene.h"
#include <algorithm>

using namespace cocos2d;
using namespace std;
StarsLayer::StarsLayer()
{
	m_stateOwner = StageScene::theScene()->getStateOwner();
}

StarsLayer::~StarsLayer()
{
}

StarsLayer *StarsLayer::create()
{
	StarsLayer* layer = new StarsLayer();
	layer->init();
	layer->autorelease();
	return layer;
}

void StarsLayer::onEnter()
{
	CCNode::onEnter();
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, kStageStarsTouchPriority, true);
	StarsController::theModel()->addView(this);
	StageLayersMgr::theMgr()->addLayer(this);
}

void StarsLayer::onExit()
{
	CCNode::onExit();
	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
	StarsController::theModel()->removeView(this);
	StageLayersMgr::theMgr()->removeLayer(this);
}

bool StarsLayer::init()
{
	auto winSize = CCDirector::sharedDirector()->getWinSize();
	m_layout = UiLayout::create("layout/stage.xml");
	setContentSize(m_layout->getContentSize());
	addChild(m_layout);

	StarsController::theModel()->initStarsData();
	addBkGrids();
	addClippingNode();
	initStars();

	return true;
}

void StarsLayer::addClippingNode()
{
	CCLayerColor *back = CCLayerColor::create(ccc4(125, 0, 0, 255));
	CCSize size = CCSize(STAR_SIZE * COlUMNS_SIZE, STAR_SIZE * ROWS_SIZE);
	back->setContentSize(size);
	
	CCSprite *sp = CCSprite::create("shop/sd_zuanshi2.png");
	sp->setAnchorPoint(ccp(0, 0));
	m_clippingNode = CCClippingNode::create();
	m_clippingNode->setInverted(false);
	m_clippingNode->setAlphaThreshold(1.0f);
	m_clippingNode->setStencil(back);

	addChild(m_clippingNode);
	m_clippingNode->setPosition(m_layout->getChildById(1)->getPosition());
}

void StarsLayer::addBkGrids()
{
	CCNode *node = CCNode::create();
	static const float kSpacing = 2;
	float curX = kSpacing;
	float curY = kSpacing;
	float maxHeight = 0;

	for (int row = 0; row < ROWS_SIZE; ++row)
	{
		for (int col = 0; col < COlUMNS_SIZE; ++col)
		{
			StarNode *nodeData = StarsController::theModel()->getStarNode(LogicGrid(col, row));
			auto grid = CCSprite::create("stage/yxjm_di2.png");
			grid->setAnchorPoint(ccp(0, 0));
			auto size = grid->getContentSize();
			if (!nodeData->isStill())
			{
				node->addChild(grid);
				grid->setPosition(ccp(curX, curY));
			}
			curX += size.width + kSpacing;
			if (maxHeight < size.height)
			{
				maxHeight = size.height;
			}
		}
		curX = kSpacing;
		curY += maxHeight + kSpacing;
	}
	node->setContentSize(m_layout->getChildById(5)->getContentSize());

	addChild(node);
	node->setPosition(m_layout->getChildById(1)->getPosition());
}

StarViewNode *StarsLayer::createStarByGrid(const LogicGrid &grid)
{
	StarNode *nodeData = StarsController::theModel()->getStarNode(grid);
	if (nodeData)
	{
		return StarViewNode::create(nodeData);
	}
	return NULL;
}

void StarsLayer::initStars()
{
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	StarViewNode *pStarSprite = NULL;
	float startHeight = STAR_SIZE * ROWS_SIZE;
	float speed = startHeight / 0.5f;

	float kDuration = 1.0f;
	for (int col = 0; col < COlUMNS_SIZE; ++col)
	{
		for (int row = 0; row < ROWS_SIZE; ++row) 
		{
			LogicGrid grid(col, row);
			pStarSprite = createStarByGrid(grid);
			if (pStarSprite)
			{
				pStarSprite->setAnchorPoint(ccp(0.5f, 0.5f));
				CCPoint targetPos = getPosByGrid(grid);
				CCPoint sourcePos = targetPos;
				sourcePos.y += 30;
			
				pStarSprite->setPosition(sourcePos);
				m_clippingNode->addChild(pStarSprite);
				m_starsSprite.push_back(pStarSprite);

				auto *move = CCEaseBackInOut::create(CCMoveTo::create(kDuration, targetPos));
				pStarSprite->runAction(move);
			}
		}
	}
	runAction(CCSequence::create(CCDelayTime::create(kDuration)
		, CCCallFunc::create(this, callfunc_selector(StarsLayer::starInitDone)),NULL));
}

void StarsLayer::starInitDone()
{
	for (size_t i = 0; i < m_starsSprite.size(); ++i)
	{
		m_starsSprite[i]->showBornAnimation();
	}
	StageLayersMgr::theMgr()->initStarDone();
	StarsController::theModel()->preOneRound();
}

StarViewNode *StarsLayer::getClickedStar(CCPoint pos)
{
	for (size_t i = 0; i < m_starsSprite.size(); ++i)
	{
		StarViewNode *star = m_starsSprite[i];
		if (star->boundingBox().containsPoint(pos))
		{
			return star;
		}
	}
	return NULL;
}

bool StarsLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
	CCPoint touchLocation = pTouch->getLocation();
	CCPoint pos = m_clippingNode->convertToNodeSpace(touchLocation);
	StarViewNode *star = getClickedStar(pos);
	if (star == NULL) return false;
	
	m_stateOwner->clickStar(star->getGrid());
	return true;
}

//左下第一个grid为（0，0）
CCPoint StarsLayer::getPosByGrid(LogicGrid grid)
{
	CCPoint pos;
	pos.x = STAR_SIZE * (grid.x + 0.5f);
	pos.y = STAR_SIZE * (grid.y + 0.5f);
	return pos;
}

void StarsLayer::removeStar(StarViewNode *node)
{
	node->removeFromParent();
	auto iter = find(m_starsSprite.begin(), m_starsSprite.end(), node);
	if (iter != m_starsSprite.end())
	{
		m_starsSprite.erase(iter);
	}
}

void StarsLayer::onCreateNewStar(StarNode *node)
{
	auto pStarSprite = StarViewNode::create(node);
	auto grid = node->getAttr().grid;
	if (pStarSprite)
	{
		pStarSprite->setAnchorPoint(ccp(0.5f, 0.5f));
		CCPoint pos = getPosByGrid(grid);
		pStarSprite->setPosition(pos);
		pStarSprite->showBornAnimation();
		m_clippingNode->addChild(pStarSprite);
		//addChild(pStarSprite);
		m_starsSprite.push_back(pStarSprite);
	}
}

void StarsLayer::onToNormalState()
{
}
