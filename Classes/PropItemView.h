#ifndef __PropItemView_H__
#define __PropItemView_H__
#include "cocos2d.h"
#include "TouchNode.h"
#include <functional>
#include "PropManager.h"

class UiLayout;
class PropItemView 
	: public TouchNode
	, public IPropView
{
public:
	static PropItemView *create(int type, int touchPriority);
	virtual ~PropItemView(){}
	void setTouchHandle(std::function<void(int)> handle){ m_touchHandle = handle; }
private:
	PropItemView(int type, int touchPriority);
	virtual bool init();
	virtual bool onTouchBegan(cocos2d::CCPoint pt, bool isInside);
	void runScale();
	void refreshItemNum();

	virtual void onPropItemChanged();
	virtual void onEnter();
	virtual void onExit();
private:
	std::string m_iconPath;
	int m_type;
	UiLayout *m_layout;
	std::function<void(int)> m_touchHandle;
};
#endif