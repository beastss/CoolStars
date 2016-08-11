#include "ViewUtil.h"
#include "CCFunctionAction.h"
#include "MainScene.h"
USING_NS_CC;
using namespace std;

void view_util::runFadeInAction(CCNode *node, float duration)
{
	CCObject* pObj;
	CCARRAY_FOREACH(node->getChildren(), pObj)
	{
		CCNode* child = (CCNode*)pObj;
		child->runAction(CCFadeIn::create(duration));
	}
}

void view_util::runFadeOutAction(CCNode *node, float duration)
{
	CCObject* pObj;
	CCARRAY_FOREACH(node->getChildren(), pObj)
	{
		CCNode* child = (CCNode*)pObj;
		child->runAction(CCFadeOut::create(duration));
	}
}
