#ifndef __PANELLLAYER_H__
#define __PANELLLAYER_H__

#define FADE_COUNT1 4
#define FADE_COUNT2 6
#define FADE_COUNT3 4

#define MAX_UPDATE_STEPS 80
#define UPDATE_COINS_TIME 1
#include "cocos2d.h"
USING_NS_CC;

enum {
    /* the orders of following pens should be the same as
       g_starFrameNameArray */
    TAG_STAR_1 = 0,
    TAG_STAR_2,
    TAG_STAR_3,
    TAG_STAR_4,
    TAG_STAR_5,
    TAG_BOMB,
    TAG_PAUSE_BUTTON,
    TAG_MENU,
	TAG_BUY_MENU,
    TAG_PAINT,
	TAG_RAINBOW,
    TAG_SHOP,
	TAG_HELP,
	TAG_MAGIC_BACK,
	TAG_MAGIC_ARROW,
	TAG_EXIT,
	TAG_LOGIN,
	TAG_MAX
};

typedef void (CCObject::*SEL_CallFuncI)(int);
#define callfuncI_selector(_SELECTOR) (SEL_CallFuncI)(&_SELECTOR)

typedef struct {
    CCObject *pTarget;
    SEL_CallFuncI pCallback;
} OnClickListener;

class StageUiPanel :
    public CCLayer
{
public:
    StageUiPanel(void);
    ~StageUiPanel(void);

    /** create one layer */
    static StageUiPanel *create();

    virtual bool init();

    // default implements are used to call script callback if exist
    virtual void ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent);
    virtual void ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent);
    virtual void ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent);
    virtual void ccTouchesCancelled(CCSet *pTouches, CCEvent *pEvent);

    virtual void setTouchEnabled(bool value);

    void setBestScore(int bestScore);
    void setStage(int stage);
    void setTarget(int target);
	void setStep(int step);
    void setScore(int score);
    void setScore(int targetScore, float duration);
    void setCoins(int coins);
	void showCoinsAni();
	void setTargetCoins(int target,float delayTime = 0);
	void updateCoins(float dt);
    void deliveryScore(const CCPoint &from, int totalScore, int count);
    void updateScore(float dt);
    void setScoreHint(const char* hint);
    void setStageClear(bool clear);
    void menuCallback(CCObject *pSender);
    void toggleSkill(int tag);
    void disableSkill();

    inline void setOnClickListener(CCObject *pTarget, SEL_CallFuncI pCallback) {
        if (pTarget && pCallback){
            pTarget->retain();
            m_obOnClickListener.pTarget = pTarget;
            m_obOnClickListener.pCallback = pCallback;
        }
    }
	void updateProCountLabel(int proType);
private:
    void invokeListener(CCNode *pSender);

public:
	CCLabelTTF *m_pCoins;

private:
    bool m_bSpritePressed;
    CCSprite *m_pPause;
    CCLabelTTF *m_pStageLabel;
    CCLabelTTF *m_pTargetLabel;
    CCLabelTTF *m_pScoreLabel;
	CCLabelTTF *m_pStepLabel;
    CCLabelTTF *m_pScoreHint;
    
	CCLabelTTF *m_pBombLabel;
	CCLabelTTF *m_pPaintLabel;
	CCLabelTTF *m_pReflashLabel;
    CCSprite *m_pStageClear;
    CCSprite *m_pSelectedSkill;
    CCPoint m_obStageClearPosition;
    OnClickListener m_obOnClickListener;
    int m_nCurrentScore;
    int m_nTargetScore;
    int m_nUpdateDelta;
    float m_fUpdateInterval;
	int m_nCurrentCoins;
	int m_nTargetCoins;
	float m_nUpdateCoinsInterval;
	float m_fUpdateCoinsAllTime;
	CCNode* fadeNode1[FADE_COUNT1];
	CCNode* fadeNode2[FADE_COUNT2];
	CCNode* fadeNode3[FADE_COUNT2];
};
#endif // __PANELLLAYER_H__