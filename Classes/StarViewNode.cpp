#include "StarViewNode.h"
#include "StarsLayer.h"
#include "StarsController.h"
#include "StarsEraseModule.h"
#include "ActionRunner.h"
#include "CCFunctionAction.h"
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
	m_runner = ActionRunner::create();
	m_runner->retain();
}

StarViewNode::~StarViewNode()
{
	m_runner->clear();
	m_runner->release();
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

void StarViewNode::doMove(LogicGrid targetGrid, int direction)
{
	LogicGrid oldGrid = m_model->getAttr().grid;
	//计算到达目标相差的格子数
	int step = max(abs(targetGrid.x - oldGrid.x), abs(targetGrid.y - oldGrid.y));
	const float kInterval = CCDirector::sharedDirector()->getAnimationInterval();
	const float kOneStepTime = 0.1f;
	const float kDuration = kOneStepTime * step;
	const int kFrames = kDuration / kInterval;

	StarsController::theModel()->recordStarMoving(m_model);
	m_runner->queueAction(CallFuncAction::withFunctor([=]
	{
		CCPoint pos = getPosByGrid(targetGrid);
		//CCLOG("count:%d", m_runner->count());
		//移动的最后一个格子，播放弹一下的动画
		if (m_runner->count() == 2)
		{
			int vec[5][2] = { {0, 0}, { 0, 1 }, { 0, -1 }, { -1, 0 }, { 1, 0 } };
			const float kOffset = 20;
			auto tempPos = ccp(pos.x + kOffset * vec[direction][0], pos.y + kOffset * vec[direction][1]);
			CCMoveTo *moveTo1 = CCMoveTo::create(kDuration, tempPos);
			CCMoveTo *moveTo2 = CCMoveTo::create(kOneStepTime * 0.4f, pos);
			auto func = CCFunctionAction::create([=]()
			{
				StarsController::theModel()->moveStarFinished(m_model);
			});
			runAction(CCSequence::create(moveTo1, moveTo2, func, NULL));
		}
		else
		{
			CCMoveTo *moveTo = CCMoveTo::create(kDuration, pos);
			runAction(CCActionEase::create(moveTo));
		}
	}));
	m_runner->queueAction(DelayNFrames::delay(kFrames + 1)); //延迟一帧 不然星星移动有偏移，星星移动时间和delay不会完全相同
}

void StarViewNode::doEraseAction()
{
	StarsEraseModule::theModel()->eraseStarBegan();
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
