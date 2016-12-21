#ifndef __STAGEBONUSVIEW_H__
#define __STAGEBONUSVIEW_H__
#include "cocos2d.h"
#include <unordered_map>
#include "DataConfig.h"
#include <functional>
class UiLayout;

class StageBonusView
	: public cocos2d::CCNode
{
public:
	static StageBonusView *create(const GoodsData &data);
	void scaleAndFadeOut();
private:
	StageBonusView(const GoodsData &data);
	virtual bool init();
	void initLayout();
	void fadeout(float duration);
private:
	GoodsData m_data;
	UiLayout *m_layout;

	std::unordered_map<int, std::string> m_goodsRes;
};

#endif