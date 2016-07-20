#ifndef __STAGEPANELUTIL_H__
#define __STAGEPANELUTIL_H__

#include "cocos2d.h"
#include "TouchNode.h"
#include <functional>

class UiLayout;
class StagePetSkillPanel: public TouchNode
{
public:
	static StagePetSkillPanel *create(int touchPriority);
	StagePetSkillPanel(int touchPriority) : TouchNode(touchPriority){}
	virtual bool init();
	
	virtual bool onTouchBegan(cocos2d::CCPoint pt, bool isInside);
private:
	UiLayout *m_layout;
};

class ChangeStarColorPanel:public TouchNode
{
public:
	static ChangeStarColorPanel *create(int myColor, int touchPriority);
	void setCallback(std::function<void(bool ,int)> callback){ m_callback = callback; }
	virtual bool init();

	virtual bool onTouchBegan(cocos2d::CCPoint pt, bool isInside);
private:
	ChangeStarColorPanel(int myColor, int touchPriority);
private:
	UiLayout *m_layout;
	std::vector<int> m_colors;
	std::function<void(bool, int)> m_callback;

};

#endif