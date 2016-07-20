#ifndef __GUIDEVIEW_H__
#define __GUIDEVIEW_H__
#include "cocos2d.h"
class UiLayout;
enum RectType
{
	kRectTypeFixedPos,
	kRectTypeBottom,
	kRectTypeTop,
};

enum FingerType
{
	kFingerTypeHide,
	kFingerTypeNormal,
	kFingerTypeSlide,
};
class GuideView
	: public cocos2d::CCNode
	, public cocos2d::CCTouchDelegate

{
public:
	static GuideView *create(int guideId);

private:
	GuideView(int guideId);
	virtual bool init();

	void initPanel();
	void initTargetRect();
	void initFinger();
	void initTextDialog();
	void initMask();

	virtual void onEnter();
	virtual void onExit();
	virtual bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual void ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	bool isTargetArea(cocos2d::CCPoint worldPos);
	void adjustTextDialogPos();
private:
	UiLayout *m_layout;
	UiLayout *m_textDialog;
	int m_guideId;
	cocos2d::CCRect m_targetRect;
	static float kRefWidth;
	static float kRefHeight;
};
#endif