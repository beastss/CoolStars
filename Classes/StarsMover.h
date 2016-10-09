#ifndef __STARSMOVER_H__
#define __STARSMOVER_H__
#include "cocos2d.h"
#include "LogicGridUtil.h"
#include "CommonMacros.h"
#include <unordered_map>
class StarNode;

class StarsMover
{
public:
	static StarsMover *fatory();
protected:
	void drop(StarNode *node);
	StarsMover();
private:// 相对的掉落方向，由具体的移动方向来决定
	bool dropDown();
	bool dropLeftDown();
	bool dropRightDown();
private:
	void init();
//接口
public:
	virtual void moveStars() = 0;
	virtual void genStars() = 0;
private:
	virtual bool isBottom() = 0;
	virtual LogicGrid getDownOffset() = 0;
	virtual LogicGrid getLeftOffset() = 0;
	virtual LogicGrid getRightOffset() = 0;
protected:
	StarNode *m_curNode;
	int m_direction;
	std::unordered_map<int, LogicGrid>m_upMoveTopGrids;
	std::unordered_map<int, LogicGrid>m_downMoveTopGrids;
	std::unordered_map<int, LogicGrid>m_leftMoveTopGrids;
	std::unordered_map<int, LogicGrid>m_rightMoveTopGrids;
};

class MoveStarsUp
	:public StarsMover
{
public:
	MoveStarsUp(){ m_direction = kMoveUp;}
	virtual void moveStars();
	virtual void genStars();
private:
	virtual bool isBottom();
	virtual LogicGrid getDownOffset();
	virtual LogicGrid getLeftOffset();
	virtual LogicGrid getRightOffset();
};

class MoveStarsDown
	:public StarsMover
{
public:
	MoveStarsDown(){ m_direction = kMoveDown; }
	virtual void moveStars();
	virtual void genStars();
private:
	virtual bool isBottom();
	virtual LogicGrid getDownOffset();
	virtual LogicGrid getLeftOffset();
	virtual LogicGrid getRightOffset();
};

class MoveStarsLeft
	:public StarsMover
{
public:
	MoveStarsLeft(){ m_direction = kMoveLeft; }
	virtual void moveStars();
	virtual void genStars();
private:
	virtual bool isBottom();
	virtual LogicGrid getDownOffset();
	virtual LogicGrid getLeftOffset();
	virtual LogicGrid getRightOffset();
};

class MoveStarsRight
	:public StarsMover
{
public:
	MoveStarsRight(){ m_direction = kMoveRight; }
	virtual void moveStars();
	virtual void genStars();
private:
	virtual bool isBottom();
	virtual LogicGrid getDownOffset();
	virtual LogicGrid getLeftOffset();
	virtual LogicGrid getRightOffset();
};

#endif