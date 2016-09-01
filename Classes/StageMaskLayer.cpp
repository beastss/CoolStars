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
void StarMaskOperator::onHighLightStars(const std::vector<LogicGrid> &grids, int radiusX, int radiusY)
{
	m_stars.clear();
	for (size_t i = 0; i < grids.size(); ++i)
	{
		auto node = StarsController::theModel()->getStarNode(grids[i]);
		auto attr = node->getAttr();
		auto view = node->getView();
		auto resPath = node->getResPath();
		string extraPath = node->getExtraResPath();
		bool hasExtraRes = !extraPath.empty();

		auto pos = view->getParent()->convertToWorldSpace(view->getPosition());
		CCSprite *starSpr = CCSprite::create(resPath.c_str());
		starSpr->setPosition(pos);
		m_stars.insert(make_pair(starSpr, attr.grid));
		m_layer->addNode(starSpr);

		if (hasExtraRes)
		{
			CCSprite *extraImg = CCSprite::create(extraPath.c_str());
			extraImg->setPosition(pos);
			m_layer->addNode(extraImg);
		}
	}

	//初始化为无效grid
	m_curGrid.reset();

	m_radiusX = radiusX;
	m_radiusY = radiusY;
}

bool StarMaskOperator::onTouchBegan(cocos2d::CCTouch *pTouch)
{
	OnTouchMoved(pTouch);
	return true;
}

void StarMaskOperator::OnTouchMoved(cocos2d::CCTouch *pTouch)
{
	bool selected = false;
	LogicGrid temp;
	for (auto iter = m_stars.begin(); iter != m_stars.end(); ++iter)
	{
		auto node = iter->first;
		auto pos = node->getParent()->convertToNodeSpace(pTouch->getLocation());
		if (node->boundingBox().containsPoint(pos))
		{
			temp = iter->second;
			selected = true;
			break;
		}
	}
	
	if (selected)
	{
		if (!(temp == m_curGrid))
		{
			m_curGrid = temp;
			m_layer->setNodesVisible(false);
			m_layer->highLightRect(temp.x, temp.y, m_radiusX, m_radiusY);
		}
	}
	else
	{
		m_curGrid.reset();
		m_layer->setNodesVisible(true);
		m_layer->removeRectHightLight();
	}
}

void StarMaskOperator::onTouchEnd(cocos2d::CCTouch *pTouch)
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

StageMaskLayer::StageMaskLayer()
: m_curOp(NULL)
{
	m_colorStarOp = new StarMaskOperator(this);
	m_petOp = new PetMaskOperator(this);
}

bool StageMaskLayer::init()
{
	auto winSize = CCDirector::sharedDirector()->getWinSize();
	setContentSize(winSize);

	CCLayerColor *mask = CCLayerColor::create(ccc4(0, 0, 0, 175));
	mask->setContentSize(winSize);
	addChild(mask);
	m_container = CCNode::create();
	m_rectNodes = CCNode::create();
	addChild(m_container);
	addChild(m_rectNodes);
	
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
	if (isVisible())
	{
		m_curOp->OnTouchMoved(pTouch);
	}
}

void StageMaskLayer::ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
	if (isVisible())
	{
		m_curOp->onTouchEnd(pTouch);
	}
}

void StageMaskLayer::onHighLightStars(const std::vector<LogicGrid> &grids, int radiusX, int radiusY)
{
	m_curOp = m_colorStarOp;
	setVisible(true);
	setNodesVisible(true);
	m_colorStarOp->onHighLightStars(grids, radiusX, radiusY);
}

void StageMaskLayer::initPetViewsInfo(std::unordered_map<int, cocos2d::CCPoint> info)
{
	m_petsInfo = info;
}

void StageMaskLayer::onHighLightPets(const std::vector<int> &petIds)
{	
	m_curOp = m_petOp;
	setVisible(true);
	setNodesVisible(true);
	m_petOp->onHighLightPets(petIds);
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
	removeRectHightLight();
	GuideMgr::theMgr()->endGuide(kGuideStart_hide_skill_mask_layer);
}

void StageMaskLayer::addNode(cocos2d::CCNode *node)
{
	m_container->addChild(node);
}

void StageMaskLayer::highLightRect(int x, int y, int radiusX, int radiusY)
{
	removeRectHightLight();
	//如果是炸弹，全屏
	auto node = StarsController::theModel()->getStarNode(LogicGrid(x, y));
	if (node && node->getAttr().type == kBomb)
	{
		radiusX = COlUMNS_SIZE; 
		radiusY = ROWS_SIZE;
	}

	for (int i = x - radiusX; i <= x + radiusX; ++i)
	{
		for (int j = y - radiusY; j <= y + radiusY; ++j)
		{
			auto node = StarsController::theModel()->getStarNode(LogicGrid(i, j));
			if (!node || !node->canBeRemoved()) continue;
			auto view = node->getView();
			auto resPath = node->getResPath();
			string extraPath = node->getExtraResPath();
			bool hasExtraRes = !extraPath.empty();

			auto pos = view->getParent()->convertToWorldSpace(view->getPosition());
			CCSprite *starSpr = CCSprite::create(resPath.c_str());
			starSpr->setPosition(pos);
			m_rectNodes->addChild(starSpr);

			if (hasExtraRes)
			{
				CCSprite *extraImg = CCSprite::create(extraPath.c_str());
				extraImg->setPosition(pos);
				m_rectNodes->addChild(extraImg);
			}
		}
	}
}

void StageMaskLayer::removeRectHightLight()
{
	m_rectNodes->removeAllChildren();
}