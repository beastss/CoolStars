#include "StageBonusView.h"
#include <unordered_map>
#include "UiLayout.h"
#include "CommonMacros.h"
#include "CCFunctionAction.h"
#include "CommonUtil.h"
USING_NS_CC;
using namespace std;

StageBonusView::StageBonusView(const GoodsData &data)
: m_data(data)
{
	m_goodsRes[kGoodsDiamond] = "common/title_diamond.png";
	m_goodsRes[kGoodsFood] = "common/title_food.png";
	m_goodsRes[kGoodsStrength] = "common/title_strength.png";
	m_goodsRes[kGoodsKey] = "stage/stars/key.png";
}

StageBonusView *StageBonusView::create(const GoodsData &data)
{
	auto view = new StageBonusView(data);
	view->init();
	view->autorelease();
	return view;
}

bool StageBonusView::init()
{
	//暂时处理如果物品种类不是以下 则不显示
	if (m_goodsRes.find(m_data.type) == m_goodsRes.end()) return false;

	m_layout = UiLayout::create("layout/stage_bonus_view.xml");
	initLayout();
	addChild(m_layout);

	setContentSize(m_layout->getContentSize());
	setAnchorPoint(ccp(0.5f, 0));
	return true;
}

void StageBonusView::initLayout()
{
	CCSprite *spr = dynamic_cast<CCSprite *>(m_layout->getChildById(1));
	spr->initWithFile(m_goodsRes[m_data.type].c_str());
}

void StageBonusView::scaleAndFadeOut()
{
	auto size = getContentSize();
	float delayTime = 0.2f;
	auto fadeoutFunc = CCFunctionAction::create(bind(&StageBonusView::fadeout, this, delayTime));
	auto removeFunc = CCFunctionAction::create([=]()
	{
		removeFromParent();
	});

	CCScaleTo *scale = CCScaleTo::create(1.0f, 1.4f);
	CCMoveBy *move = CCMoveBy::create(1.0f, ccp(size.width * 0.1f, size.height * 0.1f));
	auto spawn = CCSpawn::create(scale, CCEaseExponentialInOut::create(move), NULL);
	auto delay = CCDelayTime::create(delayTime);
	runAction(CCSequence::create(spawn, CCDelayTime::create(0.5f), fadeoutFunc, delay, removeFunc, NULL));
}

void StageBonusView::fadeout(float duration)
{
	CCSprite *spr = dynamic_cast<CCSprite *>(m_layout->getChildById(1));
	spr->runAction(CCFadeOut::create(duration));
	CCLabelAtlas *num = dynamic_cast<CCLabelAtlas *>(m_layout->getChildById(2));
	num->runAction(CCFadeOut::create(duration));
}