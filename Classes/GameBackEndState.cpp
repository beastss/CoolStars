#include "GameBackEndState.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID )
#include "org_cocos2dx_lib_DongXinPush.h"
#endif

using namespace std;

GameBackEndState::GameBackEndState()
: m_isBusinessMode(false)
{

}

GameBackEndState *GameBackEndState::theModel()
{
	static GameBackEndState model;
	return &model;
}

