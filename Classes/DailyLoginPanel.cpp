#include "DailyLoginPanel.h"
#include "UiLayout.h"
#include "CommonUtil.h"
#include "DailyLoginModel.h"
#include "TitlePanel.h"
#include "EmptyBox.h"
#include "DataManager.h"
#include "MainScene.h"
#include "SoundMgr.h"
using namespace std;
USING_NS_CC;

bool DailyLoginPetHint::init()
{
	auto layout = UiLayout::create("layout/daily_login_pet_hint.xml");
	setContentSize(layout->getContentSize());
	addChild(layout);

	//CCMenuItem *okBtn = dynamic_cast<CCMenuItem *>(layout->getChildById(3));
	//okBtn->setTarget(this, menu_selector(DailyLoginPetHint::onOkBtnClicked));

	//addMaskLayer();
	
	runAction(CCSequence::create(CCDelayTime::create(2.0f), CCRemoveSelf::create(), NULL));
	return true;
}

void DailyLoginPetHint::onOkBtnClicked(cocos2d::CCObject* pSender)
{
	SoundMgr::theMgr()->playEffect(kEffectMusicButton);
	removeFromParent();
}
/////////////////////////////////////////////////////////////
DailyLoginNode::DailyLoginNode(int day)
: m_day(day)
{

}

DailyLoginNode *DailyLoginNode::create(int day)
{
	auto node = new DailyLoginNode(day);
	node->init();
	node->autorelease();
	return node;
}

bool DailyLoginNode::init()
{
	m_layout = UiLayout::create("layout/daily_login_node.xml");
	addChild(m_layout);
	auto size = m_layout->getContentSize();
	setContentSize(size);

	int state = DailyLoginModel::theModel()->getRewardState(m_day);
	switch (state)
	{
	case DailyLoginModel::kAlreadyGotReward:
		m_layout->getChildById(6)->setVisible(false);
		break;
	case DailyLoginModel::kCanGotReward:
		m_layout->getChildById(7)->setVisible(false);
		m_layout->getChildById(8)->setVisible(false);
		break;
	case DailyLoginModel::kCannotGotReward:
		m_layout->getChildById(6)->setVisible(false);
		m_layout->getChildById(7)->setVisible(false);
		m_layout->getChildById(8)->setVisible(false);
		break;
	default:
		break;
	}
	highLightRewards();

	auto config = DataManagerSelf->getDailyLoginConfig(m_day);
	auto rewardImg = dynamic_cast<CCSprite *>(m_layout->getChildById(2));
	rewardImg->initWithFile(config.rewardPath.c_str());

	auto daysImg = dynamic_cast<CCSprite *>(m_layout->getChildById(5));
	daysImg->initWithFile(config.dayLabelPath.c_str());
	
	auto numLabel = dynamic_cast<CCLabelAtlas *>(m_layout->getChildById(9));
	auto goodsData = config.goods;
	numLabel->setString(CommonUtil::intToStr(goodsData.amount));
	return true;
}

void DailyLoginNode::highLightRewards()
{
	int state = DailyLoginModel::theModel()->getRewardState(m_day);
	if (state == DailyLoginModel::kCanGotReward)
	{
		auto rotateTo1 = CCRotateTo::create(0.1f, 20);
		auto rotateTo2 = CCRotateTo::create(0.2f, -20);
		auto rotateTo3 = CCRotateTo::create(0.1f, 0);
		auto rotateSeq = CCSequence::create(rotateTo1, rotateTo2, rotateTo3, NULL);
		m_layout->getChildById(2)->runAction(CCRepeatForever::create(rotateSeq));
	}
}

/////////////////////////////////////////////////////////////
void DailyLoginPanel::onEnter()
{
	ScaleDialog::onEnter();
	MsgNotifier::theModel()->addView(this);
}

void DailyLoginPanel::onExit()
{
	ScaleDialog::onExit();
	MsgNotifier::theModel()->removeView(this);
}

bool DailyLoginPanel::init()
{
	m_layout = UiLayout::create("layout/daily_login_dialog.xml");
	auto size = m_layout->getContentSize();
	addChild(m_layout);
	setContentSize(size);
	addMaskLayer();
	
	CCMenuItem *closeBtn = dynamic_cast<CCMenuItem *>(m_layout->getChildById(5));
	closeBtn->setTarget(this, menu_selector(DailyLoginPanel::onCloseBtnClicked));

	CCMenuItem *getRewardBtn = dynamic_cast<CCMenuItem *>(m_layout->getChildById(10));
	getRewardBtn->setTarget(this, menu_selector(DailyLoginPanel::onGetRewardBtnClicked));
	
	refreshUi();
	return true;
}

void DailyLoginPanel::refreshUi()
{
	bool hasReward = DailyLoginModel::theModel()->hasRewad();
	m_layout->getChildById(10)->setVisible(hasReward);
	m_layout->getChildById(11)->setVisible(!hasReward);

	for (int i = 0; i < 7; ++i)
	{
		auto box = dynamic_cast<EmptyBox *>(m_layout->getChildById(12 + i));
		auto node = DailyLoginNode::create(i + 1);
		box->setNode(node);
		m_nodes.push_back(node);
	}
}

void DailyLoginPanel::onCloseBtnClicked(cocos2d::CCObject* pSender)
{
	SoundMgr::theMgr()->playEffect(kEffectMusicButton);
	removeFromParent();
}

void DailyLoginPanel::onGetRewardBtnClicked(cocos2d::CCObject* pSender)
{
	SoundMgr::theMgr()->playEffect(kEffectMusicButton);
	DailyLoginModel::theModel()->getReward();
	refreshUi();
	for (size_t i = 0; i < m_nodes.size(); ++i)
	{
		//m_nodes[i]->onGetReward();
	}
	//removeFromParent();
}

void DailyLoginPanel::onDailyLoginPetReward(int petId, bool success)
{
	if (!success)
	{
		MainScene::theScene()->showDialog(DailyLoginPetHint::create(), false);
	}
}
