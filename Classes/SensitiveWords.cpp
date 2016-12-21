#include "SensitiveWords.h"
#include "CommonUtil.h"
USING_NS_CC;
using namespace std;

SensitiveWords *SensitiveWords::theModel()
{
	static SensitiveWords model;
	return &model;
}

SensitiveWords::SensitiveWords()
{
	m_recordPath = "data/sensitive_words";
}

bool SensitiveWords::isSensitiveWord(std::string text)
{
	std::string strPath = CCFileUtils::sharedFileUtils()->fullPathForFilename(m_recordPath.c_str());
	unsigned long len = 0;
	unsigned char* data = NULL;
	data = CCFileUtils::sharedFileUtils()->getFileData(strPath.c_str(), "r", &len);
	
	string str = std::string((const char*)data, len);
	delete[]data;
	auto words = CommonUtil::split(str, ",");

	bool exist = false;
	for (size_t i = 0; i < words.size(); ++i)
	{
		if (text.find(words[i]) != string::npos)
		{
			exist = true;
			break;
		}
	}

	return exist;
}