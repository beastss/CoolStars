#ifndef __GOODSVIEW_H__
#define __GOODSVIEW_H__
#include "cocos2d.h"
#include <unordered_map>
#include "DataConfig.h"
#include <functional>
class UiLayout;

class GoodsView
	: public cocos2d::CCNode
{
public:
	static GoodsView *create(const GoodsData &data);
	float runMoveAction(std::function<void()> handle = std::function<void()>());
private:
	GoodsView(const GoodsData &data);
	virtual bool init();
	void initLayout();
private:
	GoodsData m_data;
	UiLayout *m_layout;

	std::unordered_map<int, std::string> m_goodsRes;
};

#endif