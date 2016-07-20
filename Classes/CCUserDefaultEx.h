#ifndef __SUPPORT_CCUSERDEFAULTEX_H__
#define __SUPPORT_CCUSERDEFAULTEX_H__

#include <string>

//0A,0D回车换行，需要避免
#define ENC_KEY_DEFAULT "BdlO"
#define ENC_KEY_TWO "eIxN"
/**
 * @addtogroup data_storage
 * @{
 */

/**
 * CCUserDefault acts as a tiny database. You can save and get base type values by it.
 * For example, setBoolForKey("played", true) will add a bool value true into the database.
 * Its key is "played". You can get the value of the key by getBoolForKey("played").
 * 
 * It supports the following base types:
 * bool, int, float, double, string
 */
class CCUserDefaultEx
{
public:
    ~CCUserDefaultEx();

    // get value methods

    /**
    @brief Get bool value by key, if the key doesn't exist, a default value will return.
     You can set the default value, or it is false.
    */
    bool    getBoolForKey(const char* pKey);
    bool    getBoolForKey(const char* pKey, bool defaultValue);
	bool	getBoolForKey(const char* pKey, bool defaultValue,const char* key);
    /**
    @brief Get integer value by key, if the key doesn't exist, a default value will return.
     You can set the default value, or it is 0.
    */
    int     getIntegerForKey(const char* pKey);
    int     getIntegerForKey(const char* pKey, int defaultValue);
	int     getIntegerForKey(const char* pKey, int defaultValue,const char* key);
    /**
    @brief Get float value by key, if the key doesn't exist, a default value will return.
     You can set the default value, or it is 0.0f.
    */
    float    getFloatForKey(const char* pKey);
    float    getFloatForKey(const char* pKey, float defaultValue);
	float    getFloatForKey(const char* pKey, float defaultValue,const char* key);
    /**
    @brief Get double value by key, if the key doesn't exist, a default value will return.
     You can set the default value, or it is 0.0.
    */
    double  getDoubleForKey(const char* pKey);
    double  getDoubleForKey(const char* pKey, double defaultValue);
	double  getDoubleForKey(const char* pKey, double defaultValue,const char* key);
    /**
    @brief Get string value by key, if the key doesn't exist, a default value will return.
    You can set the default value, or it is "".
    */
    std::string getStringForKey(const char* pKey);
    std::string getStringForKey(const char* pKey, const std::string & defaultValue);
	std::string getStringForKey(const char* pKey, const std::string & defaultValue,const char* key);
    // set value methods

    /**
    @brief Set bool value by key.
    */
    void    setBoolForKey(const char* pKey, bool value);
	void    setBoolForKey(const char* pKey, bool value,const char* key);
    /**
    @brief Set integer value by key.
    */
    void    setIntegerForKey(const char* pKey, int value);
	void    setIntegerForKey(const char* pKey, int value,const char* key);
    /**
    @brief Set float value by key.
    */
    void    setFloatForKey(const char* pKey, float value);
	void    setFloatForKey(const char* pKey, float value,const char* key);
    /**
    @brief Set double value by key.
    */
    void    setDoubleForKey(const char* pKey, double value);
	void    setDoubleForKey(const char* pKey, double value,const char* key);
    /**
    @brief Set string value by key.
    */
    void    setStringForKey(const char* pKey, const std::string & value);
	void    setStringForKey(const char* pKey, const std::string & value,const char* key);
    /**
     @brief Save content to xml file
     */
    void    flush();

    static CCUserDefaultEx* sharedUserDefault();
    static void purgeSharedUserDefault();

	static void xorEncDec(std::string& txt, const char* key);
	static void encKeyName(std::string& keyName);

	static std::string G2U(const char* gb2312);
private:
    CCUserDefaultEx();
    
    static CCUserDefaultEx* m_spUserDefault;
};

#endif // __SUPPORT_CCUSERDEFAULT_H__
