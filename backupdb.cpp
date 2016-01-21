#include "backupdb.h"
#include "connectsql.h"
#include <vector>  
using namespace std;
extern ConnectSql sql;
vector <QString> vectorSat; 
Backupdb::Backupdb(QWidget *parent)
	: QDialog(parent)
{

	ui.setupUi(this);
	QSqlQuery query(*sql.db);
	query.exec("select*from SATELLITE_PUBLIC order by ID");
	QStringList list,list2;
	QString lastChip;
	vectorSat.clear();
	bool Isexist=false;
	while(query.next())
	{
		for (int i=0;i<list.count();i++)
		{
			if (query.value(1).toString()==list.at(i))
				Isexist=true;
		}
		if(!Isexist)
		{
			list.append(query.value(1).toString());
			vectorSat.push_back(query.value(1).toString());
		}
		Isexist=false;
	}
	ui.comboBox->addItems(list);

	ui.comboBox_2->clear();
	list.clear();
	query.exec("select*from PARAMETER order by ID");

	while(query.next())
	{
		list.append(query.value(1).toString());
	}
	ui.comboBox_2->addItems(list);

	QString strfile;
	strfile = QCoreApplication::applicationDirPath();
	QString path = strfile+"/Backup.accdb"; 
	ui.lineEdit->setText(path);
	setStyleSheet("QHeaderView::section {background-color:white;color: black;padding-left: 4px;border: 1px solid #6c6c6c;};"
		"color: white;padding-left: 4px;border: 1px solid #6c6c6c;}"
		"QHeaderView::section:checked{background-color: white;color: black;}");	

	namelistview = new QListView(this);
	model = new QStringListModel(this);
	namelistview->setWindowFlags(Qt::ToolTip);
	installEventFilter(namelistview);
	connect(namelistview, SIGNAL(clicked(const QModelIndex &)), this, SLOT(completeText(const QModelIndex &)));
	connect(ui.comboBox, SIGNAL(editTextChanged(const QString &)), this, SLOT(setCompleter(const QString &)));

	QStringList*namelist = new QStringList;
	namelist->append(QString::fromLocal8Bit("累计复位次数"));
	namelist->append(QString::fromLocal8Bit("连续复位次数"));
	namelist->append(QString::fromLocal8Bit("复位原因"));	
	namelist->append(QString::fromLocal8Bit("单错次数"));	
	namelist->append(QString::fromLocal8Bit("发生错误的IO或RAM或ROM地址"));	
	namelist->append(QString::fromLocal8Bit("陷阱类型"));	
	namelist->append(QString::fromLocal8Bit("切机标志"));	
	namelist->append(QString::fromLocal8Bit("加电标志"));	
	namelist->append(QString::fromLocal8Bit("SRAM单错累计次数"));	
	namelist->append(QString::fromLocal8Bit("PROM单错累计次数"));	
	namelist->append(QString::fromLocal8Bit("双错累计次数"));	
	namelist->append(QString::fromLocal8Bit("SRAM双错累计次数"));	
	namelist->append(QString::fromLocal8Bit("PROM双错累计次数"));	
	namelist->append(QString::fromLocal8Bit("ROM状态标志"));	
	namelist->append(QString::fromLocal8Bit("RAM状态标志"));	
	namelist->append(QString::fromLocal8Bit("EEPROM状态标志"));	
	namelist->append(QString::fromLocal8Bit("发生错误的地址区域"));	
	namelist->append(QString::fromLocal8Bit("EEPROM错误地址"));	
	namelist->append(QString::fromLocal8Bit("EEPROM双错累计次数"));	
	namelist->append(QString::fromLocal8Bit("EEPROM单错累计次数"));	
	namelist->append(QString::fromLocal8Bit("EEPROM错误次数"));	
	namelist->append(QString::fromLocal8Bit("MRAM双错累计次数"));	
	namelist->append(QString::fromLocal8Bit("MRAM单错累计次数"));	
	namelist->append(QString::fromLocal8Bit("MRAM错误计数"));	
	namelist->append(QString::fromLocal8Bit("NOR_FLASH双错累计次数"));	
	namelist->append(QString::fromLocal8Bit("NOR_FLASH单错累计次数"));	
	namelist->append(QString::fromLocal8Bit("NOR_FLASH错误计数"));	
	namelist->append(QString::fromLocal8Bit("同步故障类型"));

	ui.comboBox_3->clear();
	query.exec("select*from DICTIONARY order by ID");
    list.clear();
	while(query.next())
	{
		for (int i=0;i<list.count();i++)
		{
			if (query.value(1).toString()==list.at(i))
				Isexist=true;
		}
		if(!Isexist)
			list.append(query.value(1).toString());
		//vtpara.push_back(query.value(1).toString());
		Isexist=false;
	}
	ui.comboBox_3->addItems(list);

	QDateTime now = QDateTime::currentDateTime();
	QString endtime = now.toString("yyyy-MM-dd hh:mm:ss");  
	ui.dateTimeEdit_2->setDateTime(now);

	QDateTime start = now.addYears(-1);
	QString starttime = start.toString("yyyy-MM-dd hh:mm:ss");  
	ui.dateTimeEdit->setDateTime(start);
}
void Backupdb::on_backupButton_clicked()
{
	QString todir =ui.lineEdit->text();
	QString sourcedir = QCoreApplication::applicationDirPath();
	sourcedir+="/DACS.accdb";  

	todir.replace("\\","/");  
	//QFile::copy(sourcedir, todir);
	if (sql.dboracle==true)
	{		
		oraclebackup();
	}
	else
	{
		
	}

//	QMessageBox::information(NULL, QString::fromLocal8Bit("提示"),QString::fromLocal8Bit("备份成功！"), QMessageBox::Yes);
}
void Backupdb::on_openButton_clicked()
{
	//QString fileName = QFileDialog::getSaveFileName(this, tr("Access file"), qApp->applicationDirPath (),
	//	tr("Access Files (*.accdb)"));
	//if (fileName.isEmpty())
	//	return;
	//ui.lineEdit->setText(fileName);

	QString fileName = QFileDialog::getOpenFileName(this, tr("Access file"), qApp->applicationDirPath (),
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
	query.exec("delete*from DICTIONARY");
	query.exec("delete*from SATELLITE_PUBLIC");
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
		return;
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

	query.exec("delete*from SATELLITE_PUBLIC");
	querysat.exec("select * from SATELLITE_PUBLIC where SATELLITENO = '"+strSat+"' ");
	while(querysat.next())
	{
		query.prepare("INSERT INTO SATELLITE_PUBLIC VALUES (?, ?, ?, ?)");
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


	QString strChipName = ui.comboBox_2->currentText();
	strsql="select*from PARAMETER where CHIPNAME = '"+strChipName+"' order by ID";
	if (ui.paracheckBox->isChecked())
		strsql =QString("select * from PARAMETER order by ID");
	querysat.exec(strsql);
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
	QString strname = ui.comboBox_3->currentText();
	strsql="select*from DICTIONARY where NAME = '"+strname+"' order by ID";
	if (ui.dictionarycheckBox->isChecked())
		strsql =QString("select * from DICTIONARY order by ID");
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
	if (ui.paracheckBox->isChecked())
		strsql =QString("select * from PARAMETER order by ID");
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
void Backupdb::on_previewButton_3_clicked()
{
	QString strName = ui.comboBox_3->currentText();
	QSqlQueryModel *model=new QSqlQueryModel();
	QString strsql =QString("select * from DICTIONARY where NAME = '"+strName+"' order by ID");//.arg(QString::fromLocal8Bit("名称"));
	if (ui.dictionarycheckBox->isChecked())
		strsql =QString("select * from DICTIONARY order by ID");
	model->setQuery(strsql,*sql.db);
	model->setHeaderData(0,Qt::Horizontal,tr("ID"));
	QString str = str.fromLocal8Bit("名称");
	model->setHeaderData(1,Qt::Horizontal,str);
	str = str.fromLocal8Bit("数值");
	model->setHeaderData(2,Qt::Horizontal,str);
	str = str.fromLocal8Bit("解释");
	model->setHeaderData(3,Qt::Horizontal,str);
	ui.tableView_3->setModel(model);
	ui.tableView_3->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
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
void Backupdb::on_allButton_2_clicked()
{
	QString strName = ui.comboBox_3->currentText();
	QSqlQueryModel *model=new QSqlQueryModel();
	QString strsql =QString("select * from DICTIONARY order by ID");//.arg(QString::fromLocal8Bit("名称"));
	model->setQuery(strsql,*sql.db);
	model->setHeaderData(0,Qt::Horizontal,tr("ID"));
	QString str = str.fromLocal8Bit("名称");
	model->setHeaderData(1,Qt::Horizontal,str);
	str = str.fromLocal8Bit("数值");
	model->setHeaderData(2,Qt::Horizontal,str);
	str = str.fromLocal8Bit("解释");
	model->setHeaderData(3,Qt::Horizontal,str);
	ui.tableView_3->setModel(model);
	ui.tableView_3->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
}
void Backupdb::setCompleter(const QString &text) {
	namelistview->hide();
	//disconnect(ui.comboBox,SIGNAL(editTextChanged (const QString & )),this,SLOT(setCompleter(const QString & )));  
	if (text.isEmpty()) {
		namelistview->hide();
		return;
	}

	if ((text.length() > 1) && (!namelistview->isHidden())) {
		return;
	}

	QSqlQuery query(*sql.db);	


	QString strsql= "select * from SATELLITE_PUBLIC where SATELLITENO like '"+text+"'";
	query.exec(strsql);
	if (query.next())return;

	QStringList list;
	model->setStringList(list);
	for(int i = 0;i < vectorSat.size();++i){

		if (vectorSat[i].indexOf(text) != -1)
		{

			list.append(vectorSat[i]);
		}
	}

	model->setStringList(list);
	//	model->setStringList(sl);
	namelistview->setModel(model);

	if (model->rowCount() == 0) {
		return;
	}

	// Position the text edit
	namelistview->setMinimumWidth(this->width());
	namelistview->setMaximumWidth(this->width());

	QPoint p(0, this->height());
	int x = this->mapToGlobal(p).x();
	int y = this->mapToGlobal(p).y() + 1;

	//listView->move(x, y);
	namelistview->setGeometry(this->x()+85, this->y()+95, 50, 100);
	namelistview->resize(100,200);
	namelistview->setFixedWidth(ui.comboBox->width());
	namelistview->show();
	//	connect(ui.comboBox,SIGNAL(editTextChanged (const QString & )),this,SLOT(setCompleter(const QString & )));  
}
void Backupdb::completeText(const QModelIndex &index) {
	QString strName = index.data().toString();
	for (int i =0;i<ui.comboBox->count();i++)
	{
		if (ui.comboBox->itemText(i)==strName)
		{
			ui.comboBox->setCurrentIndex(i);
		}
	}
	//ui.comboBox->setEditText(strName);
	namelistview->hide();
}

Backupdb::~Backupdb()
{
	//dbackup->close();
}
