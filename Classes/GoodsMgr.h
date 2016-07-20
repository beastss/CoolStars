#ifndef __GOODSMGR_H__
#define __GOODSMGR_H__
#include "cocos2d.h"
#include "DataConfig.h"

class GoodsMgr
{
public:
	static GoodsMgr *theMgr();
	void addGoods(const GoodsData &data);
	void addGoods(const std::vector<GoodsData> &datas);
private:
};
#endif