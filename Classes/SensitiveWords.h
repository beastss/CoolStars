#ifndef _SENSITIVE_WORDS_H_
#define _SENSITIVE_WORDS_H_
#include "cocos2d.h"
class SensitiveWords
{
public:
	static SensitiveWords *theModel();
	bool isSensitiveWord(std::string text);
private:
	SensitiveWords();
private:
	std::string m_recordPath;
};
#endif