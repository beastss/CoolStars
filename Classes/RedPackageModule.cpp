#include "RedPackageModule.h"
#include "StageDataMgr.h"
#include "CommonUtil.h"
#include "GoodsMgr.h"
#include "DataManager.h"
#include "PackageDialog.h"
#include "MainScene.h"
#include "SqliteHelper.h"
#include "RedPackageDialog.h"
#include "PackageModel.h"

using namespace std;
USING_NS_CC;
using namespace CommonUtil;

RedPackageModule::RedPackageModule()
: m_everGetRedPackage(false)
, m_alreadyGetThisRedPackage(false)
{
	init();
}

void RedPackageModule::init()
{
	char str[100] = { 0 };
	SqliteHelper helper(DB_SAVING);
	string sql = "select * from save_state";
	auto result = helper.readRecord(sql.c_str());
	assert(result.size() == 1);
	auto data = result[0];

	m_everGetRedPackage = atoi(data[1]) == 1;
}

bool RedPackageModule::isShowUp()
{
	int curScore = StageDataMgr::theMgr()->getCurScore();
	int targetScore = StageDataMgr::theMgr()->getTargetScore();
	return (float)curScore / targetScore >= 0.5f && !m_alreadyGetThisRedPackage;
}

void RedPackageModule::openRedPackage()
{
	if (m_alreadyGetThisRedPackage) return;
	int redPackageType = kRedPackageDiamond;
	if (m_everGetRedPackage)
	{
		vector<float> percents;
		//红包60%获得钻石，40%开启钻石礼包
		percents.push_back(60);
		percents.push_back(40);

		redPackageType = getResultByPercent(percents);
	}
	else
	{
		m_everGetRedPackage = true;
		SqliteHelper sqlHelper(DB_SAVING);
		char str[100] = { 0 };
		sprintf(str, "update save_state set %s = '%d' where id = 1;", "ever_get_red_package", m_everGetRedPackage ? 1: 0);
		sqlHelper.executeSql(str);
	}

	if (redPackageType == kRedPackageDiamond)
	{
		RedPackageDialog *dialog = RedPackageDialog::create();
		MainScene::theScene()->showDialog(dialog);
	}
	else
	{
		PackageDialog *dialog = PackageDialog::create(kPackageDiamond);
		MainScene::theScene()->showDialog(dialog);
	}
	m_alreadyGetThisRedPackage = true;

}