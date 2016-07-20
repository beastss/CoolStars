#include "GoodsView.h"
#include <unordered_map>
#include "UiLayout.h"
#include "CommonMacros.h"
#include "CCFunctionAction.h"
#include "CommonUtil.h"
USING_NS_CC;
using namespace std;

GoodsView::GoodsView(const GoodsData &data)
: m_data(data)
{
	m_goodsRes[kGoodsDiamond] = "common/title_diamond.png";
	m_goodsRes[kGoodsFood] = "common/title_food.png";
	m_goodsRes[kGoodsStrength] = "common/title_strength.png";
	m_goodsRes[kGoodsKey] = "common/title_strength.png";
}

GoodsView *GoodsView::create(const GoodsData &data)
{
	auto view = new GoodsView(data);
	view->init();
	view->autorelease();
	return view;
}

bool GoodsView::init()
{
	//暂时处理如果物品种类不是以下 则不显示
	if (m_goodsRes.find(m_data.type) == m_goodsRes.end()) return false;


	m_layout = UiLayout::create("layout/goods_view.xml");
	initLayout();
	addChild(m_layout);

	setContentSize(m_layout->getContentSize());
	setAnchorPoint(ccp(0.5f, 0));
	return true;
}

void GoodsView::initLayout()
{
	CCSprite *spr = dynamic_cast<CCSprite *>(m_layout->getChildById(1));
	spr->initWithFile(m_goodsRes[m_data.type].c_str());

	CCLabelAtlas *num = dynamic_cast<CCLabelAtlas *>(m_layout->getChildById(2));
	num->setString(CommonUtil::intToStr(m_data.amount));
	num->setVisible(false);
}

float GoodsView::runMoveAction(std::function<void()> handle)
{
	float kMoveDuration = 0.5f;
	float kDelayDuration = 1.0f;
	CCMoveBy *moveBy = CCMoveBy::create(kMoveDuration, ccp(0, 30));
	auto showNumFunc = CCFunctionAction::create([=]()
	{
		auto node = m_layout->getChildById(2);
		node->setVisible(true);
		node->runAction(CCFadeIn::create(0.3f));
	});
	auto removeFunc = CCFunctionAction::create([=]()
	{
		//removeFromParent();
		if (handle)
		{
			handle();
		}
	});

	auto seq = CCSequence::create(CCEaseBackInOut::create(moveBy), showNumFunc, CCDelayTime::create(kDelayDuration), removeFunc, NULL);
	runAction(seq);

	return kMoveDuration + kDelayDuration;
}
