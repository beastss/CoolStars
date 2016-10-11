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

	void startGuide(int startAction, std::function<void()> callback = std::function<void()>(), int param = 0);
	void endGuide(int endAction, std::function<void()> callback = std::function<void()>(), int param = 0);

	void setGuideEnable(bool enable);
	void pauseGuide(bool pause){ m_pauseGuide = pause; }//���������ڶ�����������
	void finishGuide();
	bool isRunning(){ return m_isRunning; }
private:
	void onSave();
public:
	std::vector<int> m_finishedGuides;
	int m_curGuideId;
	bool m_guideEnable;
	bool m_pauseGuide;
	bool m_isRunning;//�Ƿ�����ִ������
};
#endif