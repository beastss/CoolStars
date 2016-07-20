#include "GuideEditLayer.h"
#include "CommonMacros.h"
#include "CommonUtil.h"
#include "UiLayout.h"
#include <fstream>
USING_NS_CC;
using namespace std;

GuideEditLayer *GuideEditLayer::create()
{
	auto layer = new GuideEditLayer();
	layer->init();
	layer->autorelease();
	return layer;
}

void GuideEditLayer::onEnter()
{
	CCNode::onEnter();
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, kGuideEditTouchPriority, true);
}

void GuideEditLayer::onExit()
{
	CCNode::onExit();
	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
}
bool GuideEditLayer::init()
{
	auto winSize = CCDirector::sharedDirector()->getWinSize();
	addMaskLayer();

	m_layout = UiLayout::create("guide_edit/guide_edit.xml");
	m_layout->setMenuTouchPriority(kGuideEditTouchPriority - 1);
	initPanel();
	addChild(m_layout);
	setContentSize(winSize);
	return true;
}

void GuideEditLayer::initPanel()
{
	CCMenuItemSprite *closeBtn = dynamic_cast<CCMenuItemSprite *>(m_layout->getChildById(4));
	closeBtn->setTarget(this, menu_selector(GuideEditLayer::onCloseBtnClicked));

	CCMenuItemSprite *saveBtn = dynamic_cast<CCMenuItemSprite *>(m_layout->getChildById(5));
	saveBtn->setTarget(this, menu_selector(GuideEditLayer::onSaveBtnClicked));
}

void GuideEditLayer::addMaskLayer()
{
	auto winSize = CCDirector::sharedDirector()->getWinSize();

	CCLayerColor *mask = CCLayerColor::create(ccc4(0, 0, 0, 125));
	mask->setContentSize(winSize);

	addChild(mask);
}

bool GuideEditLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
	removeChildByTag(kHighLightNodeTag);
	return true;
}

void GuideEditLayer::ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
	auto startPos = pTouch->getStartLocation();
	auto curPos = pTouch->getLocation();

	removeChildByTag(kHighLightNodeTag);
	auto highLightNode = CCLayerColor::create(ccc4(0, 255, 0, 150));
	highLightNode->ignoreAnchorPointForPosition(false);
	highLightNode->setAnchorPoint(ccp(0, 1));

	highLightNode->setContentSize(CCSize(curPos.x - startPos.x, startPos.y - curPos.y));
	highLightNode->setPosition(startPos);
	addChild(highLightNode, 1, kHighLightNodeTag);
}

void GuideEditLayer::ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{

}

void GuideEditLayer::onCloseBtnClicked(cocos2d::CCObject* pSender)
{
	removeFromParent();
}

void GuideEditLayer::onSaveBtnClicked(cocos2d::CCObject* pSender)
{
	CCNode *highLightNode = getChildByTag(kHighLightNodeTag);
	if (highLightNode)
	{
		ofstream out("guide_edit/guide_edit_output.txt");
		auto pos = highLightNode->getPosition();
		auto size = highLightNode->getContentSize();
		out << pos.x << ",";
		out << pos.y - size.height << ",";
		out << size.width << ",";
		out << size.height;

		out.close();
	}
}



