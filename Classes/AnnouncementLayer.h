#ifndef __ANNOUNCEMENTLAYER_H__
#define __ANNOUNCEMENTLAYER_H__
#include "cocos2d.h"
#include <deque>
#include <functional>
class AnnouncementLayer
	: public cocos2d::CCNode
	, public cocos2d::CCTouchDelegate
{
public:
	CREATE_FUNC(AnnouncementLayer);
	void showTips(const char *str, bool swallowTouch, std::function<void()> callback);
	void addAnnouncement(const char *picPath);
	void setLoop(bool loop){ m_loop = loop; }
	void removeAnnouncement();
private:
	AnnouncementLayer() :m_loop(false), m_swallowTouch(false){}
	virtual void onEnter();
	virtual void onExit();
	virtual bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	void showAnnouncement();
	virtual bool init();
	void initPanel();
private:
	std::deque<std::string> m_announcement;
	bool m_loop;
	bool m_swallowTouch;
};
#endif