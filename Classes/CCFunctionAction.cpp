#include "CCFunctionAction.h"
USING_NS_CC;
using namespace std;
CCFunctionAction * CCFunctionAction::create(function<void()> func)
{
	CCFunctionAction *pRet = new CCFunctionAction(func);

	if (pRet)
	{
		pRet->autorelease();
	}

	return pRet;
}

void CCFunctionAction::update(float time)
{
	CC_UNUSED_PARAM(time);
	m_func();
}

CCObject* CCFunctionAction::copyWithZone(CCZone *pZone)
{
	CCZone *pNewZone = NULL;
	CCFunctionAction *pRet = NULL;

	if (pZone && pZone->m_pCopyObject) {
		pRet = (CCFunctionAction*)(pZone->m_pCopyObject);
	}
	else {
		pRet = new CCFunctionAction(m_func);
		pZone = pNewZone = new CCZone(pRet);
	}

	CCActionInstant::copyWithZone(pZone);
	CC_SAFE_DELETE(pNewZone);
	return pRet;
}
