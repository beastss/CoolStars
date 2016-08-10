#ifndef __GUIDEMACROS_H__
#define __GUIDEMACROS_H__

#define kNotTriggerGuide 0
enum GuideStartAction
{
	kGuideStart_common = 1,
	kGuideStart_key_run_out,
	kGuideStart_mainMenu_in = 10,

	kGuideStart_preStage_in = 20,

	kGuideStart_stage_in = 30,
	kGuideStart_stage_showTargetFinished,
	kGuideStart_stage_petFullPower,
	kGuideStart_stage_in_pet_skill_state,

	kGuideStart_ranking_in = 60,

	kGuideStart_lottery_in = 70,
	kGuideStart_lottery_use_all_keys,

	kGuideStart_pet_in = 80,
	kGuideStart_pet_color_select,
	kGuideStart_pet_upgrade,

	kGuideStart_guide_in = 90,
};

enum GuideEndAction
{
	kGuideEnd_Default = 0,//不需要向GuideMgr 发送endAction
	kGuideEnd_common = 1,

	kGuideEnd_mainMenu_in = 10,

	kGuideEnd_preStage_in = 20,
	kGuideEnd_preStage_select_pet,

	kGuideEnd_stage_in = 30,
	kGuideEnd_star_clicked,
	kGuideEnd_pet_clicked,
	kGuideStart_stage_out_pet_skill_state,
	kGuideStart_hide_skill_mask_layer,
	kGuideEnd_pet_use_skill,
	kGuideEnd_stage_gameOver,

	kGuideEnd_ranking_in = 60,

	kGuideEnd_lottery_in = 70,
	kGuideEnd_lottery_click_pet_btn,

	kGuideEnd_pet_in = 80,
	kGuideEnd_pet_color_select,
	kGuideEnd_pet_upgrade,

	kGuideEnd_guide_in = 90,
	kGuideEnd_guide_click,

};
#endif