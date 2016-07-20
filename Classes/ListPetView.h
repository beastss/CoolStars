#ifndef __LISTPETVIEW_H__
#define __LISTPETVIEW_H__
#include "cocos2d.h"
#include <functional>
#include "ListView.h"

class ListPetView
	: public ListView	
{
public:
	static ListPetView *create(const cocos2d::CCSize &size);
	int addNode(cocos2d::CCNode *node);
	void setItemSelectedCallback(std::function<void(int)> handle){ m_itemSelectedHandle = handle; }
	void setCurItem(int index);
	virtual void clear();
private:
	ListPetView(const cocos2d::CCSize &size);
	virtual bool init();
	void update(float dt);

	virtual void onTouchBegan(cocos2d::CCTouch *pTouch);
	virtual void onTouchMoved(cocos2d::CCTouch *pTouch);
	virtual void onTouchEnded(cocos2d::CCTouch *pTouch);
	void dragBack();
	void nextItem();
	void moveItemToCenter(int index);
private:
	int m_curItem;
	std::function<void(int)> m_itemSelectedHandle;
};
#endif