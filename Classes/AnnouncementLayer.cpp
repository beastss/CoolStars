#include "AnnouncementLayer.h"
#include "CCFunctionAction.h"
#include "ViewUtil.h"
USING_NS_CC;
using namespace std;

bool AnnouncementLayer::init()
{
	auto winSize = CCDirector::sharedDirector()->getWinSize();
	setContentSize(winSize);

	initPanel();
	return true;
}

void AnnouncementLayer::initPanel()
{

}

void AnnouncementLayer::showTips(const char *str)
{
	CCNode *node = CCNode::create();
	CCSprite * bk = CCSprite::create("ranking/phb_zj.png");
	node->setContentSize(bk->getContentSize());
	bk->setPosition(ccpMult(bk->getContentSize(), 0.5f));
	node->addChild(bk);

	CCLabelTTF *text = CCLabelTTF::create(str, "Arial", 25);
	text->setColor(ccc3(0, 0, 0));
	text->setPosition(ccpMult(node->getContentSize(), 0.5f));
	node->addChild(text);

	node->setAnchorPoint(ccp(0.5f, 0.5f));
	auto winSize = CCDirector::sharedDirector()->getWinSize();
	node->setPosition(ccp(winSize.width * 0.5f, winSize.height * 0.7f));
	addChild(node);

	auto func = CCFunctionAction::create([=]()
	{
		node->removeFromParent();
	});
	float kFadeInTime = 0.3f;
	float kStayTime = 2.0f;
	float kFadeOutTime = 0.2f;

	auto fadeIn = CCFunctionAction::create([=]()
	{
		node->runAction(CCEaseBackInOut::create(CCMoveBy::create(0.2f, ccp(0, 10))));
		view_util::runFadeInAction(node, kFadeInTime);
	});
	auto fadeOut = CCFunctionAction::create([=]()
	{
		node->runAction(CCEaseBackInOut::create(CCMoveBy::create(0.2f, ccp(0, -10))));
		view_util::runFadeOutAction(node, kFadeOutTime);
	});
	node->runAction(CCSequence::create(fadeIn, CCDelayTime::create(kStayTime), fadeOut, CCDelayTime::create(kFadeOutTime), func, NULL));
}

void AnnouncementLayer::addAnnouncement(const char *picPath)
{
	m_announcement.push_back(picPath);
	if (m_announcement.size() == 1)
	{
		showAnnouncement();
	}
}

void AnnouncementLayer::showAnnouncement()
{
	if (m_announcement.empty()) return;

	auto size = getContentSize();
	auto nodeSize = CCSize(size.width, 50);

	CCNode *node = CCNode::create();
	node->setContentSize(nodeSize);
	node->setPosition(ccp(0, size.height * 0.8f));
	addChild(node);

	auto bk = CCLayerColor::create(ccc4(0, 0, 0, 120));
	bk->setContentSize(nodeSize);
	node->addChild(bk);

	auto spr = CCSprite::create(m_announcement.front().c_str());
	auto sprSize = spr->getContentSize();
	spr->setPosition(ccp(nodeSize.width + sprSize.width * 0.7f, nodeSize.height * 0.5f));
	node->addChild(spr);

	auto moveIn = CCMoveTo::create(3.0f, ccp(nodeSize.width * 0.5f, nodeSize.height * 0.5f));
	auto stay = CCDelayTime::create(3.0f);
	auto moveOut = CCMoveTo::create(3.0f, ccp(-sprSize.width * 0.7f, nodeSize.height * 0.5f));
	auto func = CCFunctionAction::create([=]()
	{
		m_announcement.pop_front();
		node->removeFromParent();
		showAnnouncement();
	});

	spr->runAction(CCSequence::create(moveIn, stay, moveOut, func, NULL));
}