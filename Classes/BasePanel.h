#ifndef __BASEPANEL_H__
#define __BASEPANEL_H__
#include "cocos2d.h"

class BasePanel
	: public cocos2d::CCNode
{
public:
	virtual bool init();
	virtual ~BasePanel(){}
	int getPanelId(){ return m_panelId; }
	int getUsage(){ return m_usage; }
protected:
	BasePanel();
	virtual void onEnter();
	void setPanelId(int panelId){ m_panelId = panelId; }
	void setUsage(int usage){ m_usage = usage; }
protected:
	int m_touchPriority;
	int m_panelId;
	int m_usage;
private:
	
};
#endif