#ifndef __STARSMOVER_H__
#define __STARSMOVER_H__
#include "cocos2d.h"
#include "LogicGridUtil.h"
class StarNode;

class StarsMover
{
public:
	static StarsMover *fatory();
	virtual void moveStars(){}
	virtual void genStars(){}
	virtual bool isBottom(){ return false; }
	virtual LogicGrid getDownOffset(){ return LogicGrid(); }
	virtual LogicGrid getLeftOffset(){ return LogicGrid(); }
	virtual LogicGrid getRightOffset(){return LogicGrid(); }
private:
	bool StarsMover::dropDown();
	bool StarsMover::dropLeftDown();
	bool StarsMover::dropRightDown();
protected:
	void StarsMover::drop(StarNode *node);
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
	virtual bool isBottom();
	virtual LogicGrid getDownOffset();
	virtual LogicGrid getLeftOffset();
	virtual LogicGrid getRightOffset();
};

#endif