#include <mysql/mysql.h>
#include <stdio.h>
#include <string>
#include <string.h>
#include <stdlib.h>
#include <list>
#include <pthread.h>
#include <iostream>
#include "sql_connection.h"

using namespace std;

sql_connection::sql_connection()
{

}

MYSQL *GetConnection(){
	return con;
}

//构造初始化
void sql_connection::init(string url, string User, string PassWord, string DBName, int Port, unsigned int MaxConn)
{
	this->url = url;
	this->Port = Port;
	this->User = User;
	this->PassWord = PassWord;
	this->DatabaseName = DBName;

	con = mysql_init(con);

	if (con == NULL)
	{
		cout << "Error:" << mysql_error(con);
		exit(1);
	}
	con = mysql_real_connect(con, url.c_str(), User.c_str(), PassWord.c_str(), DBName.c_str(), Port, NULL, 0);

	if (con == NULL)
	{
		cout << "Error: " << mysql_error(con);
		exit(1);
	}

}


//销毁数据库连接池
void sql_connection::DestroyPool()
{
	mysql_close(con);
}


sql_connection::~sql_connection()
{
	DestroyPool();
}

