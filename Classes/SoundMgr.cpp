#include "SoundMgr.h"
#include "cocos2d.h"
#include "CCUserDefaultEx.h"
#include "DataManager.h"
#include "SqliteHelper.h"
USING_NS_CC;
using namespace CocosDenshion;

SoundMgr *SoundMgr::theMgr()
{
	static SoundMgr mgr;
	return &mgr;
}

void SoundMgr::init()
{
	auto sound = DataManagerSelf->getSoundConfig();
	m_backgroundMusic[kBackGroundMusicMain] = sound.background;
	m_effectMusic[kEffectMusicButton] = sound.button;
	m_effectMusic[kEffectMusicBomb] = sound.bomb;
	m_effectMusic[kEffectStarErase] = sound.starErase;

	auto audioMgr = SimpleAudioEngine::sharedEngine();
	for (auto iter = m_backgroundMusic.begin(); iter != m_backgroundMusic.end(); ++iter)
	{
		audioMgr->preloadBackgroundMusic(iter->second.c_str());
	}

	for (auto iter = m_effectMusic.begin(); iter != m_effectMusic.end(); ++iter)
	{
		SimpleAudioEngine::sharedEngine()->preloadEffect(iter->second.c_str());
	}
	
	loadLastSetting();
}

void SoundMgr::playBackground()
{
	SimpleAudioEngine::sharedEngine()->playBackgroundMusic(m_backgroundMusic[kBackGroundMusicMain].c_str(), true);
}

void SoundMgr::playEffect(int effectType)
{
	if (!m_isMute)
	{
		SimpleAudioEngine::sharedEngine()->playEffect(m_effectMusic[effectType].c_str());
	}
}

void SoundMgr::setMute( bool mute )
{
	if (mute == m_isMute) return;
	if (mute)
	{
		SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
	}
	else
	{
		playBackground();
	}

	m_isMute = mute;
	saveCurSetting();
}

bool SoundMgr::isMute()
{
	return m_isMute;
}

SoundMgr::~SoundMgr()
{
	SimpleAudioEngine::sharedEngine()->end();
}

void SoundMgr::loadLastSetting()
{
	char str[100] = { 0 };
	SqliteHelper helper(DB_SAVING);
	string sql = "select * from save_state";
	auto result = helper.readRecord(sql.c_str());
	assert(result.size() == 1);
	auto data = result[0];

	bool isMute = atoi(data[2]) == 1;
	setMute(isMute);
}

void SoundMgr::saveCurSetting()
{
	SqliteHelper sqlHelper(DB_SAVING);
	char str[100] = { 0 };
	sprintf(str, "update save_state set %s = '%d' where id = 1;", "is_mute", m_isMute ? 1 : 0);
	sqlHelper.executeSql(str);
}