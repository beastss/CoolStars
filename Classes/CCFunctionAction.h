#include "cocos2d.h"
#include <functional>

class CCFunctionAction : public cocos2d::CCActionInstant
{
public:
	CCFunctionAction(std::function<void()> func) :m_func(func){}
	virtual ~CCFunctionAction(){}
	virtual void update(float time);
	virtual CCObject* copyWithZone(cocos2d::CCZone *pZone);
public:
	static CCFunctionAction * create(std::function<void()> func);
private:
	std::function<void()> m_func;
};

