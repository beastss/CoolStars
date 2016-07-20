#ifndef __STAGESCENE_H__
#define __STAGESCENE_H__
#include "cocos2d.h"
#include "BasePanel.h"
#include "StageSceneState.h"

class StarsLayer;
class StarsLayer;
class StageMaskLayer;

class StageScene :
	public BasePanel
{
public:
	~StageScene();
	static StageScene *theScene();
	
	StarsLayer *getStarsLayer(){return m_starsLayer;}
	StageUiLayer *getStageUiLayer(){ return m_stageUiLayer; }
	StageStateOwner *getStateOwner(){ return &m_stateOwner; }
private:
	StageScene();
	virtual void onExit();
	virtual bool init();
private :
	StageStateOwner m_stateOwner;
	StarsLayer* m_starsLayer;
	StageUiLayer *m_stageUiLayer;
	StageMaskLayer *m_maskLayer;
private:
	static StageScene * s_scene;
};
#endif