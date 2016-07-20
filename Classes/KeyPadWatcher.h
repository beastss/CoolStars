#ifndef __KEYPADWATCHER_H__
#define __KEYPADWATCHER_H__ 
#include "cocos2d.h"

class KeyPadWatcher
	: public cocos2d::CCLayer
{
public:
	CREATE_FUNC(KeyPadWatcher);
private:
	virtual bool init();
	virtual void  keyBackClicked();
	virtual void  keyMenuClicked();
	void exitGame();
private:
};
#endif