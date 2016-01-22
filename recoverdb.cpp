#include "recoverdb.h"
#include "connectsql.h"
extern ConnectSql sql;
Recoverdb::Recoverdb(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
}
void Recoverdb::on_recoverButton_clicked()
{
	if (ui.lineEdit->text()=="")
	{
		QString str = str.fromLocal8Bit("提示");
		QString str2 = str.fromLocal8Bit("请选择本地源数据库！");
		QMessageBox::information(NULL, str,str2, QMessageBox::Ok);
		return;
	}
	if (sql.dboracle==true)
	{
		QString sDbNm =ui.lineEdit->text();
		sDbNm.replace("\\","/");  
		dbrecover =new QSqlDatabase(QSqlDatabase::addDatabase("QODBC", "ODBC" )); 
		QString dsn = QString("DRIVER={Microsoft Access Driver (*.mdb, *.accdb)}; FIL={MS Access};DBQ=%1;").arg(sDbNm);//连接字符串  

		dbrecover->setDatabaseName(dsn);
		dbrecover->setUserName(""); 
		dbrecover->setPassword("admin");  

		if(!dbrecover->open())
		{
			QString str = str.fromLocal8Bit("提示");
			QString str2 = str.fromLocal8Bit("连接数据库失败！");
			QMessageBox::information(NULL, str,str2, QMessageBox::Ok);
			return ;
		}

		QSqlQuery query(*sql.db);
		query.exec("delete from SATELLITE ");
		if(!query.exec())
		{
			QMessageBox::information(NULL, QString::fromLocal8Bit("提示"),QString::fromLocal8Bit("删除失败！"), QMessageBox::Ok);
			return;
		}
		query.exec("delete from SOURCEDATA");	
		query.exec("delete from PARAMETER");
		query.exec("delete from USERS");
		query.exec("delete from DICTIONARY");
		query.exec("delete from SATELLITE_PUBLIC");

		QSqlQuery querysat(*dbrecover);
		bool bquery=querysat.exec("select*from SATELLITE  order by ID");
		QString strTime;
		if (!bquery)
		{
			QMessageBox::information(NULL, QString::fromLocal8Bit("提示"),QString::fromLocal8Bit("失败！"), QMessageBox::Ok);
		}
		while(querysat.next())
		{
			query.prepare("INSERT INTO SATELLITE "
				"VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");
			for (int i=0;i<37;i++)
			{
				query.bindValue(i, querysat.value(i));
			}
			/*query.prepare("INSERT INTO SATELLITE "
			"VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");

			for (int i=0;i<10;i++)
			{
			if(i==1)
			{
			strTime=querysat.value(i).toString();
			strTime.replace("T"," ");  
			query.bindValue(i, strTime);
			}
			else
			{
			query.bindValue(i, querysat.value(i).toString());
			}

			}*/
			if(!query.exec())
			{
				QMessageBox::information(NULL, QString::fromLocal8Bit("提示"),QString::fromLocal8Bit("恢复失败！"), QMessageBox::Ok);
				return;
			}
		}

		querysat.exec("select * from SATELLITE_PUBLIC ");
		while(querysat.next())
		{
			query.prepare("INSERT INTO SATELLITE_PUBLIC VALUES (?, ?, ?, ?)");
			for (int i=0;i<4;i++)
			{
				query.bindValue(i, querysat.value(i));
			}
			if(!query.exec())
			{
				QMessageBox::information(NULL, QString::fromLocal8Bit("提示"),QString::fromLocal8Bit("恢复失败！"), QMessageBox::Ok);
				return;
			}
		}

		querysat.exec("select*from SOURCEDATA  order by ID");
		while(querysat.next())
		{

			query.prepare("INSERT INTO SOURCEDATA "
				"VALUES(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");
			for (int i=0;i<37;i++)
			{
				query.bindValue(i, querysat.value(i));
			}
			/*query.prepare("INSERT INTO SOURCEDATA "
			"VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");
			for (int i=0;i<10;i++)
			{
			if(i==1)
			{
			strTime=querysat.value(i).toString();
			strTime.replace("T"," ");  
			query.bindValue(i, strTime);
			}
			else
			{
			query.bindValue(i, querysat.value(i).toString());
			}
			}*/
			if(!query.exec())
			{
				QMessageBox::information(NULL, QString::fromLocal8Bit("提示"),QString::fromLocal8Bit("恢复失败！"), QMessageBox::Ok);
				return;
			}
		}
		querysat.exec("select*from PARAMETER  order by ID");
		while(querysat.next())
		{

			query.prepare("INSERT INTO PARAMETER "
				"VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");
			for (int i=0;i<10;i++)
			{
				query.bindValue(i, querysat.value(i));
			}
			if(!query.exec())
			{
				QMessageBox::information(NULL, QString::fromLocal8Bit("提示"),QString::fromLocal8Bit("恢复失败！"), QMessageBox::Ok);
				return;
			}
		}

		querysat.exec("select*from USERS  order by ID");
		while(querysat.next())
		{

			query.prepare("INSERT INTO USERS "
				"VALUES (?, ?, ?, ?)");
			for (int i=0;i<4;i++)
			{
				query.bindValue(i, querysat.value(i));
			}
			if(!query.exec())
			{
				QMessageBox::information(NULL, QString::fromLocal8Bit("提示"),QString::fromLocal8Bit("恢复失败！"), QMessageBox::Ok);
				return;
			}
		}
		QMessageBox::information(NULL, QString::fromLocal8Bit("提示"),QString::fromLocal8Bit("恢复成功！"), QMessageBox::Ok);

		QString  strsql=QString("select * from DICTIONARY order by ID");
		querysat.exec(strsql);
		while(querysat.next())
		{

			query.prepare("INSERT INTO DICTIONARY "
				"VALUES (?, ?, ?, ?)");
			for (int i=0;i<4;i++)
			{
				query.bindValue(i, querysat.value(i));
			}
			if(!query.exec())
			{
				QMessageBox::information(NULL, QString::fromLocal8Bit("提示"),QString::fromLocal8Bit("恢复失败！"), QMessageBox::Ok);
				return;
			}
		}
	}
	else
	{
		QString sourcedir =ui.lineEdit->text();
		QString todir = QCoreApplication::applicationDirPath();
		todir+="/DACS.accdb";  
		QFile::copy(sourcedir, todir);
	}

}
void Recoverdb::on_openButton_clicked()
{
	QString fileName = QFileDialog::getOpenFileName(this, tr("Access file"), qApp->applicationDirPath (),
		tr("Access Files (*.accdb)"));
	if (fileName.isEmpty())
		return;
	ui.lineEdit->setText(fileName);
}
void Recoverdb::on_exitButton_clicked()
{
	this->close();
};
Recoverdb::~Recoverdb()
{
	//dbrecover->close();
}
