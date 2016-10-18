#include "StageTargetView.h"
#include "UiLayout.h"
#include <vector>
#include "CommonUtil.h"
#include "EmptyBox.h"
#include <algorithm>

USING_NS_CC;
using namespace std;
using namespace CommonUtil;

StageTargetView *StageTargetView::create(const EraseStarsData &data)
{
	StageTargetView *view = new StageTargetView(data);
	view->init();
	view->autorelease();
	return view;
}

StageTargetView::StageTargetView(const EraseStarsData &data)
: m_targetData(data)
{
}

void StageTargetView::onEnter()
{
	CCNode::onEnter();
	StarsController::theModel()->addView(this);
}

void StageTargetView::onExit()
{
	CCNode::onExit();
	StarsController::theModel()->removeView(this);
}

bool StageTargetView::init()
{
	m_layout = UiLayout::create("layout/stage_target.xml");
	m_layout->setScale(0.65f);
	setContentSize(m_layout->boundingBox().size);
	addChild(m_layout);
	initLayout();
	m_layout->getChildById(5)->setVisible(false);
	return true;
}

void StageTargetView::initLayout()
{
	StarAttr attr;
	attr.type = m_targetData.starType;
	attr.color = m_targetData.color;
	auto node = StarNode::createNodeFatory(attr);
	string path = node->getResPath();
	string extraPath = node->getExtraResPath();
	bool hasExtraRes = !extraPath.empty();
	delete node;

	CCSprite *extraImg = dynamic_cast<CCSprite *>((m_layout->getChildById(2)));
	if (hasExtraRes)
	{
		extraImg->initWithFile(extraPath.c_str());
	}

	CCSprite *img = dynamic_cast<CCSprite *>((m_layout->getChildById(1)));
	img->initWithFile(path.c_str());
	
	extraImg->setVisible(hasExtraRes);
	img->setVisible(!hasExtraRes);

	refreshNum();
}

void StageTargetView::showBornAnimation(string path)
{
	auto size = getContentSize();
	if (!path.empty())
	{
		CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo(path.c_str());

		int pos1 = path.rfind("/");
		int pos2 = path.rfind(".");
		string armatureName = path.substr(pos1 + 1, pos2 - pos1 - 1);

		CCArmature *node = CCArmature::create(armatureName.c_str());
		node->getAnimation()->playWithIndex(0);
		node->setPosition(ccp(size.width * 0.5, size.height * 0.5f));
		EmptyBox *box = dynamic_cast<EmptyBox *>((m_layout->getChildById(3)));
		box->setNode(node);
	}
}

void StageTargetView::onStarRemove()
{
	auto target = StarsController::theModel()->getStageTarget();
	auto leftTarget = target->getEraseStarsLeft();
	auto iter = find_if(leftTarget.begin(), leftTarget.end(), [=](EraseStarsData data)->bool
	{
		return data.starType == m_targetData.starType && data.color == m_targetData.color;
	});

	assert(iter != leftTarget.end());
	if (iter != leftTarget.end())
	{
		m_targetData = *iter;
		refreshNum();
	}

}

void StageTargetView::refreshNum()
{
	CCLabelAtlas *leftNum = dynamic_cast<CCLabelAtlas *>((m_layout->getChildById(4)));
	leftNum->setString(intToStr(m_targetData.num));
	leftNum->setVisible(m_targetData.num > 0);

	m_layout->getChildById(5)->setVisible(m_targetData.num <= 0);
}

