#ifndef _CONNECTION_POOL_
#define _CONNECTION_POOL_

#include <stdio.h>
#include <list>
#include <mysql/mysql.h>
#include <error.h>
#include <string.h>
#include <iostream>
#include <string>

using namespace std;

class sql_connection
{
public:
	MYSQL *GetConnection();				 //获取数据库连接
	void DestroyPool();					 //销毁连接
 

	void init(string url, string User, string PassWord, string DataBaseName, int Port, unsigned int MaxConn); 
	
	sql_connection();
	~sql_connection();


private:
	MYSQL *con; //连接

private:
	string url;			 //主机地址
	string Port;		 //数据库端口号
	string User;		 //登陆数据库用户名
	string PassWord;	 //登陆数据库密码
	string DatabaseName; //使用数据库名
};

#endif
