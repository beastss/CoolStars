#include "GameBackEndState.h"
#include "StageDataMgr.h"
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

void GameBackEndState::recordStageStart()
{
}

void GameBackEndState::recordStageEnd(bool win)
{
}

void GameBackEndState::init()
{

}