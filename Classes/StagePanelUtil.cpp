#include "StagePanelUtil.h"
#include "PetManager.h"
#include "UiLayout.h"
#include "EmptyBox.h"
#include "SoundMgr.h"
USING_NS_CC;
using namespace std;

StagePetSkillPanel *StagePetSkillPanel::create(int touchPriority)
{
	auto panel = new StagePetSkillPanel(touchPriority);
	panel->init();
	panel->autorelease();
	return panel;
}

bool StagePetSkillPanel::init()
{
	auto winSize = CCDirector::sharedDirector()->getWinSize();
	CCLayerColor *bk = CCLayerColor::create(ccc4(0, 0, 0, 200));
	bk->setContentSize(winSize);
	addChild(bk);

	m_layout = UiLayout::create("layout/stage_pet_skills.xml");
	int uiIds[] = { 1, 2, 3, 4 };
	auto ids = PetManager::petMgr()->getCurPetIds();
	for (size_t i = 0; i < ids.size(); ++i)
	{
		auto petModel = PetManager::petMgr()->getPetById(ids[i]);
		string skillIcon = petModel->getPetData().petSkillRes;
		
		CCSprite *skillSpr = CCSprite::create(skillIcon.c_str());
		auto node = dynamic_cast<EmptyBox *>((m_layout->getChildById(uiIds[i])));
		node->setNode(skillSpr);
		node->setAnchorPoint(ccp(0.5f, 0.5f));
	}

	m_layout->setAnchorPoint(ccp(0, 1));
	m_layout->setPosition(ccp(0, winSize.height));
	addChild(m_layout);
	setContentSize(winSize);
	return true;
}


bool StagePetSkillPanel::onTouchBegan(cocos2d::CCPoint pt, bool isInside)
{
	removeFromParent();
	return isInside;
}


///////////////////////////////////////////////////////////////////////
ChangeStarColorPanel::ChangeStarColorPanel(int myColor, int touchPriority)
: TouchNode(touchPriority)
{
	
	for (int i = 0; i < COLOR_AMOUNT; ++i)
	{
		int color = i + kColorRed;
		if (color != myColor)
		{
			m_colors.push_back(color);
		}
	}
}

ChangeStarColorPanel *ChangeStarColorPanel::create(int myColor, int touchPriority)
{
	auto panel = new ChangeStarColorPanel(myColor, touchPriority);
	panel->init();
	panel->autorelease();
	return panel;
}

bool ChangeStarColorPanel::init()
{
	m_layout = UiLayout::create("layout/stage_change_star_color.xml");
	auto size = m_layout->getContentSize();
	int uiIds[] = { 1, 2, 3, 4};
	
	for (size_t i = 0; i < m_colors.size(); ++i)
	{
		int color = m_colors[i];
		auto config = DataManagerSelf->getStarsColorConfig(color);
		auto resPath = config.colorStarRes;

		CCSprite *starSpr = CCSprite::create(resPath.c_str());
		auto node = dynamic_cast<EmptyBox *>((m_layout->getChildById(uiIds[i])));
		node->setNode(starSpr);
	}

	CCLayerColor *bk = CCLayerColor::create(ccc4(0, 0, 0, 200));
	bk->setContentSize(size);
	addChild(bk);

	addChild(m_layout);
	setContentSize(size);

	return true;
}

bool ChangeStarColorPanel::onTouchBegan(cocos2d::CCPoint pt, bool isInside)
{
	int uiIds[4] = { 1, 2, 3, 4};
	int color = kColorRandom;
	bool isSelect = false;
	for (int i = 0; i < 4; ++i)
	{
		CCNode *node = m_layout->getChildById(uiIds[i]);
		auto localPos = convertToNodeSpace(pt);
		if (node->boundingBox().containsPoint(localPos))
		{
			SoundMgr::theMgr()->playEffect(kEffectMusicButton);
			color = m_colors[i];
			isSelect = true;
			break;
		}
	}

	if (m_callback)
	{
		m_callback(isSelect, color);
	}
	removeFromParent();
	return true;
}
