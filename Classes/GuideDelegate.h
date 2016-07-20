#ifndef __GUIDEDELEGATE_H__
#define __GUIDEDELEGATE_H__
#include "cocos2d.h"
#include <functional>
#include <unordered_map>

/*
class GuideDelegate
{
public:
	void setPanelId(int panelId);
	void registerCallback(int guideType, std::function<void()> callback);
	void guideCheck(int guideType);
private:
	int m_panelId;
	std::unordered_map<int, std::function<void()>> m_callbacks;
};
*/

struct GuideDelegate
{
	virtual void initGuide() = 0;
};
#endif
