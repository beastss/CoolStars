#ifndef __PROPMANAGER_H__
#define __PROPMANAGER_H__
#include "cocos2d.h"
#include "StarNode.h"

enum PropType
{
	kPropReorder,
	kPropBrush,
	kPropBomb,

	kPorpTypeAmount,
};

struct IPropView
{
	virtual void onPropItemChanged(){}
};
class PropManager
{
public:
	static PropManager *propMgr();
	void loadPropData();
	
	int getPropItemAmount(int type);
	void setPropItemAmount(int type, int amount);
	
	void usePropBomb(int StarType, const LogicGrid &grid);
	void usePropBrush(const LogicGrid &grid, int color);
	void usePropReorder();

	void addView(IPropView *view);
	void removeView(IPropView *view);
	void setInfinite(bool isInfinite){ m_infinite = isInfinite; }
	bool isInfiniteMode(){ return m_infinite;}

	void highLightCanBombArea();
	void highLightCanBrushArea();
private: 
	PropManager() : m_infinite(false){}
	void usePropItem(int propType);
	void save();

private:
	std::vector<int> m_propAmount;
	std::vector<IPropView *> m_views;
	bool m_infinite;
};
#endif