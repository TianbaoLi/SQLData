#include <winsock.h>
#include <stdio.h>
#include <string.h>
#include <mysql.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <ctime>
#include <io.h>
#include <direct.h>
#pragma comment (lib, "libmysql.lib")
#pragma comment (lib, "mysqlclient.lib")
using namespace std;
MYSQL * myData;
void CreateTable(string tableName)
{
	string szSqlText;
	szSqlText = "CREATE TABLE ";
	szSqlText += tableName;
	szSqlText += "(ID INT, CHILD TEXT, FATHER TEXT)";
	//sprintf(szSqlText, "CREATE TABLE CONCEPT(CHILD TEXT, FATHER TEXT)");
	if (mysql_query(myData, szSqlText.c_str()))
	{
		cout << "Create Table Error "<< endl;
		cout << szSqlText << endl;
		cout << mysql_error(myData) << endl;
	}
}
void Fix(string &s)
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
void FillData(string tableName)
{
	string szSqlText, child, father;
	int count = 0;
	stringstream ss;
	string id;
	ifstream fin("DATA.txt");
	CreateTable(tableName);
	while (fin >> child >> father)
	{
		count++;
		ss.str("");
		Fix(child);
		Fix(father);
		szSqlText = "INSERT INTO ";
		szSqlText += tableName;
		szSqlText+=" VALUES ('";
		ss << count;
		id = ss.str();
		szSqlText += id;
		szSqlText += "', '";
		szSqlText += child;
		szSqlText += "', '";
		szSqlText += father;
		szSqlText += "');";

		//cout << szSqlText << endl;

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
#define BigPrime 2147483647
#define PathLength 100
#define NameLength 500
#define SchemaLength 500
char FolderName[PathLength];
int Seed, Sol;
int Hashmap(const char *x)
{
	Seed = 13;
	Sol = 0;
	while (*x)
		Sol = (Sol*Seed + (*x++)) % BigPrime;
	if (Sol <= 0)
		Sol += BigPrime;
	return Sol%BigPrime;
}
int HashAnswer;
char Name[NameLength], ScheCompare[NameLength];
bool IndexMatched, NoMatched;
FILE *CompareIn, *HashNumberOut;
bool Hash(const char *x, char *Path)
{
	HashAnswer = Hashmap(x);
	IndexMatched = 0;
	NoMatched = 0;
	memset(Path, 0, sizeof(Path));
	memset(Name, 0, sizeof(Name));
	memset(ScheCompare, 0, sizeof(ScheCompare));
	_itoa(HashAnswer, Name, 10);
	strcpy(Path, FolderName);
	strcat(Path, Name);
	strcat(Path, ".dat");
	while (_access(Path, 0) == 0)
	{
		CompareIn = fopen(Path, "r");
		fscanf(CompareIn, "%s", &ScheCompare);
		fclose(CompareIn);
		if (strcmp(ScheCompare, x) == 0)
			return true;
		memset(Path, 0, sizeof(Path));
		strcpy(Path, FolderName);
		HashAnswer++;
		if (HashAnswer == BigPrime - 1)
			HashAnswer = 0;
		_itoa(HashAnswer, Name, 10);
		strcat(Path, Name);
		strcat(Path, ".dat");
	}
	return false;
}
char szSqlText[200];
MYSQL_RES *result;
MYSQL_FIELD *fd;
MYSQL_ROW sql_row;
char column[500][500];
char Path[PathLength];
FILE *SchemaOut;
int i, j;
bool SqlFindFather(const char* tableName, const char* fatherName)
{
	memset(Path, 0, sizeof(Path));
	if (Hash(fatherName, Path) == true)
		return false;
	memset(szSqlText, 0, sizeof(szSqlText));
	strcpy(szSqlText, "SELECT * FROM ");
	strcat(szSqlText, tableName);
	strcat(szSqlText, " WHERE father = '");
	strcat(szSqlText, fatherName);
	strcat(szSqlText, "'");
	if (mysql_query(myData, szSqlText))
	{
		cout << "Error at " << endl;
		cout << szSqlText << endl;
		cout << mysql_error(myData) << endl;
		//system("pause");
		return false;
	}
	result = mysql_store_result(myData);
	if (result)
	{
		cout << "Schema: " << fatherName << "\t";
		cout << "number of result: " << (unsigned long)mysql_num_rows(result) << "\t";
		memset(column, 0, sizeof(column));
		for (i = 0; fd = mysql_fetch_field(result); i++)
			strcpy(column[i], fd->name);
		j = mysql_num_fields(result);
		/*for (int i = 0; i<j; i++)
			printf("%s\t", column[i]);
		printf("\n");*/
		//printf("%s\n", Path);
		SchemaOut = fopen(Path, "w");
		fprintf(SchemaOut, "%s\n", fatherName);
		while (sql_row = mysql_fetch_row(result))
		{
			for (i = 0; i < j; i++)
				fprintf(SchemaOut, "%s\t", sql_row[i]);
			fprintf(SchemaOut, "\n");
		}
		fclose(SchemaOut);
	}
	mysql_free_result(result);
	return true;
}
int main()
{
	memset(FolderName, 0, sizeof(FolderName));
	strcat(FolderName, "HashList\\");
	_mkdir(FolderName);
	char szTargetDSN[] = "test";
	myData = mysql_init((MYSQL*)0);
	int count = 0;
	char child[SchemaLength], father[SchemaLength];
	FILE *knowledgeIn = fopen("DATA.txt", "r");
	clock_t start, end;
	if (mysql_real_connect(myData, "127.0.0.1", "root", "2147483647", szTargetDSN, MYSQL_PORT, NULL, 0) == 0)
		cout << "Database Connection Failed!" << endl;
	else
	{
		//FillData("CONCEPT1");
		while (EOF)
		{
			count++;
			if (count <6500000)
			{
				continue;
			}
			fscanf(knowledgeIn, "%s %s", &child, &father);
			start = clock();
			cout << "No." << count << ": " << father << ends;
			SqlFindFather("CONCEPT1", father);
			end = clock();
			cout << "time cost: " << ((double)end - start) / 1000 << "s" << endl;
			//Sleep(10);
			//system("pause");
		}
	}
	mysql_close(myData);
	fclose(knowledgeIn);
	system("pause");
	return 0;
}
