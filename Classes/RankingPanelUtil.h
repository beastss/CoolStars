#ifndef __RANKINGPANELUTIL_H__
#define __RANKINGPANELUTIL_H__
#include "cocos2d.h"
#include "cocos-ext.h"
#include "ScaleDialog.h"
#include "GUI/CCEditBox/CCEditBox.h"
#include "RankingModel.h"

class UiLayout;

class RankingNameInputPanel
	: public ScaleDialog
	, public cocos2d::extension::CCEditBoxDelegate
{
public:
	CREATE_FUNC(RankingNameInputPanel);
	
private:
	virtual bool init();
	void initLayout();
	void initEditBox();
	void refreshName(std::string name);
	void onGenRandomName(cocos2d::CCObject* pSender);
	void onConfirm(cocos2d::CCObject* pSender);
private:
	virtual void editBoxEditingDidBegin(cocos2d::extension::CCEditBox *editBox);
	virtual void editBoxEditingDidEnd(cocos2d::extension::CCEditBox *editBox);
	virtual void editBoxTextChanged(cocos2d::extension::CCEditBox *editBox, const std::string &text);
	virtual void editBoxReturn(cocos2d::extension::CCEditBox *editBox);
private:
	UiLayout *m_layout;
	cocos2d::extension::CCEditBox *m_editBox;
};

class RankingNode
	: public cocos2d::CCNode
{
public:
	static RankingNode *create(int rank, const RankingData &data);
private:
	RankingNode(int rank, const RankingData &data) : m_rank(rank), m_data(data){}
	virtual bool init();
private:
	UiLayout *m_layout;
	int m_rank;
	RankingData m_data;
};

class RankingOpponentUpgradePanel
	: public ScaleDialog
{
public:
	CREATE_FUNC(RankingOpponentUpgradePanel);

private:
	virtual bool init();
	void initLayout();
	void onConfirm(cocos2d::CCObject* pSender);
	void doMoveAction();
	void onMoveActionFinished();
private:
	UiLayout *m_layout;
};
#endif