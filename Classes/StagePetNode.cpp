#include "StagePetNode.h"
#include "PetEntity.h"
#include "PetManager.h"
#include "UiLayout.h"
#include "EmptyBox.h"
#include "StagePetSkillIconProgress.h"
#include "CommonUtil.h"
using namespace std;
USING_NS_CC;

StagePetNode::StagePetNode(int petId, int touchPriority)
: m_touchPriority(touchPriority)
, m_petId(petId)
{
	m_model = PetManager::petMgr()->getPetById(petId);
	assert(m_model);
}

StagePetNode *StagePetNode::create(int petId, int touchPriority)
{
	auto view = new StagePetNode(petId, touchPriority);
	view->init();
	view->autorelease();
	return view;
}

void StagePetNode::onEnter()
{
	CCNode::onEnter();
	StarsController::theModel()->addView(this);
	PetManager::petMgr()->addView(this);
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, m_touchPriority, true);
}

void StagePetNode::onExit()
{
	CCNode::onExit();
	StarsController::theModel()->removeView(this);
	PetManager::petMgr()->removeView(this);
	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
}

bool StagePetNode::init()
{
	m_layout = UiLayout::create("layout/stage_pet_node.xml");
	auto size = m_layout->getContentSize();
	addChild(m_layout);
	initLayout();
	setContentSize(size);
	setAnchorPoint(ccp(0.5f, 0.5f));
	
	CCLayerColor *mask = CCLayerColor::create(ccc4(255, 0, 0, 75));
	mask->setContentSize(size);
	//addChild(mask);

	return true;
}

void StagePetNode::initLayout()
{
	EmptyBox *stateBox = dynamic_cast<EmptyBox *>(m_layout->getChildById(3));
	m_stateLayout = UiLayout::create("layout/stage_pet_node_state.xml");
	stateBox->setNode(m_stateLayout);
	m_stateLayout->setVisible(false);

	m_petAnimation = dynamic_cast<CCArmature *>(m_layout->getChildById(4));

	string path = m_model->getPetData().petAnimationRes;
	CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo(path.c_str());

	int pos1 = path.rfind("/");
	int pos2 = path.rfind(".");
	string armatureName = path.substr(pos1 + 1, pos2 - pos1 - 1);

	m_petAnimation->init(armatureName.c_str());
	m_petAnimation->getAnimation()->play("standby");

	EmptyBox *box = dynamic_cast<EmptyBox *>(m_layout->getChildById(5));
	box->removeNode();
	m_skillIcon = StagePetSkillIconProgress::create(m_petId);
	box->setNode(m_skillIcon);

	auto data = PetManager::petMgr()->getPetById(m_petId)->getPetData();
	auto config = DataManagerSelf->getPetColorConfig(data.color);

	//等级图标
	CCSprite *lvImg = dynamic_cast<CCSprite *>(m_layout->getChildById(6));
	lvImg->initWithFile(config.skillLvLabel.c_str());
	//等级
	CCLabelAtlas *lvNum = dynamic_cast<CCLabelAtlas *>(m_layout->getChildById(7));
	CCSprite *numRes = CCSprite::create(config.numRes.c_str());
	auto numResSize = numRes->getContentSize();
	lvNum->initWithString(CommonUtil::intToStr(data.level), config.numRes.c_str(), numResSize.width / 10, numResSize.height, '0');
}

bool StagePetNode::isInside(CCPoint pt)
{
	auto pos = convertToNodeSpace(pt);
	auto size = getContentSize();
	CCRect rect(0, 0, size.width, size.height);
	return rect.containsPoint(pos);
}

bool StagePetNode::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
	if (isInside(pTouch->getLocation()))
	{
		if (m_touchHandle)
		{
			m_touchHandle(m_petId);
		}
		if (m_model->isEnergyFull())
		{
			playHappyAction(false);
		}
		return true;
	}

	return false;
}

void StagePetNode::playHappyAction(bool loop)
{
	if (loop)
	{
		m_petAnimation->getAnimation()->play("move", -1, -1, 1);
	}
	else
	{
		m_petAnimation->getAnimation()->play("move");
		m_petAnimation->getAnimation()->setMovementEventCallFunc(this, SEL_MovementEventCallFunc(&StagePetNode::runNormalAction));
	}
}

void StagePetNode::runNormalAction(CCArmature *, MovementEventType, const char *)
{
	m_petAnimation->getAnimation()->setMovementEventCallFunc(NULL, NULL);
	m_petAnimation->getAnimation()->play("standby");
}

int StagePetNode::getColor()
{
	return m_model->getPetData().color;
}

void StagePetNode::onPetEnergyChanged(int petId)
{
	if (m_petId == petId)
	{

	}
}

void StagePetNode::onDesignatedStarChanged(int starType, int color, int rounds)
{
	auto pet = PetManager::petMgr()->getPetById(m_petId);
	if (!pet || pet->getPetData().commonid != kPetRooster) return;

	if (rounds <= 0)
	{
		m_stateLayout->setVisible(false);
	}
	else
	{
		m_stateLayout->setVisible(true);

		CCSprite *star = dynamic_cast<CCSprite *>(m_stateLayout->getChildById(4));
		auto config = DataManagerSelf->getStarsColorConfig(color);
		auto resPath = config.colorStarRes;
		star->initWithFile(resPath.c_str());

		CCLabelAtlas *num = dynamic_cast<CCLabelAtlas *>(m_stateLayout->getChildById(5));
		char str[100] = { 0 };
		sprintf(str, ";%d", rounds);
		num->setString(str);
	}
	
}

void StagePetNode::onUsePetSkill(int petId)
{
	playHappyAction(false);
}

void StagePetNode::setIconVisible(bool visible)
{
	m_skillIcon->setVisible(visible);

	float petImgY = m_layout->getChildById(4)->getPositionY();
	if (visible)
	{
		m_layout->getChildById(6)->setPositionY(petImgY - 110);
		m_layout->getChildById(7)->setPositionY(petImgY - 110);
	}
	else
	{
		m_layout->getChildById(6)->setPositionY(petImgY - 70);
		m_layout->getChildById(7)->setPositionY(petImgY - 70);
	}

}