#include "CCUserDefaultEx.h"
#include "cocos2d.h"



USING_NS_CC;

using namespace std;

/**
 * implements of CCUserDefault
 */

CCUserDefaultEx* CCUserDefaultEx::m_spUserDefault = 0;

/**
 * If the user invoke delete CCUserDefault::sharedUserDefault(), should set m_spUserDefault
 * to null to avoid error when he invoke CCUserDefault::sharedUserDefault() later.
 */
CCUserDefaultEx::~CCUserDefaultEx(){
    m_spUserDefault = NULL;
}

CCUserDefaultEx::CCUserDefaultEx(){
	m_spUserDefault = NULL;
}

void CCUserDefaultEx::purgeSharedUserDefault(){
	CC_SAFE_DELETE(m_spUserDefault);
    m_spUserDefault = NULL;
}

bool CCUserDefaultEx::getBoolForKey(const char* pKey){
	return getBoolForKey(pKey, false);
}

bool CCUserDefaultEx::getBoolForKey(const char* pKey, bool defaultValue){
	std::string value = getStringForKey(pKey, defaultValue ? "true" : "false");
	return getBoolForKey(pKey,defaultValue,ENC_KEY_DEFAULT);
}

bool CCUserDefaultEx::getBoolForKey(const char* pKey, bool defaultValue, const char* encKey){
	std::string value = getStringForKey(pKey, defaultValue ? "true" : "false",encKey);
	return (!strcmp(value.c_str(), "true"));
}

int CCUserDefaultEx::getIntegerForKey(const char* pKey){
	return getIntegerForKey(pKey, 0);
}

int CCUserDefaultEx::getIntegerForKey(const char* pKey, int defaultValue){
	return getIntegerForKey(pKey,defaultValue,ENC_KEY_DEFAULT);
}

int CCUserDefaultEx::getIntegerForKey(const char* pKey, int defaultValue, const char* encKey){
	char tmp[50];
	memset(tmp, 0, 50);
	sprintf(tmp, "%d", defaultValue);
	std::string value = getStringForKey(pKey, tmp, encKey);
	return atoi(value.c_str());
}

float CCUserDefaultEx::getFloatForKey(const char* pKey){
	return getFloatForKey(pKey, 0.0f);
}

float CCUserDefaultEx::getFloatForKey(const char* pKey, float defaultValue){
	return getFloatForKey(pKey,defaultValue,ENC_KEY_DEFAULT);
}

float CCUserDefaultEx::getFloatForKey(const char* pKey, float defaultValue, const char* encKey){
	float ret = (float)getDoubleForKey(pKey, (double)defaultValue,encKey);
	return ret;
}

double  CCUserDefaultEx::getDoubleForKey(const char* pKey){
	return getDoubleForKey(pKey, 0.0);
}

double CCUserDefaultEx::getDoubleForKey(const char* pKey, double defaultValue){
	return getDoubleForKey(pKey,defaultValue,ENC_KEY_DEFAULT);
}

double CCUserDefaultEx::getDoubleForKey(const char* pKey, double defaultValue, const char* encKey){
	char tmp[50];
	memset(tmp, 0, 50);
	sprintf(tmp, "%f", defaultValue);
	std::string value = getStringForKey(pKey, tmp,encKey);
	return atof(value.c_str());
}

std::string CCUserDefaultEx::getStringForKey(const char* pKey){
	return getStringForKey(pKey, "");
}

string CCUserDefaultEx::getStringForKey(const char* pKey, const std::string & defaultValue){
	return getStringForKey(pKey,defaultValue,ENC_KEY_DEFAULT);
}

string CCUserDefaultEx::getStringForKey(const char* pKey, const std::string & defaultValue,const char* encKey){
	std::string def = defaultValue;
	xorEncDec(def, encKey);
	std::string keyName = pKey;
	encKeyName(keyName);
	std::string enc = CCUserDefault::sharedUserDefault()->getStringForKey(keyName.c_str(), def);
	xorEncDec(enc, encKey);
	return enc;
}

void CCUserDefaultEx::setBoolForKey(const char* pKey, bool value){
    setBoolForKey(pKey,value,ENC_KEY_DEFAULT);
}

void CCUserDefaultEx::setBoolForKey(const char* pKey, bool value, const char* encKey){
	// save bool value as string
	if (true == value){
		setStringForKey(pKey, "true", encKey);
	}else{
		setStringForKey(pKey, "false",encKey);
	}
}

void CCUserDefaultEx::setIntegerForKey(const char* pKey, int value){
	setIntegerForKey(pKey,value,ENC_KEY_DEFAULT);
}

void CCUserDefaultEx::setIntegerForKey(const char* pKey, int value, const char* encKey){
	// check key
	if (!pKey){
		return;
	}

	// format the value
	char tmp[50];
	memset(tmp, 0, 50);
	sprintf(tmp, "%d", value);
	setStringForKey(pKey, tmp,encKey);
}

void CCUserDefaultEx::setFloatForKey(const char* pKey, float value){
    setFloatForKey(pKey, value,ENC_KEY_DEFAULT);
}

void CCUserDefaultEx::setFloatForKey(const char* pKey, float value, const char* encKey){
	setDoubleForKey(pKey, value,encKey);
}

void CCUserDefaultEx::setDoubleForKey(const char* pKey, double value){
    setDoubleForKey(pKey,value,ENC_KEY_DEFAULT);
}

void CCUserDefaultEx::setDoubleForKey(const char* pKey, double value, const char* encKey){
	// check key
	if (! pKey){
		return;
	}

	// format the value
	char tmp[50];
	memset(tmp, 0, 50);
	sprintf(tmp, "%f", value);
	setStringForKey(pKey, tmp,encKey);
}

void CCUserDefaultEx::setStringForKey(const char* pKey, const std::string & value){
	setStringForKey(pKey,value,ENC_KEY_DEFAULT);
}

void CCUserDefaultEx::setStringForKey(const char* pKey, const std::string & value, const char* encKey){
	// check key
	if (!pKey){
		return;
	}
	std::string enc = value;
	xorEncDec(enc, encKey);
	std::string keyName = pKey;
	encKeyName(keyName);
	CCUserDefault::sharedUserDefault()->setStringForKey(keyName.c_str(), enc.c_str());
}

CCUserDefaultEx* CCUserDefaultEx::sharedUserDefault(){
    if (!m_spUserDefault){
        m_spUserDefault = new CCUserDefaultEx();
    }
    return m_spUserDefault;
}

void CCUserDefaultEx::flush(){
	CCUserDefault::sharedUserDefault()->flush();
}

void CCUserDefaultEx::xorEncDec(std::string& txt, const char* key ){
	int keyIdx = 0;
	int keyLen = strlen(key);
	for (unsigned int i = 0; i < txt.length(); i++) {
		txt[i] = txt[i]^key[keyIdx];
		keyIdx++;
		if (keyIdx >= keyLen) {
			keyIdx = 0;
		}
	}
}

void CCUserDefaultEx::encKeyName(std::string& keyName){
	int keyLen = strlen(keyName.c_str());
	for (int i = 0; i < keyLen; i++){
		if (keyName[i] >= 'A' && keyName[i] <= 'Z'){
			keyName[i] = ('Z' - keyName[i] + 'a');
		}else if (keyName[i] >= 'a' && keyName[i] <= 'z'){
			keyName[i] = ('z' - keyName[i] + 'A');
		}else{
			keyName[i] = 'B';
		}	
	}
}

std::string CCUserDefaultEx::G2U(const char* gb2312){
#ifdef WIN32
	int len = MultiByteToWideChar(CP_ACP, 0, gb2312, -1, NULL, 0);
	wchar_t* wstr = new wchar_t[len+1];
	memset(wstr, 0, len+1);
	MultiByteToWideChar(CP_ACP, 0, gb2312, -1, wstr, len);
	len = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, NULL, 0, NULL, NULL);
	char* str = new char[len+1];
	memset(str, 0, len+1);
	WideCharToMultiByte(CP_UTF8, 0, wstr, -1, str, len, NULL, NULL);
	if(wstr) delete[] wstr;
	std::string result = str;
	return result;
#else
	std::string result = gb2312;
	return result;
#endif
}