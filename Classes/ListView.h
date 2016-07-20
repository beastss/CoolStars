#ifndef __LISTVIEW_H__
#define __LISTVIEW_H__
#include "cocos2d.h"
#include "TouchNode.h"

class ListView
	: public cocos2d::CCNode
	, public cocos2d::CCTouchDelegate
{
public:
	static ListView *create(const cocos2d::CCSize &size);
	void setTouchPriority(int touchPriority){ m_touchPriority = touchPriority; }
	void removeNode(int index);
	cocos2d::CCNode *getNode(int index);
	int count(){ return m_nodes.size(); }
	void setSpacing(float spacing){ m_spacing = spacing; }
	virtual void clear();
	virtual int addNode(cocos2d::CCNode *node);
protected:
	ListView(const cocos2d::CCSize &size);
	void doMove(cocos2d::CCPoint offset);
	bool isOutOfRange();
	virtual bool init();
	virtual void onTouchBegan(cocos2d::CCTouch *pTouch){}
	virtual void onTouchMoved(cocos2d::CCTouch *pTouch){}
	virtual void onTouchEnded(cocos2d::CCTouch *pTouch){}
private:
	void dragback();
	virtual void onEnter();
	virtual void onExit();
	virtual bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual void ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual void ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	
protected:
	cocos2d::CCNode *m_container;
	cocos2d::CCClippingNode *m_clippingNode;
	std::vector<cocos2d::CCNode *> m_nodes;
	cocos2d::CCSize m_size;
	int m_touchPriority;
	float m_spacing;
	bool m_isTouching;
};
#endif