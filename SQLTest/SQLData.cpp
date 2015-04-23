#include <winsock.h>
#include <stdio.h>
#include <string.h>
#include <mysql.h>
#include <iostream>
#include <fstream>
#include <string>
#pragma comment (lib, "libmysql.lib")
#pragma comment (lib, "mysqlclient.lib")
using namespace std;
MYSQL * myData;
void fix(string &s)
{
	string comma = "\'";
	int t, temp;
	t = (int)s.find(comma, 0);
	while (t != s.npos)
	{
		s.insert(t, comma);
		temp = t+2;
		t = (int)s.find(comma, temp);
	}
}
void FillData()
{
	string szSqlText, child, father;
	int count = 0;
	//sprintf(szSqlText, "CREATE TABLE CONCEPT(CHILD TEXT, FATHER TEXT)");
	ifstream fin("DATA.txt");
	while (fin >> child >> father)
	{
		count++;
		fix(child);
		fix(father);
		szSqlText = "INSERT INTO CONCEPT VALUES ('";
		szSqlText += child;
		szSqlText += "', '";
		szSqlText += father;
		szSqlText += "');";
		//cout << szSqlText << endl;
		//system("pause");
		if (mysql_query(myData, szSqlText.c_str()))
		{
			cout << "Insert Error at " << count << endl;
			cout << szSqlText << endl;
			cout << mysql_error(myData) << endl;
			break;
		}
	}
	//sprintf(szSqlText, "INSERT INTO CONCEPT VALUES ('g', 'r')");
	//sprintf(szSqlText, "CREATE TABLE Persons(Id_P int,LastName varchar(255),FirstName varchar(255),Address varchar(255),City varchar(255))");
	//sprintf(szSqlText, "DROP TABLE Persons");
}
int main()
{
	char szTargetDSN[] = "test";
	myData = mysql_init((MYSQL*)0);
	if (mysql_real_connect(myData, "127.0.0.1", "root", "2147483647", szTargetDSN, MYSQL_PORT, NULL, 0) == 0)
		cout << "Database Connection Failed!" << endl;
	else
	{
		//FillData()
	}
	mysql_close(myData);
	system("pause");
	return 0;
}
