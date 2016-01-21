#include "connectsql.h"
#include <QtGui/QApplication>
ConnectSql::ConnectSql()
{
	strtitle =QString::fromLocal8Bit("在轨数据收集与分析系统");
}

ConnectSql::~ConnectSql()
{
    QString DBname=db->connectionName();
   db->close();
   // QSqlDatabase::removeDatabase(DBname);

}

bool ConnectSql::connect(int databaseindex,QString strdb[])
{

	if(databaseindex==1)
	{
		db = new QSqlDatabase(QSqlDatabase::addDatabase("QOCI" ,"ORACLE" ));

		db->setHostName(strdb[0]);
		db->setUserName(strdb[1]);
		db->setPassword(strdb[2]);
		int port = atoi(strdb[3].toAscii());
		db->setPort(port);
		db->setDatabaseName(strdb[4]);
		if(!db->open())
		{
			QString str = str.fromLocal8Bit("提示");
			QString str2 = str.fromLocal8Bit("连接数据库失败！");
			QMessageBox::information(NULL, str,str2, QMessageBox::Ok);
			return false;
		}


		dboracle=true;
	}
	else
	{

// 		QString strfile;
// 		strfile = QCoreApplication::applicationDirPath();
// 		QString sDbNm = strfile+"/OracleBackup.accdb";  
// 		dbackup = QSqlDatabase::addDatabase("QODBC"); 
// 		QString dsn = QString("DRIVER={Microsoft Access Driver (*.mdb, *.accdb)}; FIL={MS Access};DBQ=%1;").arg(sDbNm);
// 
// 		dbackup.setDatabaseName(dsn);
// 		dbackup.setUserName(""); 
// 		dbackup.setPassword("111");  

		QString strfile;
		strfile = QCoreApplication::applicationDirPath();
		QString sDbNm = strfile+"/DACS.accdb";  
		db =new QSqlDatabase(QSqlDatabase::addDatabase("QODBC", "ODBC" )); 
		QString dsn = QString("DRIVER={Microsoft Access Driver (*.mdb, *.accdb)}; FIL={MS Access};DBQ=%1;").arg(strdb[0]);//连接字符串  
		
		db->setDatabaseName(dsn);
		db->setUserName(""); 
		db->setPassword("admin");  
		dboracle=false;
		if(!db->open())
		{
			QString str = str.fromLocal8Bit("提示");
			QString str2 = str.fromLocal8Bit("连接数据库失败！");
			QMessageBox::information(NULL, str,str2, QMessageBox::Ok);
			return false;
		}
	}
	


}
void ConnectSql::InputProduct()
{

}
void ConnectSql::AddProduct()
{

}
void ConnectSql::DeleteProduct()
{

}
void ConnectSql::InputOriginalData()
{

}
void ConnectSql::AddOriginalData()
{

}
void ConnectSql::DeleteOriginalData()
{

}
void ConnectSql::InputComponentData()
{

}
void ConnectSql::AddComponentData()
{

}
void ConnectSql::DeleteComponentData()
{

}