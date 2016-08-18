#include "PropsGuideView.h"
#include "CCFunctionAction.h"
USING_NS_CC;
using namespace std;

bool PropsGuideView::init()
{
	auto winSize = CCDirector::sharedDirector()->getWinSize();
	setContentSize(winSize);

	CCLayerColor *mask = CCLayerColor::create(ccc4(0, 0, 0, 175));
	mask->setContentSize(winSize);
	addChild(mask);

	runScaleAction();
	return true;
}

void PropsGuideView::runScaleAction()
{
	auto size = getContentSize();
	CCSprite *spr = CCSprite::create("stage/ui/yxjm_wuxian.png");
	spr->setPosition(ccpMult(size, 0.5f));
	addChild(spr);

	auto func = CCFunctionAction::create([=]()
	{
		spr->removeFromParent();
		toPropsItem();
	});
	spr->setScale(0.8f);
	auto scaleLarge = CCScaleTo::create(1.0f, 1.3f);
	auto scaleNormal = CCScaleTo::create(0.6f, 1.0f);
	auto delay = CCDelayTime::create(0.5f);
	spr->runAction(CCSequence::create(scaleLarge, scaleNormal, delay, func, NULL));
}

void PropsGuideView::toPropsItem()
{
	auto size = getContentSize();
	for (size_t i = 0; i < m_pos.size(); ++i)
	{
		CCSprite *spr = CCSprite::create("stage/ui/yxjm_wuxian.png");
		spr->setPosition(ccpMult(size, 0.5f));
		addChild(spr);

		auto scale = CCScaleTo::create(0.5f, 0.3f);
		auto pos = convertToNodeSpace(m_pos[i]);
		auto move = CCMoveTo::create(0.5f, pos);

		spr->runAction(CCSpawn::create(scale, move, NULL));
	}
	
	auto func = CCFunctionAction::create([=]()
	{
		if (m_handle)
		{
			m_handle();
		}
		removeFromParentAndCleanup(true);
	});

	runAction(CCSequence::create(CCDelayTime::create(3.0f), func, NULL));
}
