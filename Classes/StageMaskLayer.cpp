#include "StageMaskLayer.h"
#include "CommonMacros.h"
#include "DataManager.h"
#include "StarNode.h"
#include "StarViewNode.h"
#include "StageLayersMgr.h"
#include "StageScene.h"
#include "StagePetNode.h"
#include "GuideMgr.h"
USING_NS_CC;
using namespace std;
/*
void MaskOperator::addNode(cocos2d::CCNode *node)
{
	m_layer->addNode(node); 
}

*/

///////////////////////////////////////////////////
void StarColorMaskOperator::onHighLightStars(int color)
{
	m_stars.clear();
	auto nodes = StarsController::theModel()->getStarNodes();
	for (size_t i = 0; i < nodes.size(); ++i)
	{
		auto attr = nodes[i]->getAttr();
		if (attr.type == kColorStar && attr.color == color)
		{
			auto view = nodes[i]->getView();
			auto resPath = nodes[i]->getResPath();
			auto pos = view->getParent()->convertToWorldSpace(view->getPosition());
			CCSprite *starSpr = CCSprite::create(resPath.c_str());
			starSpr->setPosition(pos);
			m_stars.insert(make_pair(starSpr, attr.grid));

			m_layer->addNode(starSpr);
		}
	}
}

void StarColorMaskOperator::onTouchEnd(cocos2d::CCTouch *pTouch)
{
	for (auto iter = m_stars.begin(); iter != m_stars.end(); ++iter)
	{
		auto node = iter->first;
		auto pos = node->getParent()->convertToNodeSpace(pTouch->getLocation());
		if (node->boundingBox().containsPoint(pos))
		{
			auto stateOwner = StageScene::theScene()->getStateOwner();
			stateOwner->clickStar(iter->second);
		}
	}

	m_layer->endMask();
}

///////////////////////////////////////////////////
void PetMaskOperator::onHighLightPets(const std::vector<int> &petIds)
{
	for (size_t i = 0; i < petIds.size(); ++i)
	{
		int tempId = petIds[i];
		auto petInfo = m_layer->getPetViewInfo();
		auto iter = petInfo.find(tempId);
		if (iter != petInfo.end())
		{
			StagePetNode *petNode = StagePetNode::create(tempId, kStageMaskTouchPriority - 1);
			petNode->setPosition(iter->second);
			petNode->setTouchHandle([=](int petId)
			{
				auto stateOwner = StageScene::theScene()->getStateOwner();
				stateOwner->clickPet(petId);
				m_layer->endMask();
			});
			m_layer->addNode(petNode);
		}
	}
}

void PetMaskOperator::onTouchEnd(cocos2d::CCTouch *pTouch)
{
	m_layer->endMask();
}

///////////////////////////////////////////////////
void StarRectMaskOperator::onHighLightRectStars(int x, int y, int width, int height)
{
	m_stars.clear();
	for (int i = x; i < x + width; ++i)
	{
		for (int j = y; j < y + height; ++j)
		{
			auto node = StarsController::theModel()->getStarNode(LogicGrid(i, j));
			if (!node) continue;
			auto view = node->getView();
			auto resPath = node->getResPath();
			auto pos = view->getParent()->convertToWorldSpace(view->getPosition());
			CCSprite *starSpr = CCSprite::create(resPath.c_str());
			starSpr->setPosition(pos);
			m_layer->addNode(starSpr);
			m_stars.insert(make_pair(starSpr, LogicGrid(i, j)));
		}
	}
}

void StarRectMaskOperator::onTouchEnd(cocos2d::CCTouch *pTouch)
{
	for (auto iter = m_stars.begin(); iter != m_stars.end(); ++iter)
	{
		auto node = iter->first;
		auto pos = node->getParent()->convertToNodeSpace(pTouch->getLocation());
		if (node->boundingBox().containsPoint(pos))
		{
			auto stateOwner = StageScene::theScene()->getStateOwner();
			stateOwner->clickStar(iter->second);
		}
	}

	m_layer->endMask();
}
///////////////////////////////////////////////////
StageMaskLayer::StageMaskLayer()
: m_curOp(NULL)
{
	m_colorStarOp = new StarColorMaskOperator(this);
	m_petOp = new PetMaskOperator(this);
	m_starRectOp = new StarRectMaskOperator(this);
}

bool StageMaskLayer::init()
{
	auto winSize = CCDirector::sharedDirector()->getWinSize();
	setContentSize(winSize);

	CCLayerColor *mask = CCLayerColor::create(ccc4(0, 0, 0, 175));
	mask->setContentSize(winSize);
	addChild(mask);
	m_container = CCNode::create();
	addChild(m_container);
	
	setVisible(false);

	return true;
}

void StageMaskLayer::onEnter()
{
	CCNode::onEnter();
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, kStageMaskTouchPriority, true);
	StageLayersMgr::theMgr()->addLayer(this);
}

void StageMaskLayer::onExit()
{
	CCNode::onExit();
	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
	StageLayersMgr::theMgr()->removeLayer(this);

}

bool StageMaskLayer::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
	if (!isVisible()) return false;

	return m_curOp->onTouchBegan(pTouch);
}

void StageMaskLayer::ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
	m_curOp->OnTouchMoved(pTouch);
}

void StageMaskLayer::ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
	m_curOp->onTouchEnd(pTouch);
}

void StageMaskLayer::onHighLightStars(int color)
{
	m_curOp = m_colorStarOp;
	setVisible(true);
	m_colorStarOp->onHighLightStars(color);
}

void StageMaskLayer::initPetViewsInfo(std::unordered_map<int, cocos2d::CCPoint> info)
{
	m_petsInfo = info;
}

void StageMaskLayer::onHighLightPets(const std::vector<int> &petIds)
{	
	m_curOp = m_petOp;
	setVisible(true);
	m_petOp->onHighLightPets(petIds);
}

void StageMaskLayer::onHighLightRectStars(int x, int y, int width, int height)
{
	m_curOp = m_starRectOp;
	setVisible(true);
	m_starRectOp->onHighLightRectStars(x, y, width, height);
}

void StageMaskLayer::endMask(bool toNormalState)
{
	if (toNormalState)
	{
		auto stateOwner = StageScene::theScene()->getStateOwner();
		stateOwner->enterNormalState();
	}

	setVisible(false);
	m_container->removeAllChildren();
	GuideMgr::theMgr()->endGuide(kGuideStart_hide_skill_mask_layer);
}

void StageMaskLayer::addNode(cocos2d::CCNode *node)
{
	m_container->addChild(node);
}
