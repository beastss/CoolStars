#ifndef __STAGEPANELUTIL_H__
#define __STAGEPANELUTIL_H__

#include "cocos2d.h"
#include "TouchNode.h"
#include <functional>
#include "ScaleDialog.h"

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
/////////////////////////////////////////////
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

/////////////////////////////////////////////
class FailHitDialog : public ScaleDialog
{
public:
	CREATE_FUNC(FailHitDialog);
	void setHandle(std::function<void()>handle){ m_handle = handle; }
private:
	virtual bool init();
	void onCloseBtnClicked(cocos2d::CCObject* pSender);
private:
	std::function<void()> m_handle;
};
//////////////////////////////////////////////
class FailToUpgradePetDialog : public ScaleDialog
{
public:
	static FailToUpgradePetDialog *create(int usage = kUsageStageFail);
private:
	FailToUpgradePetDialog(int usage):m_usage(usage){}
	virtual bool init();
	void onCancel(cocos2d::CCObject* pSender);
	void onExit(cocos2d::CCObject* pSender);
	void onToPetScene(cocos2d::CCObject* pSender);
private:
	int m_usage;
public:
	enum
	{
		kUsageStageFail,
		kUsageExitGame,
	};
};
#endif