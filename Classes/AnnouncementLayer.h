#ifndef __ANNOUNCEMENTLAYER_H__
#define __ANNOUNCEMENTLAYER_H__
#include "cocos2d.h"
#include <deque>
#include <functional>
class AnnouncementLayer
	: public cocos2d::CCNode
{
public:
	CREATE_FUNC(AnnouncementLayer);
	void showTips(const char *str);
	void addAnnouncement(const char *picPath);
	void setLoop(bool loop){ m_loop = loop; }
	void removeAnnouncement();
private:
	AnnouncementLayer() :m_loop(false){}
	void showAnnouncement();
	virtual bool init();
	void initPanel();
private:
	std::deque<std::string> m_announcement;
	bool m_loop;
};
#endif