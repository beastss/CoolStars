#ifndef __STAGESCENESTATE_H__
#define __STAGESCENESTATE_H__
#include "cocos2d.h"
#include "StarNode.h"
class StarsLayer;
class StageUiLayer;
class StageStateOwner;

class StageSceneState
{
public:
	virtual void clickStar(const LogicGrid &gird){}
	virtual void clickPet(int petId){}
	virtual void clickProp(int propId){}

protected:
	StageSceneState(StageStateOwner *owner);
protected:
	StarsLayer *m_starsLayer;
	StageUiLayer *m_uiLayer;
	StageStateOwner *m_owner;
};

class StageNormalState : public StageSceneState
{
public:
	StageNormalState(StageStateOwner *owner) : StageSceneState(owner){}
public:
	virtual void clickStar(const LogicGrid &grid);
	virtual void clickPet(int petId);
	virtual void clickProp(int propId);
};


class StagePropsClickState: public StageSceneState
{
public:
	StagePropsClickState(StageStateOwner *owner) : StageSceneState(owner){}
	void setCurProp(int propType){ m_curType = propType; }
public:
	virtual void clickStar(const LogicGrid &grid);
	virtual void clickPet(int petId);
	int m_curType;
};

class StagePetSkillState : public StageSceneState
{
public:
	StagePetSkillState(StageStateOwner *owner) : StageSceneState(owner){}
	void setCurPet(int petId){ m_curPetId = petId; }
public:
	virtual void clickStar(const LogicGrid &grid);
	virtual void clickPet(int petId);
private:
	void outPetSkillState();
private:
	int m_curPetId;
};

/*owner*/
class StageStateOwner
{
public:
	StageStateOwner();
	~StageStateOwner();
	void init();
	void setStarsLayer(StarsLayer *starsLayer){ m_starsLayer = starsLayer; }
	void setUiLayer(StageUiLayer *uiLayer){ m_uiLayer = uiLayer; }
	StarsLayer *getStarsLayer(){ return m_starsLayer; }
	StageUiLayer *getUiLayer(){ return m_uiLayer; }
public:
	void enterNormalState();
	void enterPropsClickState(int propType);
	void enterPetSkillState(int petId);
public:
	void clickStar(const LogicGrid &grid);
	void clickPet(int petId);
	void clickProp(int propId);
private:
	StarsLayer *m_starsLayer;
	StageUiLayer *m_uiLayer;
	StageSceneState *m_curState;
private:
	StagePropsClickState *m_propsState;
	StageNormalState *m_normalState;
	StagePetSkillState *m_petSkillState;
};
#endif