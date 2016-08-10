#include "PropManager.h"
#include "SqliteHelper.h"
#include "CommonUtil.h"
#include "StageOperator.h"
#include "CommonMacros.h"
#include <algorithm>
#include "StarsEraseModule.h"
#include "StarsController.h"
#include "GameDataAnalysis.h"
USING_NS_CC;
using namespace std;
using namespace CommonUtil;

void PropManager::save()
{
	SqliteHelper sqlHelper(DB_SAVING);
	char str[100] = { 0 };
	string sql = "replace into save_prop values(1,\"";
	for (size_t i = 0; i < m_propAmount.size(); ++i)
	{
		sprintf(str, "%d,", m_propAmount[i]);
		sql += str;
	}
	if (m_propAmount.size() > 0)
	{
		sql = sql.substr(0, sql.length() - 1);
	}
	sql += "\");";
	sqlHelper.executeSql(sql.c_str());
}

void PropManager::loadPropData()
{
	SqliteHelper sqlHelper(DB_SAVING);
	string sql = "select * from save_prop;";
	auto result = sqlHelper.readRecord(sql.c_str());
	assert(result.size() == 1);

	m_propAmount = parseStrToInts(result[0][1]);
}

int PropManager::getPropItemAmount(int type)
{
	assert(type >= 0 && type < kPorpTypeAmount);
	return m_propAmount[type];
}

void PropManager::setPropItemAmount(int type, int amount)
{
	assert(type >= 0 && type < kPorpTypeAmount);
	if (type >= 0 && type < kPorpTypeAmount)
	{
		m_propAmount[type] = amount;
		save();
	}
	NOTIFY_VIEWS(onPropItemChanged);
}

PropManager *PropManager::propMgr()
{
	static PropManager mgr;
	return &mgr;
}

void PropManager::usePropBomb(int starType, const LogicGrid &grid)
{
	if (starType == kBomb)
	{
		StarsEraseModule::theModel()->scaleErase(grid, COlUMNS_SIZE, ROWS_SIZE);
	}
	else
	{
		StarsEraseModule::theModel()->scaleErase(grid, 1, 1);
	}
	usePropItem(kPropBomb);
}

void PropManager::usePropBrush(const LogicGrid &grid, int color)
{
	StarNode *node = StarsController::theModel()->getStarNode(grid);
	auto attr = node->getAttr();
	attr.color = color;
	StageOp->changeColor(attr);
	usePropItem(kPropBrush);
}

void PropManager::usePropReorder()
{
	StageOp->reOrderStars();
	usePropItem(kPropReorder);
}

void PropManager::usePropItem(int propType)
{
	//StarsController::theModel()->moveOneStep();

	int amount = getPropItemAmount(propType);
	setPropItemAmount(propType, amount - 1);
	NOTIFY_VIEWS(onPropItemChanged);
	GameDataAnalysis::theModel()->consumeProps(propType);
}

void PropManager::addView(IPropView *view)
{
	auto iter = find(m_views.begin(), m_views.end(), view);
	if (iter == m_views.end())
	{
		m_views.push_back(view);
	}
}

void PropManager::removeView(IPropView *view)
{
	auto iter = find(m_views.begin(), m_views.end(), view);
	if (iter != m_views.end())
	{
		m_views.erase(iter);
	}
}