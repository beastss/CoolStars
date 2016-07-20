#include "PropItemView.h"
#include "PropManager.h"
#include "CommonUtil.h"
#include "StageOperator.h"
#include "UiLayout.h"
using namespace cocos2d;
using namespace std;
using namespace CommonUtil;

PropItemView *PropItemView::create(int type, int touchPriority)
{
	PropItemView *view = new PropItemView(type, touchPriority);
	view->init();
	view->autorelease();
	return view;
}

PropItemView::PropItemView(int type, int touchPriority)
: TouchNode(touchPriority)
, m_type(type)
{

}

void PropItemView::onEnter()
{
	TouchNode::onEnter();
	PropManager::propMgr()->addView(this);
}

void PropItemView::onExit()
{
	TouchNode::onExit();
	PropManager::propMgr()->removeView(this);
}

bool PropItemView::init()
{
	m_layout = UiLayout::create("layout/props_item.xml");

	auto path = DataManagerSelf->getPropsConfig(m_type).resPath;
	auto icon = dynamic_cast<CCSprite *>(m_layout->getChildById(3));
	icon->initWithFile(path.c_str());
	refreshItemNum();

	setContentSize(m_layout->getContentSize());
	addChild(m_layout);
	return true;
}

void PropItemView::refreshItemNum()
{
	CCLabelAtlas *num = dynamic_cast<CCLabelAtlas *>(m_layout->getChildById(2));
	auto mgr = PropManager::propMgr();
	int amount = mgr->getPropItemAmount(m_type);
	string str = ";";
	str += intToStr(amount);
	num->setString(str.c_str());
}

void PropItemView::runScale()
{
	CCScaleTo *scaleLarge = CCScaleTo::create(0.15f, 1.3f);
	CCScaleTo *scaleSmall = CCScaleTo::create(0.15f, 0.8f);
	CCScaleTo *scaleNormal = CCScaleTo::create(0.1f, 1.0f);
	CCSequence *seq = CCSequence::create(scaleLarge, scaleSmall, scaleNormal, NULL);

	runAction(seq);
}

bool PropItemView::onTouchBegan(cocos2d::CCPoint pt, bool isInside)
{
	if (isInside)
	{
		runScale();
		if (m_touchHandle)
		{
			m_touchHandle(m_type);
		}
	}
	return isInside;
}

void PropItemView::onPropItemChanged()
{
	refreshItemNum();
}
