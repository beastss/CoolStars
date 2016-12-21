#include "TreasureStarView.h"
#include "TreasureStageDataMgr.h"
#include "TreasureStarData.h"
#include "TreasureStageState.h"
#include "TreasureStageMacro.h"
#include "TreasureStageNotifier.h"
USING_NS_CC;
using namespace std;

TreasureStarView::TreasureStarView(TreasureStarData *model)
: m_model(model)
{
	m_model->bindView(this);
}

TreasureStarView *TreasureStarView::create(TreasureStarData *model)
{
	TreasureStarView *view = new TreasureStarView(model);
	view->init();
	view->autorelease();
	return view;
}

bool TreasureStarView::init()
{
	setTouchPriority(kTreasureStarTouchPriority);
	auto size = CCSize(TREASURE_STAR_SIZE, TREASURE_STAR_SIZE);
	setAnchorPoint(ccp(0.5f, 0.5f));
	setContentSize(size);

	string path = TreasureStageDataMgrSelf->getStarData(m_model->getColor()).starPath;
	auto spr = CCSprite::create(path.c_str());
	auto sprSize = spr->getContentSize();
	spr->setScale(size.width / sprSize.width);
	spr->setPosition(ccpMult(size, 0.5f));
	addChild(spr);
	
	return true;
}

void TreasureStarView::remove()
{
	removeFromParent();
}

void TreasureStarView::doExplosion(int score)
{
	auto pos = getParent()->convertToWorldSpace(getPosition());
	TreasureStageNotifier::theNotifier()->onStarExplore(m_model->getColor(), score, pos);
	remove();
}

bool TreasureStarView::onTouchBegan(cocos2d::CCPoint pt, bool isInside)
{
	if (isInside && TreasureStageState::theState()->canTouch())
	{
		m_model->handleTouch();
		//setScale(1.5f);
		return true;
	}
	return false;
}

void TreasureStarView::doMove(const LogicGrid &offset)
{
	const float kDuration = 0.5f;
	CCPoint pt;
	pt.x = offset.x * TREASURE_STAR_SIZE;
	pt.y = offset.y * TREASURE_STAR_SIZE;
	auto *move = CCEaseBackInOut::create(CCMoveBy::create(kDuration, pt));
	runAction(move);
}