#include "GuideView.h"
#include "UiLayout.h"
#include "CommonUtil.h"
#include "CommonMacros.h"
#include "DataManager.h"
#include "GuideMgr.h"
USING_NS_CC;
using namespace std;

float GuideView::kRefWidth = 480;
float GuideView::kRefHeight = 800;

GuideView::GuideView(int guideId)
: m_guideId(guideId)
, m_layout(NULL)
, m_textDialog(NULL)
{
}

GuideView *GuideView::create(int guideId)
{
	auto view = new GuideView(guideId);
	view->init();
	view->autorelease();
	return view;
}

void GuideView::onEnter()
{
	CCNode::onEnter();
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, kGuideTouchPriority, true);
}

void GuideView::onExit()
{
	CCNode::onExit();
	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
}

bool GuideView::init()
{
	m_layout = UiLayout::create("layout/guide_view.xml");
	auto winSize = CCDirector::sharedDirector()->getWinSize();
	setContentSize(winSize);

	initPanel();

	addChild(m_layout);
	return true;
}

void GuideView::initPanel()
{
	initTargetRect();
	initMask();
	initFinger();
	initTextDialog();
}

void GuideView::initTargetRect()
{
	auto config = DataManagerSelf->getGuideConfigById(m_guideId);
	auto rectVec = config->targetRect;
	auto winSize = CCDirector::sharedDirector()->getWinSize();
	CCSize rectSize = CCSize(rectVec[2], rectVec[3]);
	CCPoint rectPos = ccp(rectVec[0], rectVec[1]);

	switch (config->rectType)
	{
	case kRectTypeFixedPos:
		//固定坐标
		rectPos.y += (winSize.height - kRefHeight) * 0.5f;
		break;
	case kRectTypeBottom:
		break;
	case kRectTypeTop:
		rectPos.y += winSize.height - kRefHeight;
		break;
	default:
		break;
	}

	m_targetRect.setRect(rectPos.x, rectPos.y, rectSize.width, rectSize.height);
}

void GuideView::initFinger()
{
	auto config = DataManagerSelf->getGuideConfigById(m_guideId);
	//手指动画
	switch (config->fingerType)
	{
	case kFingerTypeHide:
		m_layout->getChildById(3)->setVisible(false);
		m_layout->getChildById(4)->setVisible(false);
		break;
	case kFingerTypeNormal:
	{
	  CCSprite *fingerSpr = dynamic_cast<CCSprite *>(m_layout->getChildById(3));
	  auto animation = CommonUtil::getFrameAnimation("guide/xszt_shouzi_%d.png", 2, 0.3f);
	  fingerSpr->runAction(CCRepeatForever::create(animation));
	  fingerSpr->setAnchorPoint(ccp(0, 1));
	  fingerSpr->setPosition(ccp(m_targetRect.getMaxX(), m_targetRect.getMinY()));
	  m_layout->getChildById(4)->setVisible(false);
	  break;
	}
	case kFingerTypeSlide:
	{
		m_layout->getChildById(3)->setVisible(false);
		auto size = m_layout->getChildById(4)->getContentSize();
		auto pos = m_targetRect.origin;
		pos.x += size.width * 0.34f;
		pos.y += size.height * 0.25f;
		m_layout->getChildById(4)->setPosition(pos);
		break;
	}
	default:
		break;
	}

}

void GuideView::initTextDialog()
{
	auto config = DataManagerSelf->getGuideConfigById(m_guideId);
	//文本对话框
	if (config->showTextDialog)
	{
		m_textDialog = UiLayout::create("layout/guide_text_dialog.xml");
		addChild(m_textDialog);
		auto dialogText = config->dialogText;
		CCSprite *textImg = dynamic_cast<CCSprite *>(m_textDialog->getChildById(2));
		textImg->initWithFile(dialogText.c_str());
		adjustTextDialogPos();
	}
}

void GuideView::adjustTextDialogPos()
{
	auto size = getContentSize();
	CCRect rect(0, 0, size.width, size.height * 0.5f);
	if (rect.intersectsRect(m_targetRect))
	{
		m_textDialog->setAnchorPoint(ccp(0, 1));
		m_textDialog->setPosition(ccp(0, size.height));
	}
	else
	{
		m_textDialog->setAnchorPoint(ccp(0, 0));
		m_textDialog->setPosition(ccp(0, 0));
	}
}

void GuideView::initMask()
{
	auto config = DataManagerSelf->getGuideConfigById(m_guideId);
	if (!config->showMask) return;

	auto winSize = CCDirector::sharedDirector()->getWinSize();
	CCLayerColor *mask = CCLayerColor::create(ccc4(0, 0, 0, 175));
	mask->setContentSize(winSize);

	CCLayerColor *stencil = CCLayerColor::create(ccc4(0, 0, 0, 255));
	stencil->setContentSize(CCSize(1, 1));
	stencil->setContentSize(m_targetRect.size);
	stencil->setPosition(m_targetRect.origin);

	auto clippingNode = CCClippingNode::create();
	clippingNode->setStencil(stencil);
	clippingNode->setInverted(true);
	clippingNode->addChild(mask);
	addChild(clippingNode);
}

bool GuideView::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
	GuideMgr::theMgr()->endGuide(kGuideEnd_guide_click);
	if (isTargetArea(pTouch->getLocation()))
	{
		//removeFromParent();
		return false;
	}
	return true;
}

void GuideView::ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
	return;
	CCPoint startPos = pTouch->getStartLocation();
	CCPoint curPos = pTouch->getLocation();
	if (isTargetArea(startPos) && isTargetArea(curPos))
	{
		removeFromParent();
	}
}

bool GuideView::isTargetArea(cocos2d::CCPoint worldPos)
{
	CCPoint pos = convertToNodeSpace(worldPos);
	return m_targetRect.containsPoint(pos);
}