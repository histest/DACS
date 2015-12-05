#ifndef CONNECTSQL_H
#define CONNECTSQL_H

#include<QSqlError>
#include<QSqlDatabase>
#include<QSqlQuery>
#include<QSqlQueryModel>
#include<QSqlTableModel>
#include<QSqlResult>
#include<QSqlRecord>

#include<QMessageBox>
#include<QDebug>
class ConnectSql
{
public:
    ConnectSql();
    QSqlDatabase *db;
	
    ~ConnectSql();
    bool connect(int databaseindex,QString strdb []);
	void InputProduct();
	void AddProduct();
	void DeleteProduct();
	void InputOriginalData();
	void AddOriginalData();
	void DeleteOriginalData();
	void InputComponentData();
	void AddComponentData();
	void DeleteComponentData();
	bool dboracle;
	QString struser;
	QString strtitle;
};

#endif // CONNECTSQL_H
