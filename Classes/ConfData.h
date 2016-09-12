#ifndef __CONFDATA_H__
#define __CONFDATA_H__
#include "cocos2d.h"
#include"cocos-ext.h"
USING_NS_CC;
USING_NS_CC_EXT;  
using namespace rapidjson;

#define XML_DATA "data/strings.xml"

#define BALL_QUEUE_SIZE 10

#define DBG 1

#define DESIGN_WIDTH 480
#define DESIGN_HEIGHT 800
#define COlUMNS_SIZE 8
#define ROWS_SIZE 8
#define STAR_SIZE 54 

#define MENU_ITEM_SEL_SCALE 1.1f
#define MENU_ITEM_SEL_ANCHOR ccp((MENU_ITEM_SEL_SCALE-1.0f)/2,(MENU_ITEM_SEL_SCALE-1.0f)/2)

//////////////  game mode   //////////////
#define GAME_MODE_SCORE                 1
#define GAME_MODE_STAR                  2
#define GAME_MODE_TOOL                  4
#define GAME_MODE_SPRITE                8

enum OnCloseAction{
	ONCLOSE_REMOVESELF = 0,
	ONCLOSE_GOTOMENU
};
enum Direction{
	DIC_UP = 1,
	DIC_DOWN,
	DIC_LEFT,
	DIC_RIGHT
};

#define PAY_RESULT_SUCCESS 0
#define PAY_RESULT_FAILED 1
#define PAY_RESULT_CANCEL 2
#define PAY_RESULT_TIMEOUT 3

//二次确认
#define PAY_DOUBLE_SHOW 0
#define SHOW_LGOIN 0

#define USE_ANALYTICS 0
#define PAY_NO_PAY 1
#define PAY_TYPE_MM 0
#define PAY_TYPE_JIDI 0
#define HAVE_ONE_CENT 1

typedef enum {
	PAY_TYPE_DIAMOND1, //钻石1
	PAY_TYPE_DIAMOND2, //钻石2
	PAY_TYPE_DIAMOND3, //钻石3
	PAY_TYPE_DIAMOND4, //钻石4
	PAY_TYPE_DIAMOND5, //钻石5
	PAY_TYPE_FISH_LIBAO, //新手礼包
	PAY_TYPE_EXIT_LIBAO     //退出礼包
} MyPayProducts;

typedef enum {
	CARRIET_TYPE_UNKNOWN, // 未知
	CARRIET_TYPE_CM, // 移动
	CARRIET_TYPE_CU,// 联通
	CARRIET_TYPE_CT// 电信
} CarrietType;

typedef void (CCObject::*SEL_CallFuncN3I)(CCNode*, int, int,int);
#define callfuncN3I_selector(_SELECTOR) (SEL_CallFuncN3I)(&_SELECTOR)

#endif