#ifndef __STAGEMASKLAYER_H__
#define __STAGEMASKLAYER_H__

#include "cocos2d.h"
#include "StarsController.h"
#include "StageLayersMgr.h"
#include <unordered_map>
#include "LogicGridUtil.h"
class StageMaskLayer;

class MaskOperator
{
public:
	MaskOperator(StageMaskLayer *layer) : m_layer(layer){}
	virtual bool onTouchBegan(cocos2d::CCTouch *pTouch){ return true; }
	virtual void OnTouchMoved(cocos2d::CCTouch *pTouch){}
	virtual void onTouchEnd(cocos2d::CCTouch *pTouch){}
protected:
	StageMaskLayer *m_layer;
};

//高亮指定颜色的普通星星
class StarColorMaskOperator : public MaskOperator
{
public:
	StarColorMaskOperator(StageMaskLayer *layer) :MaskOperator(layer){}
	void onHighLightStars(int color, int radiusX, int radiusY);
	virtual bool onTouchBegan(cocos2d::CCTouch *pTouch);
	virtual void OnTouchMoved(cocos2d::CCTouch *pTouch);
	virtual void onTouchEnd(cocos2d::CCTouch *pTouch);
private:
	std::unordered_map<cocos2d::CCNode *, LogicGrid> m_stars;
	LogicGrid m_curGrid;
	int m_radiusX;
	int m_radiusY;
};

//高亮指定的宠物
class PetMaskOperator : public MaskOperator
{
public:
	PetMaskOperator(StageMaskLayer *layer) :MaskOperator(layer){}
	virtual void onTouchEnd(cocos2d::CCTouch *pTouch);
	void onHighLightPets(const std::vector<int> &petIds);

private:
	std::vector<cocos2d::CCNode *> m_stars;
};

//高亮指定区域矩形
class StarRectMaskOperator : public MaskOperator
{
public:
	StarRectMaskOperator(StageMaskLayer *layer) :MaskOperator(layer){}
	virtual bool onTouchBegan(cocos2d::CCTouch *pTouch);
	virtual void OnTouchMoved(cocos2d::CCTouch *pTouch);
	virtual void onTouchEnd(cocos2d::CCTouch *pTouch);
	void onHighLightRectStars(int x, int y, int width, int height, int radiusX, int radiusY);
private:
	std::unordered_map<cocos2d::CCNode *, LogicGrid> m_stars;
	LogicGrid m_curGrid;
	int m_radiusX;
	int m_radiusY; 
};

class StageMaskLayer
	: public cocos2d::CCNode
	, public cocos2d::CCTouchDelegate
	, public IStageLayer
{
public:
	CREATE_FUNC(StageMaskLayer);
	void initPetViewsInfo(std::unordered_map<int, cocos2d::CCPoint> info);
	std::unordered_map<int, cocos2d::CCPoint> getPetViewInfo(){ return m_petsInfo; }
	void addNode(cocos2d::CCNode *node);
	void setNodesVisible(bool visible){ m_container->setVisible(visible); }
	void endMask(bool toNormalState = true);
	//二次选择的高亮
	void highLightRect(int x, int y, int radiusX, int radiusY);
	void removeRectHightLight();
private:
	StageMaskLayer();
	virtual bool init();
	virtual void onEnter();
	virtual void onExit();
	virtual bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual void ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual void ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);

	virtual void onHighLightStars(int color, int radiusX, int radiusY);
	virtual void onHighLightPets(const std::vector<int> &petIds);
	virtual void onHighLightRectStars(int x, int y, int width, int height, int radiusX, int radiusY);
protected:
	std::unordered_map<int, cocos2d::CCPoint> m_petsInfo;
	std::unordered_map<int, cocos2d::CCNode *> m_pets;
	MaskOperator *m_curOp;
	cocos2d::CCNode *m_container;
	cocos2d::CCNode *m_rectNodes;

	StarColorMaskOperator *m_colorStarOp;
	PetMaskOperator *m_petOp;
	StarRectMaskOperator *m_starRectOp;
};

#endif
