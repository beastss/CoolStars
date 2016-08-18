#include "CommonUtil.h"
#include <algorithm>
using namespace cocos2d;
using namespace std;
vector<string> CommonUtil::split(string str, string pattern)
{
	string::size_type pos;
	vector<string> result;
	str += pattern;//扩展字符串以方便操作
	int size = str.size();

	for (int i = 0; i < size; i++)
	{
		pos = str.find(pattern, i);
		if (pos < size)
		{
			string s = str.substr(i, pos - i);
			result.push_back(s);
			i = pos + pattern.size() - 1;
		}
	}
	return result;
}

vector<int> CommonUtil::parseStrToInts(string str)
{
	vector<int> ints;
	if (!str.empty())
	{
		auto strs = split(str, ",");
		for (size_t i = 0; i < strs.size(); ++i)
		{
			ints.push_back(atoi(strs[i].c_str()));
		}
	}
	return ints;
}

vector<float> CommonUtil::parseStrToFloats(string str)
{
	vector<float> floats;
	if (!str.empty())
	{
		auto strs = split(str, ",");
		for (size_t i = 0; i < strs.size(); ++i)
		{
			floats.push_back(atof(strs[i].c_str()));
		}
	}
	return floats;
}

string CommonUtil::parseIntsToStr(const vector<int> &values)
{
	string str;
	for (size_t i = 0; i < values.size(); ++i)
	{
		str += intToStr(values[i]);
		if (i < values.size() - 1)
		{
			str += ",";
		}
	}
	return str;
}

const char *CommonUtil::intToStr(int value)
{
	static char str[100];
	sprintf(str, "%d", value);
	return str;
}

//end 不超过10000
int CommonUtil::getRandomValue(int begin, int end)
{
	assert(begin <= end);
	return ((int)(CCRANDOM_0_1() * 10000)) % (end - begin + 1) + begin;
}

int CommonUtil::getResultByPercent(const std::vector<float> &percents)
{
	assert(!percents.empty());
	float result = getRandomValue(1, 10000) / 100;
	float curPercent = 0;
	for (int i = 0; i < percents.size(); ++i)
	{
		curPercent += percents[i];
		if (curPercent >= result)
		{
			return i;
		}
	}

	return 0; 
}

bool CommonUtil::floatEqual(float v1, float v2)
{
	return (fabs(v1 - v2) < 0.000001);
}


//生成 [0 , length-1] 的随机序列
vector<int> CommonUtil::buildRandomSequence(int length)
{
	vector<int> seq;
	for (int i = 0; i < length; ++i)
	{
		seq.push_back(i);
	}
	int index = 0;
	int tmp = 0;
	for (int i = length - 1; i > 0; i--) {
		index = (int(CCRANDOM_0_1() * 100)) % i;

		tmp = seq[i];
		seq[i] = seq[index];
		seq[index] = tmp;
	}

	return seq;
}

vector<int> CommonUtil::getDifference(std::vector<int> v1, std::vector<int> v2)
{
	sort(v1.begin(), v1.end());
	sort(v2.begin(), v2.end());
	vector<int> result(v1.size());
	auto iter = set_difference(v1.begin(), v1.end(), v2.begin(), v2.end(), result.begin());

	return vector<int>(result.begin(), iter);
}

bool CommonUtil::isSelected(int percent)
{
	return getRandomValue(0, 100) <= percent;
}

CCAnimate *CommonUtil::getFrameAnimation(string pathFormat, int num, float duration)
{
	CCAnimation *animation = CCAnimation::create();
	for (int i = 0; i < num; i++)
	{
		char szImageFileName[100] = { 0 };
		sprintf(szImageFileName, pathFormat.c_str(), i);
		animation->addSpriteFrameWithFileName(szImageFileName);
	}
	animation->setDelayPerUnit(duration / num); // 这个动画包含14帧，将会持续2.8秒.  
	//animation->setRestoreOriginalFrame(true); // 14帧播放完之后返回到第一帧  

	CCAnimate *action = CCAnimate::create(animation);
	return action;
}

CCMenuItemSprite *CommonUtil::getScaleMenuItemSpr(string path)
{
	auto normalSpr = CCSprite::create(path.c_str());
	auto selectedSpr = CCSprite::create(path.c_str());
	float scale = 1.1f;
	selectedSpr->setScale(scale);

	CCMenuItemSprite* imageItem = CCMenuItemSprite::create(normalSpr, selectedSpr, NULL, NULL);
	selectedSpr->setAnchorPoint(ccp((scale - 1.0f) / 2, (scale - 1.0f) / 2));
	return imageItem;
}

std::vector<GoodsData> CommonUtil::getGoodsDatas(const std::string &str)
{
	auto values = parseStrToInts(str.c_str());
	assert(values.size() % 3 == 0);
	std::vector<GoodsData> datas;
	for (size_t i = 0; i < values.size(); i += 3)
	{
		GoodsData data;
		data.type = values[i];
		data.goodsId = values[i + 1];
		data.amount = values[i + 2];
		datas.push_back(data);
	}
	return datas;
}

cocos2d::CCAction *CommonUtil::getRepeatScaleAction(float scale, float toScaleTime, float toNormalTime)
{
	auto scaleSmall = CCScaleTo::create(toScaleTime, scale);
	auto scaleNormal = CCScaleTo::create(toNormalTime, 1.0f);

	auto seq = CCRepeatForever::create(CCSequence::create(scaleSmall, scaleNormal, NULL));
	return seq;
}
