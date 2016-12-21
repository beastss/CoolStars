#ifndef __TREASURESTAGENOTIFIER_H__
#define __TREASURESTAGENOTIFIER_H__
#include "cocos2d.h"
#include "CommonMacros.h"

struct ITreasureStageView
{
	virtual void onStarExplore(int color, int score, cocos2d::CCPoint pt){}
	virtual void onPassCurStage(int stage){}
	virtual void onStageOverStart(int stage, bool won){}
	virtual void onStageOverEnd(int stage, bool won){}
	virtual void onEraseStars(int num, int score){}
	virtual void onNewStage(){}
};

class TreasureStageNotifier
{
public:
	static TreasureStageNotifier *theNotifier();
	void addView(ITreasureStageView *view);
	void removeView(ITreasureStageView *view);
public:
	void onStarExplore(int color, int score, cocos2d::CCPoint pt){ NOTIFY_VIEWS(onStarExplore, color, score, pt); }
	void onPassCurStage(int stage){ NOTIFY_VIEWS(onPassCurStage, stage); }
	void onStageOverStart(int stage, bool won){ NOTIFY_VIEWS(onStageOverStart, stage, won); }
	void onStageOverEnd(int stage, bool won){ NOTIFY_VIEWS(onStageOverEnd, stage, won); }
	void onEraseStars(int num, int score){ NOTIFY_VIEWS(onEraseStars, num, score); }
	void onNewStage(){ NOTIFY_VIEWS(onNewStage); }
private:
	std::vector<ITreasureStageView *>m_views;
};
#endif
