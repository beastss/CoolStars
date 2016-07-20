#ifndef __PETSCENEMOVEHELPER_H__
#define __PETSCENEMOVEHELPER_H__
#include "cocos2d.h"
class PetSceneMoveHelper
{
public:
	PetSceneMoveHelper();
	void setCenterNode(cocos2d::CCNode *centerNode);
	void removeNode(cocos2d::CCNode *node);
	void moveLeft(cocos2d::CCNode *newNode);
	void moveRight(cocos2d::CCNode *newNode);
	void clearNodes();
	
	void init(const cocos2d::CCPoint &leftmost, const cocos2d::CCPoint &center, const cocos2d::CCPoint &rightmost);
private:
	cocos2d::CCNode *m_curNode;

	cocos2d::CCPoint m_leftmostPos;
	cocos2d::CCPoint m_centerPos;
	cocos2d::CCPoint m_rightmostPos;

	static const float kFirstMoveDuation;
	static const float kSecondMoveDuation;
};
#endif