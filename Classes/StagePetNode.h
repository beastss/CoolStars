#ifndef __STAGEPETNODE_H__
#define __STAGEPETNODE_H__
#include "cocos2d.h"
#include "TouchNode.h"
#include <functional>
#include "CocoStudio/Armature/CCArmature.h"
#include "PetManager.h"
#include "StarsController.h"

class UiLayout;
class PetEntity;
class StagePetSkillIconProgress;
class StagePetNode
	: public cocos2d::CCNode
	, public cocos2d::CCTouchDelegate
	, public IStarsControlView
{
public:
	static StagePetNode *create(int petId, int touchPriority);
	virtual ~StagePetNode(){}
	virtual bool init();
	//技能查看
	void setTouchHandle(std::function<void(int)> handle){ m_touchHandle = handle; }
	int getColor();
	void playHappyAction(bool loop);
private:
	StagePetNode(int petId, int touchPriority);
	virtual void onEnter();
	virtual void onExit();
	virtual void onPetEnergyChanged(int petId);
	void initLayout();
	virtual bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	bool isInside(cocos2d::CCPoint pt);
	void runNormalAction(cocos2d::extension::CCArmature *, cocos2d::extension::MovementEventType, const char *);
	virtual void onDesignatedStarChanged(int starType, int color, int rounds);
private:
	int m_petId;
	const PetEntity *m_model;
	int m_touchPriority;
	UiLayout *m_layout;
	UiLayout *m_stateLayout;
	std::function<void(int)> m_touchHandle;
	cocos2d::extension::CCArmature *m_petAnimation;
	StagePetSkillIconProgress *m_skillIcon;
};

#endif