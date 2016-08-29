#ifndef __STARSMOVER_H__
#define __STARSMOVER_H__
#include "cocos2d.h"
#include "LogicGridUtil.h"
class StarNode;

class StarsMover
{
public:
	static StarsMover *fatory();
protected:
	void StarsMover::drop(StarNode *node);
private:// 相对的掉落方向，由具体的移动方向来决定
	bool StarsMover::dropDown();
	bool StarsMover::dropLeftDown();
	bool StarsMover::dropRightDown();
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
private:
	enum MoveDirection
	{
		kMoveUp,
		kMoveDown,
		kMoveLeft,
		kMoveRight,
	};
};

class MoveStarsUp
	:public StarsMover
{
public:
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
	virtual void moveStars();
	virtual void genStars();
private:
	virtual bool isBottom();
	virtual LogicGrid getDownOffset();
	virtual LogicGrid getLeftOffset();
	virtual LogicGrid getRightOffset();
};

#endif