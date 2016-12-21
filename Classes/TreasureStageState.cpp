#include "TreasureStageState.h"
#include "TreasureStageDataMgr.h"
#include "TreasureStageNotifier.h"
using namespace std;

TreasureStageState::TreasureStageState()
{
	reset();
}

void TreasureStageState::reset()
{
	m_curStage = 1;
	m_state = kStateNormal;
	m_curScore = 0;
	m_passedCurStage = false;
	m_curFoodReward = 0;
	m_curDiamondReward = 0;
}

TreasureStageState *TreasureStageState::theState()
{
	static TreasureStageState state;
	return &state;
}

bool TreasureStageState::canTouch()
{
	return m_state == kStateNormal;
}

void TreasureStageState::block()
{
	m_state = kStateBlock;
}

void TreasureStageState::resume()
{
	m_state = kStateNormal;
}

void TreasureStageState::nextStage(bool skipCurStage)
{
	if (skipCurStage)
	{
		m_curScore = TreasureStageDataMgrSelf->getStageData(m_curStage).targeSocre;
	}
	m_curFoodReward += getStageFoodReward();
	m_curDiamondReward += getStageDiamondReward();
	m_curStage++;
	m_state = kStateNormal;
	m_passedCurStage = false;
}

void TreasureStageState::eraseStars(int num)
{
	int score = TreasureStageDataMgrSelf->getEraseBonusData(num).score;
	m_curScore += score;
	TreasureStageNotifier::theNotifier()->onEraseStars(num, score);
	if (!m_passedCurStage && isWon())
	{
		TreasureStageNotifier::theNotifier()->onPassCurStage(num);
		m_passedCurStage = true;
	}
}

bool TreasureStageState::isWon()
{
	return m_curScore >= getTargeScore();
}

int TreasureStageState::getTargeScore()
{
	return TreasureStageDataMgrSelf->getStageData(m_curStage).targeSocre;
}

bool TreasureStageState::isLastStage()
{
	return m_curStage >= TreasureStageDataMgrSelf->getStageNum();
}

int TreasureStageState::getStageFoodReward()
{
	return TreasureStageDataMgrSelf->getStageData(m_curStage).food;
}

int TreasureStageState::getStageDiamondReward()
{
	return TreasureStageDataMgrSelf->getStageData(m_curStage).diamond;
}