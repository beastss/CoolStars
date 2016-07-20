#ifndef __SETTINGBTNSHELPER_H__
#define __SETTINGBTNSHELPER_H__
#include "cocos2d.h"
#include <unordered_map>
class UiLayout;

class SettingBtnsHelper
{
public:
	SettingBtnsHelper() :m_isVisible(false){}
	void toggle();
	void addPopupNodes(cocos2d::CCNode *node);
	void removePopupNodes(cocos2d::CCNode *node);
	void init(cocos2d::CCPoint targetPos);
private:
private:
	std::unordered_map<cocos2d::CCNode *, cocos2d::CCPoint> m_nodes;
	bool m_isVisible;
	cocos2d::CCPoint m_targetPos;
};
#endif