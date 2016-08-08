#include "RankingPanelUtil.h"
#include "UiLayout.h"
#include "RankingModel.h"
#include "EmptyBox.h"
#include "DataManager.h"
#include "CommonUtil.h"
#include "SoundMgr.h"
USING_NS_CC;
using namespace std;
using namespace extension;
using namespace CommonUtil;

bool RankingNameInputPanel::init()
{
	m_layout = UiLayout::create("layout/ranking_name_input.xml");
	m_layout->setMenuTouchPriority(m_touchPriority);
	initLayout();
	addChild(m_layout);

	initEditBox();
	setContentSize(m_layout->getContentSize());
	addMaskLayer();
	return true;
}

void RankingNameInputPanel::initLayout()
{
	CCMenuItem *confirmBtn = dynamic_cast<CCMenuItem *>((m_layout->getChildById(4)));
	confirmBtn->setTarget(this, menu_selector(RankingNameInputPanel::onConfirm));

	CCMenuItem *genNameBtn = dynamic_cast<CCMenuItem *>((m_layout->getChildById(3)));
	genNameBtn->setTarget(this, menu_selector(RankingNameInputPanel::onGenRandomName));
}

void RankingNameInputPanel::initEditBox()
{
	CCSize editSize = CCSize(200, 50);
	m_editBox = CCEditBox::create(editSize, CCScale9Sprite::create("ranking/text_bk.png"));
	m_editBox->setDelegate(this);
	m_editBox->setAnchorPoint(ccp(0, 0));
	m_editBox->setPosition(m_layout->getChildById(6)->getPosition());
	m_editBox->setTouchPriority(m_touchPriority - 1);
	string name = RankingModel::theModel()->getRandomName();
	refreshName(name);
	m_editBox->setMaxLength(4);
	addChild(m_editBox);
}

void RankingNameInputPanel::refreshName(std::string name)
{
	m_editBox->setText(name.c_str());
}

void RankingNameInputPanel::onGenRandomName(cocos2d::CCObject* pSender)
{
	SoundMgr::theMgr()->playEffect(kEffectMusicButton);
	string name = RankingModel::theModel()->getRandomName();
	refreshName(name);
}


void RankingNameInputPanel::onConfirm(cocos2d::CCObject* pSender)
{
	SoundMgr::theMgr()->playEffect(kEffectMusicButton);
	string name = m_editBox->getText();
	if (RankingModel::theModel()->isValidName(name))
	{
		RankingModel::theModel()->initFirstOpenRanking(name);
		removeFromParent();
	}
	else
	{
		CCMessageBox("invalid name", "title");
	}

}

void RankingNameInputPanel::editBoxEditingDidBegin(CCEditBox *editBox)
{
	CCLOG("start edit");
}

void RankingNameInputPanel::editBoxEditingDidEnd(CCEditBox *editBox)
{
	CCLOG("end edit");
}

void RankingNameInputPanel::editBoxTextChanged(CCEditBox *editBox, const std::string &text)
{
	CCLOG("textchanged");
}

void RankingNameInputPanel::editBoxReturn(CCEditBox *editBox)
{
	CCLOG("editboxreturn");
}

//////////////////////////////////////////////////////////////////////////////
RankingNode* RankingNode::create(int rank, const RankingData &data)
{
	RankingNode *node = new RankingNode(rank, data);
	node->init();
	node->autorelease();
	return node;
}

bool RankingNode::init()
{
	if (m_data.type == kPlayer)
	{
		m_layout = UiLayout::create("layout/ranking_node_player.xml");
	}
	else
	{
		m_layout = UiLayout::create("layout/ranking_node_common.xml");
	}

	if (m_data.type == kData)
	{
		m_layout->getChildById(10)->setVisible(false);
	}
	addChild(m_layout);
	setContentSize(m_layout->getContentSize());

	CCLabelTTF *name = dynamic_cast<CCLabelTTF *>(m_layout->getChildById(5));
	name->setString(m_data.name.c_str());

	CCLabelAtlas *rank = dynamic_cast<CCLabelAtlas *>(m_layout->getChildById(7));
	rank->setString(intToStr(m_rank));

	CCLabelAtlas *stage = dynamic_cast<CCLabelAtlas *>(m_layout->getChildById(6));
	stage->setString(intToStr(m_data.stage));

	CCLabelAtlas *petPercent = dynamic_cast<CCLabelAtlas *>(m_layout->getChildById(8));
	petPercent->setString(intToStr(m_data.ownPetPercent));

	auto petPercentProgress = CCProgressTimer::create(CCSprite::create("ranking/phb_jinditiao.png"));
	petPercentProgress->setType(kCCProgressTimerTypeBar);
	petPercentProgress->setMidpoint(ccp(0, 1));
	petPercentProgress->setBarChangeRate(ccp(1, 0));
	petPercentProgress->setPercentage(0);
	petPercentProgress->runAction(CCProgressTo::create(0.5f, m_data.ownPetPercent));
	auto box = dynamic_cast<EmptyBox *>(m_layout->getChildById(9));
	box->setNode(petPercentProgress);

	return true;
}

//////////////////////////////////////////////////////////////////////////////

bool RankingOpponentUpgradePanel::init()
{
	m_layout = UiLayout::create("layout/ranking_opponet_upgrade.xml");
	initLayout();
	addChild(m_layout);

	setContentSize(m_layout->getContentSize());
	addMaskLayer();
	return true;
}

void RankingOpponentUpgradePanel::initLayout()
{
	CCMenuItem *confirmBtn = dynamic_cast<CCMenuItem *>((m_layout->getChildById(4)));
	confirmBtn->setTarget(this, menu_selector(RankingOpponentUpgradePanel::onConfirm));

	int rank1 = RankingModel::theModel()->getOpponetRank();
	auto data1 = RankingOpponent::theOpponent()->getRankingData();
	//更新对手数据
	RankingOpponent::theOpponent()->update();
	int rank2 = RankingModel::theModel()->getOpponetRank();
	auto data2 = RankingModel::theModel()->getDataByRank(rank2 + 1);

	if (rank1 > rank2)
	{
		//对手节点
		auto opponent = dynamic_cast<EmptyBox *>((m_layout->getChildById(6)));
		opponent->setNode(RankingNode::create(rank1, data1));

		//被超越的节点
		auto oldRank = dynamic_cast<EmptyBox *>((m_layout->getChildById(5)));
		oldRank->setNode(RankingNode::create(rank2, data2));

		runAction(CCSequence::create(CCDelayTime::create(1.0f),
			CCCallFunc::create(this, callfunc_selector(RankingOpponentUpgradePanel::doMoveAction)), NULL));
	}
	else
	{
		auto data3 = RankingOpponent::theOpponent()->getRankingData();
		auto opponent = dynamic_cast<EmptyBox *>((m_layout->getChildById(7)));
		opponent->setNode(RankingNode::create(rank2, data3));
	}
}

void RankingOpponentUpgradePanel::doMoveAction()
{
	auto targetRank = dynamic_cast<EmptyBox *>((m_layout->getChildById(5)));
	auto opponent = dynamic_cast<EmptyBox *>((m_layout->getChildById(6)));

	const float kDuration = 0.2f;
	CCPoint toPos1;
	CCPoint toPos2;
	toPos1 = ccp(targetRank->getPositionX(), opponent->getPositionY());
	toPos2 = ccp(opponent->getPositionX(), targetRank->getPositionY());

	targetRank->runAction(CCEaseBounceInOut::create(CCMoveTo::create(kDuration, toPos1)));
	opponent->runAction(CCEaseBounceInOut::create(CCMoveTo::create(kDuration, toPos2)));

	runAction(CCSequence::create(CCDelayTime::create(kDuration + 1.0f),
		CCCallFunc::create(this, callfunc_selector(RankingOpponentUpgradePanel::onMoveActionFinished)), NULL));
}

void RankingOpponentUpgradePanel::onConfirm(cocos2d::CCObject* pSender)
{
	SoundMgr::theMgr()->playEffect(kEffectMusicButton);
	removeFromParent();
}

void RankingOpponentUpgradePanel::onMoveActionFinished()
{
	int rank = RankingModel::theModel()->getOpponetRank();
	auto data = RankingOpponent::theOpponent()->getRankingData();
	auto targetRank = dynamic_cast<EmptyBox *>((m_layout->getChildById(6)));
	targetRank->setNode(RankingNode::create(rank, data));

	data = RankingModel::theModel()->getDataByRank(rank + 1);
	auto opponent = dynamic_cast<EmptyBox *>((m_layout->getChildById(5)));
	opponent->setNode(RankingNode::create(rank + 1, data));
}