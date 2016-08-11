#include "FailStateRecord.h"
#include "SqliteHelper.h"
USING_NS_CC;
using namespace std;

FailStateRecord::FailStateRecord()
{
	char str[100] = { 0 };
	SqliteHelper helper(DB_SAVING);
	string sql = "select * from save_cur_stage";
	auto result = helper.readRecord(sql.c_str());
	assert(result.size() == 1);

	auto data = result[0];
	m_curFailState = atoi(data[3]);
}

bool FailStateRecord::isFirstFail()
{
	return m_curFailState == kStateFirstFail;
}

void FailStateRecord::recordFailState(bool isWon)
{
	SqliteHelper sqlHelper(DB_SAVING);
	char str[100] = { 0 };
	
	if (isWon)
	{
		m_curFailState = kStateWon;
	}
	else if (m_curFailState == kStateFirstFail)
	{
		m_curFailState = kStateFailTimes;
	}

	sprintf(str, "update save_state set save_cur_stage = %d where id = 1;", m_curFailState);
	sqlHelper.executeSql(str);

}
