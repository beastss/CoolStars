#ifndef __GUIDEMGR_H__
#define __GUIDEMGR_H__
#include "cocos2d.h"
#include <functional>
#include "GuideMacros.h"
class GuideMgr
{
public:
	static GuideMgr *theMgr();
	GuideMgr();
	void init();

	void startGuide(int startAction, std::function<void()> callback = std::function<void()>());
	void endGuide(int endAction, int param = 0);

	void setGuideEnable(bool enable);
private:
	void finishGuide();
	void onSave();
public:
	std::vector<int> m_finishedGuides;
	int m_curGuideId;
	bool m_guideEnable;
};
#endif