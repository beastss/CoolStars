#ifndef __SHOPSCENE_H__ 
#define __SHOPSCENE_H__ 
#include "cocos2d.h"
#include "BasePanel.h"
#include "DataConfig.h"

class UiLayout;

class ShopNode
	: public cocos2d::CCNode
{
public:
	static ShopNode *create(const ShopConfig &config);
private:
	ShopNode(const ShopConfig &config) : m_config(config){}
	void onBtnClicked(cocos2d::CCObject* pSender);
	virtual bool init();
private:
	UiLayout *m_layout;
	ShopConfig m_config;
};


class ShopScene :
	public BasePanel
{
public:
	CREATE_FUNC(ShopScene);
	virtual bool init();
private:
	ShopScene(){}
	~ShopScene(){}
	void initPanel();
private:
	UiLayout *m_layout;

};
#endif