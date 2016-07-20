#ifndef __BACKGROUNDLAYER_H__
#define __BACKGROUNDLAYER_H__

#include "cocos2d.h"
class UiLayout;
class BackgroundLayer
	: public cocos2d::CCNode
{
public:
	CREATE_FUNC(BackgroundLayer);
	virtual bool init();
private:
	void doMoveAction(cocos2d::CCNode *node, float speed, cocos2d::CCPoint sourcePos, cocos2d::CCPoint targetPos);
	void initLayout();
private:
	UiLayout *m_bkLayout;

};
#endif