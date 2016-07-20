#ifndef __SHOPMODEL_H__
#define __SHOPMODEL_H__
#include "cocos2d.h"

class ShopModel
{
public:
	static ShopModel *theModel();
	void purchase(int id);
private:
	ShopModel(){}
	void onPurchaseSucceed(int id);
	
};
#endif