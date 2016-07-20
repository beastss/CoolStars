#include "SqliteHelper.h"
#include <unordered_map>
USING_NS_CC;
using namespace std;

SqliteHelper::SqliteHelper(const char* dbPath)
{
	string path;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	path = dbPath;
#else
	path += CCFileUtils::sharedFileUtils()->getWritablePath();
	path += dbPath;
#endif
	m_pGameDataBase = prepareTableInDB(path.c_str());
}

SqliteHelper::~SqliteHelper()
{
	closeDB();
}

vector<vector<char *>> SqliteHelper::readRecord(const char *sql_str)
{
	//对于大数据，有性能瓶颈
	vector<vector<char *>> record;
	char * errMsg = NULL;//错误信息
	int result;//sqlite3_exec返回值
	char **dbResult; //是 char ** 类型，两个*号
	int nRow, nColumn;

	result = sqlite3_get_table(m_pGameDataBase, sql_str, &dbResult, &nRow, &nColumn, &errMsg);
	if (SQLITE_OK == errMsg)
	{
		//row 从1开始 因为0的一行是列名
		for (int row = 1; row <= nRow; ++row)
		{
			auto begin = dbResult + nColumn * row;
			vector<char *> rowValues(begin, begin + nColumn);
			record.push_back(rowValues);
		}
		CCLOG("read table succeed");
	}
	else
	{
		CCLOG("read table failed");
	}

	return record;
}

sqlite3* SqliteHelper::prepareTableInDB(const char* dbFilename)
{
	sqlite3 *splite2Data = 0;
	char *errMsg = 0;

	if (SQLITE_OK != sqlite3_open(dbFilename, &splite2Data)) {
		CCLOG("Open the Game Data Table falied\n");
		return 0;
	}

	return splite2Data;
}

void SqliteHelper::createTable(const char *sql_str)
{
	//std::string string_sql = "create table if not exists " + std::string(table) + "(  id int unsigned auto_increment primary key,  name varchar(50),  age int,  sex varchar(10))";
	char *errMsg = 0;
	sqlite3_exec(m_pGameDataBase,
		sql_str,
		NULL,
		NULL,
		&errMsg); // create a table;  

	if (errMsg != 0) {
		CCLOG("Create the Data table failed\n");
	}
	{
		CCLOG("create table succeed");
	}
}

void SqliteHelper::executeSql(const char *sql_str)
{
	char *perrMsg = 0;
	int info = sqlite3_exec(m_pGameDataBase,
		sql_str,
		0,
		0,
		&perrMsg);
	do{
		if (perrMsg != 0) { // execute the instruct failed  
			CCLOG("insert a record into the table failed\n");
			CCLOG("reason: %d, Error Message: %s \n", info, perrMsg);
			break;
		}
		else
		{
			CCLOG("insert succeed");
		}

	} while (0);
}

void SqliteHelper::clearTable(const char *tabelName)
{
	string sql_str = "delete from ";
	sql_str += tabelName;
	sql_str += " ;";
	char *perrMsg = 0;
	int info = sqlite3_exec(m_pGameDataBase,
		sql_str.c_str(),
		0,
		0,
		&perrMsg);

	if (perrMsg != 0) {
		CCLOG("clear table data failed\n");
	}
	{
		CCLOG("clear table data succeed");
	}
}

void SqliteHelper::openDB(const char* dbPath)
{
	closeDB();
	m_pGameDataBase = prepareTableInDB(dbPath);
}


void SqliteHelper::closeDB()
{
	if (m_pGameDataBase)
	{
		sqlite3_close(m_pGameDataBase);
		m_pGameDataBase = NULL;
	}
}

void SqliteHelper::openTransaction(bool open)
{
	if (open)
	{
		sqlite3_exec(m_pGameDataBase, "begin;", 0, 0, 0);
	}
	else
	{
		sqlite3_exec(m_pGameDataBase, "commit;", 0, 0, 0);
	}
}

unordered_map<string, vector<string>> SqliteHelper::getDBInfo()
{
	vector<string> vecTable;
	GetTablesName(vecTable);

	unordered_map<string, vector<string>> info;
	for (auto iter = vecTable.begin(); iter != vecTable.end(); ++iter)
	{
		auto tableName = *iter;
		GetColName(info[tableName], tableName.c_str());
	}

	return info;
}


void SqliteHelper::GetTablesName(vector<string>& vecTable)
{
	char *szError = new char[256];
	sqlite3_stmt *stmt = NULL;
	sqlite3_prepare((sqlite3*)m_pGameDataBase, "select name,sql from sqlite_master where type='table'  order by name", -1, &stmt, 0);
	vector<string> vecTables;
	if (stmt)
	{
		while (sqlite3_step(stmt) == SQLITE_ROW)
		{
			auto tbName = (char *)sqlite3_column_text(stmt, 0);
			vecTable.push_back(tbName);
		}
		sqlite3_finalize(stmt);
		stmt = NULL;
	}
}

//获取列名
bool SqliteHelper::GetColName(vector<string>& vecColName, string strTableName)
{
	sqlite3_stmt *stmt = NULL;
	char sql[200];
	sprintf(sql, "SELECT * FROM %s limit 0,1", strTableName.c_str());
	char **pRes = NULL;
	int nRow = 0, nCol = 0;
	char *pErr = NULL;

	//第一行是列名称
	sqlite3_get_table((sqlite3*)m_pGameDataBase, sql, &pRes, &nRow, &nCol, &pErr);
	for (int i = 0; i < nRow; i++)
	{
		for (int j = 0; j < nCol; j++)
		{
			char *pv = *(pRes + nCol*i + j);
			vecColName.push_back(pv);
		}
		break;
	}

	if (pErr != NULL)
	{
		sqlite3_free(pErr);
	}
	sqlite3_free_table(pRes);
	return true;
}

/*
//使用回调函数获取表结果的方法
//方法一
sqlite3_exec(m_pGameDataBase,
read_sql_.c_str(),
readFromSQLite,
NULL,
&perrMsg);

int  readFromSQLite(void *anyParam, int iColumn, char **pColumnValue, char **pColumnName) {
int icolumenCount = iColumn;
for (int i = 0; i < icolumenCount; ++i) {
CCLOG("%s: %s \n", pColumnName[i], pColumnValue[i]);
}
return 0;
}


//方法二
char * errMsg = NULL;//错误信息
int result;//sqlite3_exec返回值
char **dbResult; //是 char ** 类型，两个*号
int nRow, nColumn;
int result = sqlite3_get_table(pDB, "select * from pet", &dbResult, &nRow, &nColumn, &errMsg);
*/

/*
//使用方法
SqliteHelper helper;
helper.readRecord("select * from stages");
helper.insertRecordIntoSqlite("insert into MyGame3 values(123, \"aaa\", 26, \"man\")");
string sql = "create table if not exists MyGame3 (id int unsigned auto_increment primary key, name varchar(50), age int, sex varchar(10))";
helper.createTable(sql.c_str());
return;
*/