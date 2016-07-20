#include "cocos2d.h"
#define Z_ORDER_BACKGROUND 0
#define Z_ORDER_STAR (Z_ORDER_BACKGROUND + 1)
#define Z_ORDER_EXPLOSION (Z_ORDER_STAR + 1)
#define Z_ORDER_PANEL (Z_ORDER_EXPLOSION + 1)
#define Z_ORDER_SHOP (Z_ORDER_PANEL + 1)
#define Z_ORDER_MENU (Z_ORDER_SHOP + 1)
#define Z_ORDER_UNLOCK (Z_ORDER_MENU + 1)
#define Z_ORDER_FAILED (Z_ORDER_UNLOCK + 1)
#define Z_ORDER_BUY (Z_ORDER_FAILED + 1)
#define Z_MENU_LAYER_TAG (Z_ORDER_BUY + 1)

#define TAG_PAUSE 1
#define TAG_BONUS (TAG_PAUSE + 1)
#define TAG_SHOP_LAYER (TAG_BONUS + 1)
#define TAG_FALIED_LAYER (TAG_SHOP_LAYER + 1)
#define TAG_BUY_LAYER (TAG_FALIED_LAYER + 1)

#define TAG_GAMEOVER_BACK 1
#define TAG_GAMEOVER_COINTINE 2
#define TAG_GAMEOVER_MENU 3
#define TAG_GAMEOVE_TIME 4

#define SHOW_TIPS_TIME 6
#define SHOW_TIPS_LATER 5

#define REMAIN_CONTINUE_TIME 10

static const char s_fire[] = "particle/fire.png";
static const std::string s_stars[] = {
	"particle/starp1.png",
	"particle/starp2.png",
	"particle/starp3.png",
	"particle/starp4.png",
	"particle/starp5.png"
};