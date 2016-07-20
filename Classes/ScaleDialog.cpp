#include "ScaleDialog.h"
#include "CommonMacros.h"
using namespace std;
USING_NS_CC; 
ScaleDialog::ScaleDialog()
: m_touchPriority(kDialogTouchPriority)
, m_isRunScale(true)
{

}

void ScaleDialog::runScale()
{
	setScale(0.2f);

	CCScaleTo *scaleLarge = CCScaleTo::create(0.15f, 1.3f);
	CCScaleTo *scaleNormal = CCScaleTo::create(0.1f, 1.0f);
	CCSequence *seq = CCSequence::create(scaleLarge, scaleNormal, NULL);
	
	runAction(seq);
}

//使用前要设置contentSize
void ScaleDialog::addMaskLayer()
{
	CCLayerColor *mask = CCLayerColor::create(ccc4(0, 0, 0, 175));
	mask->ignoreAnchorPointForPosition(false);
	auto size = getContentSize();
	mask->setPosition(ccp(size.width * 0.5f, size.height * 0.5f));
	
	addChild(mask, -1);
}

void ScaleDialog::onEnter()
{
	CCNode::onEnter();
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, kDialogTouchPriority + 1, true);
	if (m_isRunScale)
	{
		runScale();
	}
}

void ScaleDialog::onExit()
{
	CCNode::onExit();
	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
}

bool ScaleDialog::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
	return true;
}