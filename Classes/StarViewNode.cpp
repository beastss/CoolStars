#include "StarViewNode.h"
#include "StarsLayer.h"
#include "StarsController.h"
USING_NS_CC;
using namespace std;

StarViewNode *StarViewNode::create(StarNode *node)
{
	StarViewNode *ViewNode = new StarViewNode(node);
	ViewNode->autorelease();
	ViewNode->init();
	return ViewNode;
}

StarViewNode::StarViewNode(StarNode *node)
: m_model(node)
, m_isExploded(false)
{
    m_model->bindView(this);
}

StarViewNode::~StarViewNode()
{
}
bool StarViewNode::init()
{
	string fileName = m_model->getResPath();
	CCSprite *spr = CCSprite::create(fileName.c_str());
	CCSize size = spr->getContentSize();
	spr->setPosition(ccp(size.width * 0.5, size.height * 0.5f));
	setContentSize(size);
	addChild(spr);
	
	return true;
}

void StarViewNode::showBornAnimation()
{
	auto size = getContentSize();
	string path = m_model->getBornAnimationPath();
	if (!path.empty())
	{
		CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo(path.c_str());

		int pos1 = path.rfind("/");
		int pos2 = path.rfind(".");
		string armatureName = path.substr(pos1 + 1, pos2 - pos1 - 1);

		CCArmature *node = CCArmature::create(armatureName.c_str());
		node->getAnimation()->playWithIndex(0);
		node->setPosition(ccp(size.width * 0.5, size.height * 0.5f));
		addChild(node);
	}
}

//左下第一个grid为（0，0)
cocos2d::CCPoint StarViewNode::getPosByGrid(LogicGrid grid)
{
	CCPoint pos;
	pos.x = STAR_SIZE * (grid.x + 0.5f);
	pos.y = STAR_SIZE * (grid.y + 0.5f);
	return pos;
}

void StarViewNode::doMove(LogicGrid targetGrid)
{
	stopAllActions();
	CCPoint pos = getPosByGrid(targetGrid);
    CCMoveTo *moveTo = CCMoveTo::create(0.4f, pos);
	runAction(CCEaseBackInOut::create(moveTo));
}

void StarViewNode::doEraseAction()
{
	auto attr = m_model->getAttr();
	auto pos = getParent()->convertToWorldSpace(getPosition());
	StageLayersMgr::theMgr()->starErased(pos, attr.type, attr.color);
	
	playParticle();
}

void StarViewNode::playParticle()
{
	CCParticleExplosion *pEmitter = CCParticleExplosion::create();
	string fileImage = m_model->getExplosionPath();
	if (fileImage.empty()) return;
	pEmitter->setTexture(CCTextureCache::sharedTextureCache()->addImage(fileImage.c_str()));
	pEmitter->setAutoRemoveOnFinish(true);

	pEmitter->setPosition(getPosition());

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

	getParent()->addChild(pEmitter);
}

void StarViewNode::removeExplosionAnimation (cocos2d::extension::CCArmature *animation, cocos2d::extension::MovementEventType, const char *)
{
	animation->removeFromParent();
}

void StarViewNode::removeSelf(bool withAction)
{
	if (withAction)
	{
		doEraseAction();
	}
	StarsLayer *layer = dynamic_cast<StarsLayer *>(getParent()->getParent());
	layer->removeStar(this);
}
