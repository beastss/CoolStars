#ifndef __COMMONUTIL_H__
#define __COMMONUTIL_H__
#include "cocos2d.h"
#include "DataConfig.h"
namespace CommonUtil
{	
	std::vector<std::string> split(std::string str, std::string pattern);
	std::vector<int> parseStrToInts(std::string str);//将类似 1,2,3 的字符串分割返回为数字组
	std::string parseIntsToStr(const std::vector<int> &values);
	std::vector<float> parseStrToFloats(std::string str);

	const char *intToStr(int value);

	int getRandomValue(int begin, int end);
	bool floatEqual(float v1, float v2);
	std::vector<int> buildRandomSequence(int length);
	int getResultByPercent(const std::vector<float> &percents);
	bool isSelected(int percent);

	std::vector<int> getDifference(std::vector<int> v1, std::vector<int> v2);
	
	cocos2d::CCAnimate *getFrameAnimation(std::string pathFormat, int num, float duration);
	cocos2d::CCMenuItemSprite *getScaleMenuItemSpr(std::string path);
	std::vector<GoodsData> getGoodsDatas(const std::string &str);
	cocos2d::CCAction *getScaleAction(bool loop = true, float scale = 0.8f, float toScaleTime = 0.6f, float toNormalTime = 0.4f);
}

#endif