#include "backupdb.h"
#include "connectsql.h"
extern ConnectSql sql;
Backupdb::Backupdb(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	QSqlQuery query(*sql.db);
	query.exec("select*from PARAMETER order by ID");
	QStringList list,list2;
	QString lastChip;
	while(query.next())
	{
		list.append(query.value(1).toString());

	}
	this->ui.comboBox->addItems(list);

	ui.comboBox_2->clear();
	list.clear();
	query.exec("select*from PARAMETER order by ID");
	while(query.next())
	{
		if(lastChip!=query.value(3).toString())
			list.append(query.value(3).toString());
		lastChip=query.value(3).toString();

	}
	ui.comboBox_2->addItems(list);

	QString strfile;
	strfile = QCoreApplication::applicationDirPath();
	QString path = strfile+"/OracleBackup.accdb"; 
	ui.lineEdit->setText(path);
	setStyleSheet("QHeaderView::section {background-color:white;color: black;padding-left: 4px;border: 1px solid #6c6c6c;};"
		"color: white;padding-left: 4px;border: 1px solid #6c6c6c;}"
		"QHeaderView::section:checked{background-color: white;color: black;}");	

}
void Backupdb::on_backupButton_clicked()
{
	QString todir =ui.lineEdit->text();
	QString sourcedir = QCoreApplication::applicationDirPath();
	sourcedir+="/DACS.accdb";  

	todir.replace("\\","/");  
	QFile::copy(sourcedir, todir);
	if (sql.dboracle==true)
	{		
		oraclebackup();
	}
	else
	{

		QMessageBox::information(NULL, QString::fromLocal8Bit("提示"),QString::fromLocal8Bit("备份成功！"), QMessageBox::Yes);
	}

}
void Backupdb::on_openButton_clicked()
{
	QString fileName = QFileDialog::getSaveFileName(this, tr("Access file"), qApp->applicationDirPath (),
		tr("Access Files (*.accdb)"));
	if (fileName.isEmpty())
		return;
	ui.lineEdit->setText(fileName);
}
void Backupdb::connectDB()
{
// 	QString sDbNm =ui.lineEdit->text();
// 	sDbNm.replace("\\","/");  
// 
// 	dbackup	 = QSqlDatabase::addDatabase("QODBC"); 
// 	QString dsn = QString("DRIVER={Microsoft Access Driver (*.mdb, *.accdb)}; FIL={MS Access};DBQ=%1;").arg(sDbNm);
// 
// 	dbackup.setDatabaseName(dsn);
// 	dbackup.setUserName(""); 
// 	dbackup.setPassword("111");  
// 	if(!dbackup.open())
// 	{
// 		QString str = str.fromLocal8Bit("提示");
// 		QString str2 = str.fromLocal8Bit("连接数据库失败！");
// 		QMessageBox::information(NULL, str,str2, QMessageBox::Ok);
// 		return ;
// 	}
// 	QSqlQuery querysat(*sql.db);
// 	bool bquery=querysat.exec("select*from SATELLITE");
// 	if (!bquery)
// 	{
// 		QMessageBox::information(NULL, QString::fromLocal8Bit("提示"),QString::fromLocal8Bit("失败！"), QMessageBox::No);
// 	}
// 	while(querysat.next())
// 	{
// 		QSqlQuery query(dbackup);
// 		query.prepare("INSERT INTO SATELLITE "
// 			"VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");
// 		for (int i=0;i<10;i++)
// 		{
// 			query.bindValue(i, querysat.value(i));
// 		}
// 		query.exec();
// 	}
}
void Backupdb::oraclebackup()
{
	QString sDbNm =ui.lineEdit->text();
	sDbNm.replace("\\","/");  
	dbackup =new QSqlDatabase(QSqlDatabase::addDatabase("QODBC", "ODBC" )); 
	QString dsn = QString("DRIVER={Microsoft Access Driver (*.mdb, *.accdb)}; FIL={MS Access};DBQ=%1;").arg(sDbNm);//连接字符串  

	dbackup->setDatabaseName(dsn);
	dbackup->setUserName(""); 
	dbackup->setPassword("admin");  

	if(!dbackup->open())
	{
		QString str = str.fromLocal8Bit("提示");
		QString str2 = str.fromLocal8Bit("连接数据库失败！");
		QMessageBox::information(NULL, str,str2, QMessageBox::Ok);
		return ;
	}
	QSqlQuery query(*dbackup);
	query.exec("delete*from SATELLITE");
	query.exec("delete*from SOURCEDATA");	
	query.exec("delete*from PARAMETER");
	query.exec("delete*from USERS");

	QString strSat =ui.comboBox->currentText();

	QString startDate=ui.dateTimeEdit->dateTime().toString("yyyy-MM-dd hh:mm:ss");  
	QString endDate=ui.dateTimeEdit_2->dateTime().toString("yyyy-MM-dd hh:mm:ss");  
	QString strsql;
	if (sql.dboracle==true)
	{
		strsql = "select * from SATELLITE where SATELLITENO = '"+strSat+"' and DATETIME between TIMESTAMP '"+startDate+"' and TIMESTAMP '"+endDate+"' order by ID";
	}
	else
	{
		strsql = "select * from SATELLITE where SATELLITENO = '"+strSat+"' and DATETIME between #"+startDate+"# and #"+endDate+"# order by ID";//and
	}

	QSqlQuery querysat(*sql.db);
	bool bquery=querysat.exec(strsql);
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
		if(!query.exec())
		{
			QMessageBox::information(NULL, QString::fromLocal8Bit("提示"),QString::fromLocal8Bit("备份失败！"), QMessageBox::Ok);
			return;
		}
	}

	if (sql.dboracle==true)
	{
		strsql = "select * from SOURCEDATA where SATELLITENO = '"+strSat+"' and DATETIME between TIMESTAMP '"+startDate+"' and TIMESTAMP '"+endDate+"' order by ID";
	}
	else
	{
		strsql = "select * from SOURCEDATA where SATELLITENO = '"+strSat+"' and DATETIME between #"+startDate+"# and #"+endDate+"# order by ID";//and
	}
	querysat.exec(strsql);
	while(querysat.next())
	{

		query.prepare("INSERT INTO SOURCEDATA "
			"VALUES(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");
		for (int i=0;i<37;i++)
		{
			query.bindValue(i, querysat.value(i));
		}
		if(!query.exec())
		{
			QMessageBox::information(NULL, QString::fromLocal8Bit("提示"),QString::fromLocal8Bit("备份失败！"), QMessageBox::Ok);
			return;
		}
	}
	QString strChipName = ui.comboBox_2->currentText();
	querysat.exec("select*from PARAMETER where CHIPNAME = '"+strChipName+"' order by ID");
	while(querysat.next())
	{

		query.prepare("INSERT INTO PARAMETER "
			"VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?)");
		for (int i=0;i<9;i++)
		{
			query.bindValue(i, querysat.value(i));
		}
		if(!query.exec())
		{
			QMessageBox::information(NULL, QString::fromLocal8Bit("提示"),QString::fromLocal8Bit("备份失败！"), QMessageBox::Ok);
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
			QMessageBox::information(NULL, QString::fromLocal8Bit("提示"),QString::fromLocal8Bit("备份失败！"), QMessageBox::Ok);
			return;
		}
	}
	QMessageBox::information(NULL, QString::fromLocal8Bit("提示"),QString::fromLocal8Bit("备份成功！"), QMessageBox::Ok);
}
void Backupdb::on_previewButton_clicked()
{
	QSqlQueryModel *model=new QSqlQueryModel();
	QString strsql;
	QString strSat =ui.comboBox->currentText();
	QString startDate=ui.dateTimeEdit->dateTime().toString("yyyy-MM-dd hh:mm:ss");  
	QString endDate=ui.dateTimeEdit_2->dateTime().toString("yyyy-MM-dd hh:mm:ss");  

	if (sql.dboracle==true)
	{
		//str2=endDate;//+"%"'
		strsql = "select * from SATELLITE where SATELLITENO = '"+strSat+"' and DATETIME between TIMESTAMP '"+startDate+"' and TIMESTAMP '"+endDate+"' order by ID";
	}
	else
	{
		strsql = "select * from SATELLITE where SATELLITENO = '"+strSat+"' and DATETIME between #"+startDate+"# and #"+endDate+"# order by ID";//and
	}
	model->setQuery(strsql,*sql.db);
	model->setHeaderData(0,Qt::Horizontal,tr("ID"));
	model->setHeaderData(1,Qt::Horizontal,QString::fromLocal8Bit("时间"));
	model->setHeaderData(2,Qt::Horizontal,QString::fromLocal8Bit("单错次数"));
	model->setHeaderData(3,Qt::Horizontal,QString::fromLocal8Bit("经度"));
	model->setHeaderData(4,Qt::Horizontal,QString::fromLocal8Bit("纬度"));
	/*	model->setHeaderData(5,Qt::Horizontal,tr("Height"));*/
	model->setHeaderData(5,Qt::Horizontal,tr("Sun_x"));
	model->setHeaderData(6,Qt::Horizontal,tr("Sun_y"));
	model->setHeaderData(7,Qt::Horizontal,tr("Sun_z"));
	model->setHeaderData(8,Qt::Horizontal,tr("Sun_s"));
	model->setHeaderData(9,Qt::Horizontal,QString::fromLocal8Bit("卫星编号"));
	ui.tableView->setModel(model);
//	ui.tableView->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
	ui.tableView->horizontalHeader()->resizeSection(1,160);
}
void Backupdb::on_previewButton_2_clicked()
{
	QString strChipName = ui.comboBox_2->currentText();
	QSqlQueryModel *model=new QSqlQueryModel();
	QString strsql = "select * from PARAMETER where CHIPNAME = '"+strChipName+"' order by ID";
	model->setQuery(strsql,*sql.db);
	model->setHeaderData(0,Qt::Horizontal,tr("ID"));
	QString str = str.fromLocal8Bit("卫星编号");
	model->setHeaderData(1,Qt::Horizontal,str);
	str = str.fromLocal8Bit("轨道高度");
	model->setHeaderData(2,Qt::Horizontal,str);
	str = str.fromLocal8Bit("芯片名称");
	model->setHeaderData(3,Qt::Horizontal,str);
	str = str.fromLocal8Bit("规格");
	model->setHeaderData(4,Qt::Horizontal,str);
	str = str.fromLocal8Bit("封装");
	model->setHeaderData(5,Qt::Horizontal,str);
	str = str.fromLocal8Bit("额定电压");
	model->setHeaderData(6,Qt::Horizontal,str);
	str = str.fromLocal8Bit("额定电流");
	model->setHeaderData(7,Qt::Horizontal,str);
	str = str.fromLocal8Bit("抗辐射指标");
	model->setHeaderData(8,Qt::Horizontal,str);
	ui.tableView_2->setModel(model);
	ui.tableView_2->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
}
void Backupdb::on_allButton_clicked()
{
	QString strChipName = ui.comboBox_2->currentText();
	QSqlQueryModel *model=new QSqlQueryModel();
	QString strsql = "select * from PARAMETER order by ID";
	model->setQuery(strsql,*sql.db);
	model->setHeaderData(0,Qt::Horizontal,tr("ID"));
	QString str = str.fromLocal8Bit("卫星编号");
	model->setHeaderData(1,Qt::Horizontal,str);
	str = str.fromLocal8Bit("轨道高度");
	model->setHeaderData(2,Qt::Horizontal,str);
	str = str.fromLocal8Bit("芯片名称");
	model->setHeaderData(3,Qt::Horizontal,str);
	str = str.fromLocal8Bit("规格");
	model->setHeaderData(4,Qt::Horizontal,str);
	str = str.fromLocal8Bit("封装");
	model->setHeaderData(5,Qt::Horizontal,str);
	str = str.fromLocal8Bit("额定电压");
	model->setHeaderData(6,Qt::Horizontal,str);
	str = str.fromLocal8Bit("额定电流");
	model->setHeaderData(7,Qt::Horizontal,str);
	str = str.fromLocal8Bit("抗辐射指标");
	model->setHeaderData(8,Qt::Horizontal,str);
	ui.tableView_2->setModel(model);
	ui.tableView_2->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
}
Backupdb::~Backupdb()
{
	//dbackup->close();
}
