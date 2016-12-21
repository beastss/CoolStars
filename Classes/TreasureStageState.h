#ifndef _TREASURESTAGESTATE_H_
#define	_TREASURESTAGESTATE_H_

class TreasureStageState
{
public:
	static TreasureStageState *theState();
	//base data
	void reset();
	int getCurStage(){ return m_curStage; }
	void nextStage(bool skipCurStage);
	void eraseStars(int);
	bool isWon();
	bool isLastStage();

	int getCurScore(){return m_curScore; }
	int getTargeScore();

	int getCurFoodReward(){ return m_curFoodReward; }//当前累积的饲料
	int getStageFoodReward();//本关能获取的奖励

	int getCurDiamondReward(){ return m_curDiamondReward; }
	int getStageDiamondReward();

	//control
	void block();
	void resume();
	bool canTouch();
private:
	TreasureStageState();
private:
	int m_state;
	int m_curStage;
	int m_curScore;
	bool m_passedCurStage;
	int m_curFoodReward;
	int m_curDiamondReward;
public:
	enum
	{
		kStateNormal,//正常
		kStateBlock,//阻塞
	};
};

#endif