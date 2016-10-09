#include "StarsLayer.h"
#include "StarViewNode.h"
#include "StarsController.h"
#include "StageSceneState.h"
#include "GuideMgr.h"
#include "UiLayout.h"
#include "EmptyBox.h"
#include "StageScene.h"
#include <algorithm>
#include "StageDataMgr.h"
#include "StarsUtil.h"
#include "StarBkGrid.h"

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
	auto range = StarsUtil::usedRange();
	//CCSize size = CCSize(STAR_SIZE * COlUMNS_SIZE, STAR_SIZE * ROWS_SIZE);
	CCSize size = CCSize(STAR_SIZE * range.cols, STAR_SIZE * range.rows);
	back->setContentSize(size);
	
	CCSprite *sp = CCSprite::create("shop/sd_zuanshi2.png");
	sp->setAnchorPoint(ccp(0, 0));
	m_clippingNode = CCClippingNode::create();
	m_clippingNode->setInverted(false);
	m_clippingNode->setAlphaThreshold(1.0f);
	m_clippingNode->setStencil(back);

	addChild(m_clippingNode);
	auto startPos = getStartPos();
	m_clippingNode->setPosition(startPos);

	auto maskBk = m_layout->getChildById(5);
	maskBk->setAnchorPoint(ccp(0, 0));
	maskBk->setScaleX((float)range.cols / COlUMNS_SIZE);
	maskBk->setScaleY((float)range.rows / ROWS_SIZE);
	maskBk->setPosition(m_layout->convertToNodeSpace(convertToWorldSpace(startPos)));
	m_layout->removeChild(m_layout->getChildById(5));
}

void StarsLayer::addBkGrids()
{
	float curHeight = 0;
	float curWidth = 0;
	CCNode *node = CCNode::create(); 
	for (int row = 0; row < ROWS_SIZE; ++row)
	{
		float height = 0;
		for (int col = 0; col < COlUMNS_SIZE; ++col)
		{
			auto grid = StarBkGrid::create(LogicGrid(col, row));
			node->addChild(grid);
			grid->setPosition(curWidth, curHeight);

			auto size = grid->getContentSize();
			curWidth += size.width;
			height = max(height, size.height);

		}
		curHeight += height;
		curWidth = 0;
	}
	addChild(node);
	node->setPosition(getStartPos());
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

CCPoint StarsLayer::getStartPos()
{
	//配表时，左边和右边没有空行，因为clippingnode剪切不了左下角区域

	auto range = StarsUtil::usedRange();
	auto winSize = CCDirector::sharedDirector()->getWinSize();
	//auto pos = m_layout->getChildById(1)->getPosition();
	auto pos = ccp(winSize.width * 0.05f, winSize.height *0.15f);
	pos.x += (COlUMNS_SIZE - range.cols) * STAR_SIZE / 2.0f;
	pos.y += (ROWS_SIZE - range.rows) * STAR_SIZE / 2.0f;
	return pos;
}
