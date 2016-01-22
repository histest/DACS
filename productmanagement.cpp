#include "productmanagement.h"
#include "connectsql.h"
#include "odbcexcel.h"
#include <QDesktopServices>
#include <QDate>
#include <QTimer>
#include "mydefine.h"
#include "selectchip.h"
#include "componentmanagement.h"
#include "progressbar.h"
#include <vector>  
extern bool Isfresh;
extern ConnectSql sql;

using namespace std;
vector <QString> vSat; 
int chosecount;
ProductManagement::ProductManagement(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	setStyleSheet("QPushButton{background-color: rgb(70,130,180);color: white;border-style: outset;border-color:rgb(27, 118, 150);border-width: 1px;border-radius: 1px;}"
		"QPushButton:pressed{background-color: rgb(50, 77, 92);color: white;}"
		"QPushButton:hover{	background-color: rgb(3,110,184);color: white;}"
		"QLineEdit{border-radius: 0px;}"
		"QHeaderView::section {background-color:white;color: black;padding-left: 4px;border: 1px solid #6c6c6c;};"
		"color: white;padding-left: 4px;border: 1px solid #6c6c6c;}"
		"QHeaderView::section:checked{background-color: white;color: black;}");
	ui.tableView_2->setStyleSheet("border: 1px solid gray;color: rgb(34, 125, 155);	background-color: transparent;selection-color: black;selection-background-color:rgb(34, 125, 155);");
	initUI();
	ComponentManagement*componentmanagement = new ComponentManagement;
	//connect(componentmanagement,SIGNAL(refresh(QString)), this, SLOT(initUI(QString)));

	namelistview = new QListView(this);
	model = new QStringListModel(this);
	namelistview->setWindowFlags(Qt::ToolTip);
	installEventFilter(namelistview);
	connect(namelistview, SIGNAL(clicked(const QModelIndex &)), this, SLOT(completeText(const QModelIndex &)));
	connect(ui.comboBox, SIGNAL(editTextChanged(const QString &)), this, SLOT(setCompleter(const QString &)));
}
void ProductManagement::initUI()
{
	search= new Advancedsearch;
	connect(search,SIGNAL(getsql(QString)), this, SLOT(advancedpreview(QString)));
	search->setWindowModality(Qt::WindowModal);
	this->ui.comboBox->clear();
	QSqlQuery query(*sql.db);
	query.exec("select*from SATELLITE_PUBLIC");
	QStringList list;
	bool Isexist =false;
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
			vSat.push_back(query.value(1).toString());
		}
		Isexist=false;
	}
	this->ui.comboBox->addItems(list);

	QSqlQueryModel *model=new QSqlQueryModel();
	ui.tableView_2->setModel(model);
	ui.tableView_2->horizontalHeader()->setSortIndicator(0, Qt::AscendingOrder);
}
void ProductManagement::refresh()
{
	this->ui.comboBox->clear();
	QSqlQuery query(*sql.db);
	query.exec("select*from SATELLITE_PUBLIC order by ID");
	QStringList list;
	while(query.next())
	{
		list.append(query.value(1).toString());

	}
	this->ui.comboBox->addItems(list);
	//if(Isfresh==true&&IsStop==false)
	//{

	//	

	//	QSqlQueryModel *model=new QSqlQueryModel();
	//	ui.tableView_2->setModel(model);
	//	IsStop=true;
	//}
}
void ProductManagement::DisableInput()
{
	ui.lineEdit->setEnabled(false);
	ui.inputButton->setEnabled(false);
	//ui.previewButton->setEnabled(false);
	ui.openButton->setEnabled(false);
}
QStringList CSVList; 
void ProductManagement::on_openButton_clicked()
{
	QString fileName = QFileDialog::getOpenFileName(this, tr("Excel file"), qApp->applicationDirPath (),
		tr("Excel Files (*.csv)"));
	if (fileName.isEmpty())
		return;
	//读取 csv
	QFile csvFile(fileName); 
	ui.lineEdit->setText(fileName);
	CSVList.clear();
	ui.tableWidget->setRowCount(0);
	if (csvFile.open(QIODevice::ReadWrite)) 
	{ 
		QTextStream stream(&csvFile); 
		while (!stream.atEnd()) 
		{ 
			CSVList.push_back(stream.readLine()); 
		} 
		csvFile.close(); 
	} 

	this->moveToThread(&visuaThread);
	//visuaThread.start();
	//QCoreApplication::processEvents();
	if (CSVList.count()==0)
	{
		return;
	}
	QStringList header;  
	header<<QString::fromLocal8Bit("时间")<<QString::fromLocal8Bit("星编号")<<QString::fromLocal8Bit("经度")<<QString::fromLocal8Bit("纬度")<<"digital_sun_x"<<"digital_sun_y"<<"digital_sun_z"<<"digital_sun_s";  //<<"Height"
	
	header.append(QString::fromLocal8Bit("累计复位次数"));
	header.append(QString::fromLocal8Bit("连续复位次数"));
	header.append(QString::fromLocal8Bit("复位原因"));	
	header.append(QString::fromLocal8Bit("单错累计次数"));	
	header.append(QString::fromLocal8Bit("发生错误的IO或RAM或ROM地址"));	
	header.append(QString::fromLocal8Bit("陷阱类型"));	
	header.append(QString::fromLocal8Bit("切机标志"));	
	header.append(QString::fromLocal8Bit("加电标志"));	
	header.append(QString::fromLocal8Bit("SRAM单错累计次数"));	
	header.append(QString::fromLocal8Bit("PROM单错累计次数"));	
	header.append(QString::fromLocal8Bit("双错累计次数"));	
	header.append(QString::fromLocal8Bit("SRAM双错累计次数"));	
	header.append(QString::fromLocal8Bit("PROM双错累计次数"));	
	header.append(QString::fromLocal8Bit("ROM状态标志"));	
	header.append(QString::fromLocal8Bit("RAM状态标志"));	
	header.append(QString::fromLocal8Bit("EEPROM状态标志"));	
	header.append(QString::fromLocal8Bit("发生错误的地址区域"));	
	header.append(QString::fromLocal8Bit("EEPROM错误地址"));	
	header.append(QString::fromLocal8Bit("EEPROM双错累计次数"));	
	header.append(QString::fromLocal8Bit("EEPROM单错累计次数"));	
	header.append(QString::fromLocal8Bit("EEPROM错误次数"));	
	header.append(QString::fromLocal8Bit("MRAM双错累计次数"));	
	header.append(QString::fromLocal8Bit("MRAM单错累计次数"));	
	header.append(QString::fromLocal8Bit("MRAM错误计数"));	
	header.append(QString::fromLocal8Bit("NOR_FLASH双错累计次数"));	
	header.append(QString::fromLocal8Bit("NOR_FLASH单错累计次数"));	
	header.append(QString::fromLocal8Bit("NOR_FLASH错误计数"));	
	header.append(QString::fromLocal8Bit("同步故障类型"));	

	//ui.tableWidget->setRowCount(CSVList.size()-1);
	ui.tableWidget->setColumnCount(36);  
	ui.tableWidget->setHorizontalHeaderLabels(header);  
//	ui.tableWidget->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
	int row=0;
	Q_FOREACH(QString str, CSVList) 
	{ 
		if (row!=0)
		{
			ui.tableWidget->insertRow(row-1);
			QStringList info = str.split(",");
			if (info.length()!=36)
			{
				QString str = str.fromLocal8Bit("提示");
				QString str2 = str.fromLocal8Bit("导入文件格式错误！");
				QMessageBox::information(this,str,str2);
				return;
			}
			//ui.tableWidget->setItem(row-1,0,new QTableWidgetItem(info[0]));  
			//ui.tableWidget->setItem(row-1,1,new QTableWidgetItem(info[1]));  
			//ui.tableWidget->setItem(row-1,2,new QTableWidgetItem(info[2]));  
			//ui.tableWidget->setItem(row-1,3,new QTableWidgetItem(info[3]));  
			//ui.tableWidget->setItem(row-1,4,new QTableWidgetItem(info[4]));  
			//ui.tableWidget->setItem(row-1,5,new QTableWidgetItem(info[5]));  
			//ui.tableWidget->setItem(row-1,6,new QTableWidgetItem(info[6]));  
			//ui.tableWidget->setItem(row-1,7,new QTableWidgetItem(info[7]));  
			//ui.tableWidget->setItem(row-1,8,new QTableWidgetItem(info[8]));  
			for (int i=0;i<info.length();i++)
			{
				ui.tableWidget->setItem(row-1,i,new QTableWidgetItem(info[i]));  
			}
			 QCoreApplication::processEvents();
		}
		row++;
	}
}
void ProductManagement::on_inputButton_clicked()
{
	//inputThread.exit();
	if (CSVList.count()==0)
	{
		return;
	}
	int i=0;
	QString satelliteNo ;
	QSqlQuery querysat(*sql.db);
	QSqlQuery querysource(*sql.db);
	int row =0;
	bool Isexist = false;
	Q_FOREACH(QString str, CSVList) 
	{ 
		if (row!=0)
		{
			QStringList info = str.split(",");
			satelliteNo = info[1];
			break;
		}
		row++;
	}
	querysat.exec("select*from SATELLITE_PUBLIC");
	while(querysat.next())
	{
		if (querysat.value(1).toString()==satelliteNo)
		{
			Isexist =true;
		}
	}
	if (!Isexist)
	{
		SelectChip*chip = new SelectChip;
		if (chip->chipCount==0)
		{
			QString str = str.fromLocal8Bit("提示");
			QString str2 = str.fromLocal8Bit("请先在芯片关联页面定义芯片信息！");
			QMessageBox::information(this,str,str2);
			return;
		}
		if(chip->exec()==QDialog::Accepted)
		{
			querysat.exec("select*from SATELLITE_PUBLIC");
			int satcount =0;
			while(querysat.next())
			{
				satcount++;
			}
			satcount++;
			for (int i=0;i<chip->chiplist.count();i++)
			{
				querysat.prepare("INSERT INTO SATELLITE_PUBLIC VALUES (?, ?, ?, ?)");
				querysat.bindValue(0,satcount);
				querysat.bindValue(1,satelliteNo);
				querysat.bindValue(2,500);
				querysat.bindValue(3,chip->chiplist.at(i));
				querysat.exec();
				satcount++;
			}
		}
		else
		{
			return;
		}
	}


	ProgressBar*progress = new ProgressBar();

	progress->ui.progressBar->setRange(0,CSVList.size());
	progress->setWindowFlags(Qt::FramelessWindowHint);
	progress->setModal(false);
	progress->show();
	progress->ui.label_2->setText(QString::fromLocal8Bit("检查数据库中是否已有该星数据"));
	Q_FOREACH(QString str, CSVList) 
	{ 
		if(i>0)
		{
			QStringList info = str.split(",");
			info[0].replace("/","-");
			QString strcurrentDateTime=info[0];
			if(strcurrentDateTime=="") continue;
			if(!strcurrentDateTime.contains(" ",Qt::CaseSensitive)) continue;
			QStringList info2=strcurrentDateTime.split(" ");
			QString strcurrentDate=info2[0];
			QString strcurrentTime=info2[1];
			if(!strcurrentDate.contains("-",Qt::CaseSensitive)) continue;
			QStringList info4=strcurrentDate.split("-");
			QString strYear = info4[0];
			QString strMonth = info4[1];
			if (strMonth.length()==1)
			{
				strMonth ="0"+strMonth;
			}

			QString strDay = info4[2];

			if (strDay.length()==1)
			{
				strDay="0"+strDay;
			}
			strcurrentDate = strYear+"-"+strMonth+"-"+strDay;


			info4=strcurrentTime.split(":");
			QString strHour = info4[0];
			QString strMinute = info4[1];
			if (strHour.length()==1)
			{
			 	strHour ="0"+strHour;
			}
			if (strMinute.length()==1)
			{
				strMinute ="0"+strMinute;
			}
			QString strSecond;
			if(info4.length()==2)
			   strSecond="00";
			if(info4.length()==3)
			   strSecond=info4[2];
			 
			if (strSecond.length()==1)
			{
			 	strSecond="0"+strSecond;
			}
			strcurrentTime = strHour+":"+strMinute+":"+strSecond;
			strcurrentDateTime = strcurrentDate+" "+strcurrentTime;
			QDateTime datetime;  
			datetime = QDateTime::fromString(strcurrentDateTime, "yyyy-MM-dd hh:mm:ss");

			QSqlQuery query(*sql.db);
			QString strsql;
			if (sql.dboracle)
			{
				strsql = "select*from SATELLITE where SATELLITENO  ='"+info[1]+"' and DATETIME = TIMESTAMP '"+strcurrentDateTime+"'";
			}
			else
			{
				strsql ="select*from SATELLITE where SATELLITENO  ='"+info[1]+"' and DATETIME  between #"+strcurrentDateTime+"# and #"+strcurrentDateTime+"#";

			}
			query.exec(strsql);

			if(query.next())
			{
				QString strtemp = QString::fromLocal8Bit("导入失败！该卫星数据在")+strcurrentDateTime+QString::fromLocal8Bit("已存在");
				QMessageBox::information(this,QString::fromLocal8Bit("警告"),strtemp);
				progress->close();
				return;
			}			
			else
			{
				break;
			}
		}
		i++;
		progress->ui.progressBar->setValue(i); 
	}
	this->moveToThread(&inputThread);
	progress->ui.progressBar->setValue(0); 
	progress->ui.label_2->setText(QString::fromLocal8Bit("正在导入"));

	querysat.exec("select*from SATELLITE ");
	QString ID;
	int totalcount =0;
	while(querysat.next())
	{
		totalcount++;
	}

	querysource.exec("select*from SOURCEDATA");
	int sourcecount =0;
	while(querysource.next())
	{
		sourcecount++;
	}

	QString time ;
	double longitude = 100;
	double latitude = 50;

	querysat.prepare("INSERT INTO SATELLITE "
		"VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");
	querysource.prepare("INSERT INTO SOURCEDATA "
		"VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");
	row=0;
	int deltacount=0;
	int lastcount=0;
	Q_FOREACH(QString str, CSVList) 
	{ 
		if (row!=0)
		{
			QCoreApplication::processEvents();
			QStringList info = str.split(",");
			querysat.bindValue(0, totalcount);
			querysource.bindValue(0, sourcecount);
			

			//QStringList info = str.split(",");
			info[0].replace("/","-");
			time =info[0];
			QString strcurrentDateTime=time;
			if(strcurrentDateTime=="") continue;
			QStringList info2=time.split(" ");
			QString strcurrentDate=info2[0];
			QString strcurrentTime=info2[1];

			QStringList info4=strcurrentDate.split("-");
			QString strYear = info4[0];
			QString strMonth = info4[1];
			if (strMonth.length()==1)
			{
				strMonth ="0"+strMonth;
			}

			QString strDay = info4[2];

			if (strDay.length()==1)
			{
				strDay="0"+strDay;
			}
			strcurrentDate = strYear+"-"+strMonth+"-"+strDay;


			info4=strcurrentTime.split(":");
			QString strHour = info4[0];
			QString strMinute = info4[1];
			if (strHour.length()==1)
			{
				strHour ="0"+strHour;
			}
			if (strMinute.length()==1)
			{
				strMinute ="0"+strMinute;
			}
			QString strSecond;
			if(info4.length()==2)
				strSecond="00";
			if(info4.length()==3)
				strSecond=info4[2];

			if (strSecond.length()==1)
			{
				strSecond="0"+strSecond;
			}
			strcurrentTime = strHour+":"+strMinute+":"+strSecond;
			strcurrentDateTime = strcurrentDate+" "+strcurrentTime;
			QDateTime datetime;  
			datetime = QDateTime::fromString(strcurrentDateTime, "yyyy-MM-dd hh:mm:ss");
		//	datetime = QDateTime::currentDateTime();
			querysat.bindValue(1, datetime);
			querysource.bindValue(1, datetime);

			if(row>1)
			{
				if(info[11].toInt()<lastcount &&  lastcount == 31)
				{
					deltacount=info[11].toInt()+32-lastcount;	
				}
				else if(info[11].toInt()<lastcount &&  lastcount != 31)
				{
					deltacount=lastcount-info[11].toInt();	
				}
				else
				{
					deltacount=info[11].toInt()-lastcount;	
				}
			}

			satelliteNo = info[1];
			querysat.bindValue(2, satelliteNo);
			querysource.bindValue(2, satelliteNo);

			longitude =info[2].toFloat();
			querysat.bindValue(3, longitude);
			querysource.bindValue(3, longitude);

			latitude = info[3].toFloat();
			querysat.bindValue(4, latitude);
			querysource.bindValue(4, latitude);

			//double height = info[4].toFloat();
			//querysat.bindValue(5, height);
			//querysource.bindValue(5, height);

			double sun_x =info[4].toFloat();
			querysat.bindValue(5, sun_x);
			querysource.bindValue(5, sun_x);

			double sun_y =info[5].toFloat();
			querysat.bindValue(6, sun_y);
			querysource.bindValue(6, sun_y);

			double sun_z =info[6].toFloat();
			querysat.bindValue(7, sun_z);
			querysource.bindValue(7, sun_z);

			double sun_s =info[7].toFloat();
			querysat.bindValue(8, sun_s);
			querysource.bindValue(8, sun_s);

			querysat.bindValue(9, info[8].toInt());
			querysource.bindValue(9, info[8].toInt());

			querysat.bindValue(10, info[9].toInt());
			querysource.bindValue(10, info[9].toInt());

			querysat.bindValue(11, info[10].toInt());
			querysource.bindValue(11, info[10].toInt());

			lastcount=info[11].toInt();			
			querysat.bindValue(12, deltacount);
			querysource.bindValue(12, lastcount);

			for (int i=12;i<36;i++)
			{
				querysat.bindValue(i+1, info[i].toInt());
				querysource.bindValue(i+1, info[i].toInt());
			}
			querysat.exec();
			querysource.exec();
			totalcount++;
			sourcecount++;
		}
		progress->ui.progressBar->setValue(row); 
		row++;
	}
	progress->close();

	QString str = str.fromLocal8Bit("提示");
	QString str2 = str.fromLocal8Bit("导入成功！");
	QMessageBox::information(this,str,str2);
	initUI();
}
void ProductManagement::on_previewButton_clicked()
{
	if (CSVList.count()==0)
	{
		return;
	}
	QStringList header;  

	header<<QString::fromLocal8Bit("时间")<<QString::fromLocal8Bit("单错次数")<<QString::fromLocal8Bit("经度")<<QString::fromLocal8Bit("纬度")<<"digital_sun_x"<<"digital_sun_y"<<"digital_sun_z"<<"digital_sun_s"<<QString::fromLocal8Bit("卫星编号");  //<<"Height"
	ui.tableWidget->setRowCount(CSVList.size()-1);
	ui.tableWidget->setColumnCount(9);  
	ui.tableWidget->setHorizontalHeaderLabels(header);  
	ui.tableWidget->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
	int row=0;
	Q_FOREACH(QString str, CSVList) 
	{ 
		if (row!=0)
		{
			QStringList info = str.split(",");
			ui.tableWidget->setItem(row-1,0,new QTableWidgetItem(info[0]));  
			ui.tableWidget->setItem(row-1,1,new QTableWidgetItem(info[1]));  
			ui.tableWidget->setItem(row-1,2,new QTableWidgetItem(info[2]));  
			ui.tableWidget->setItem(row-1,3,new QTableWidgetItem(info[3]));  
			ui.tableWidget->setItem(row-1,4,new QTableWidgetItem(info[4]));  
			ui.tableWidget->setItem(row-1,5,new QTableWidgetItem(info[5]));  
			ui.tableWidget->setItem(row-1,6,new QTableWidgetItem(info[6]));  
			ui.tableWidget->setItem(row-1,7,new QTableWidgetItem(info[7]));  
/*			ui.tableWidget->setItem(row-1,4,new QTableWidgetItem(info[4])); */ 
			ui.tableWidget->setItem(row-1,8,new QTableWidgetItem(info[8]));  
		}
		row++;
	}
}
void ProductManagement::on_outButton_clicked()
{
	QAbstractItemModel* model=ui.tableView_2->model();
	if (model->rowCount()==0||model==NULL)
	{
		QString str = str.fromLocal8Bit("提示");
		QString str2 = str.fromLocal8Bit("无数据");
		QMessageBox::information(this,str,str2);
		return;
	}

	QFileDialog dlg;
	dlg.setAcceptMode(QFileDialog::AcceptSave);
	//  Qt 5
	//dlg.setDirectory(QStandardPaths::writableLocation(QStandardPaths::DesktopLocation));
	//  Qt 4
	dlg.setDirectory(QDesktopServices::storageLocation(QDesktopServices::DesktopLocation));
	dlg.setNameFilter("*.xls");
	dlg.selectFile(QDate::currentDate().toString("yyyy-MM-dd.xls"));
	if(dlg.exec()!= QDialog::Accepted)
		return;
	QString filePath=dlg.selectedFiles()[0];
	if(OdbcExcel::saveFromTable(1,filePath,ui.tableView_2,"")) {
		QString str = str.fromLocal8Bit("提示");
		QString str2 = str.fromLocal8Bit("保存成功");
		QMessageBox::information(this,str,str2);
	}
	else{
		QString str = str.fromLocal8Bit("错误");
		QString msg=str.fromLocal8Bit("保存失败！\n\r")+OdbcExcel::getError();
		QMessageBox::critical(this,str,msg);
	}
}
void ProductManagement::on_queryButton_clicked()
{
	QSqlQueryModel *model=new QSqlQueryModel();
	ui.tableView_2->setModel(model);
}
void ProductManagement::on_previewButton_2_clicked()
{
	//QString startDate=ui.startdateEdit->date().toString("yyyy-MM-dd ");  
	//QString endDate=ui.enddateEdit->date().toString("yyyy-MM-dd ");  
	//QString str1=startDate;
	//QString str2=endDate;//+"%"
	QSqlQueryModel *model=new QSqlQueryModel();
	QString strsql;
	QString strSat =ui.comboBox->currentText();
	if (sql.dboracle==true)
	{
		//str2=endDate;//+"%"'
		strsql = "select * from SATELLITE where SATELLITENO = '"+strSat+"' order by ID";// where DATETIME between '"+str1+"' and '"+str2+"'andwhere to_char( DATETIME,  'YYYY-MM-DD') between  '"+str1+"'and  '"+str2+"'where DATETIME between '"+str1+"'and '"+str2+"'
	}
	else
	{
		strsql = "select * from SATELLITE where SATELLITENO = '"+strSat+"' order by ID";//where DATETIME between #"+str1+"# and #"+str2+"# and
	}
	model->setQuery(strsql,*sql.db);
	model->setHeaderData(0,Qt::Horizontal,tr("ID"));
	model->setHeaderData(1,Qt::Horizontal,QString::fromLocal8Bit("时间"));
	model->setHeaderData(2,Qt::Horizontal,QString::fromLocal8Bit("卫星编号"));
	model->setHeaderData(3,Qt::Horizontal,QString::fromLocal8Bit("经度"));
	model->setHeaderData(4,Qt::Horizontal,QString::fromLocal8Bit("纬度"));
/*	model->setHeaderData(5,Qt::Horizontal,tr("Height"));*/
	model->setHeaderData(5,Qt::Horizontal,tr("Sun_x"));
	model->setHeaderData(6,Qt::Horizontal,tr("Sun_y"));
	model->setHeaderData(7,Qt::Horizontal,tr("Sun_z"));
	model->setHeaderData(8,Qt::Horizontal,tr("Sun_s"));

	if (model)
	{
		QSortFilterProxyModel* proxy = new QSortFilterProxyModel(this);
		proxy->setSourceModel(model);
		ui.tableView_2->setModel(proxy);
	}
	ui.tableView_2->setSortingEnabled(true);
	ui.tableView_2->horizontalHeader()->setSortIndicator(0, Qt::AscendingOrder);
}
void ProductManagement::on_refreshButton_clicked()
{
	this->ui.comboBox->clear();
	QSqlQuery query(*sql.db);
	query.exec("select*from SATELLITE_PUBLIC");
	QStringList list;
	bool Isexist =false;
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
			vSat.push_back(query.value(1).toString());
		}
		Isexist=false;
	}
	this->ui.comboBox->addItems(list);
}
void ProductManagement::on_advancedsearchButton_clicked()
{

	if (chosecount>0)
	{
		search->show();
	}
	else
	{
		search->setFlag(1);
	}
	chosecount++;
}
void ProductManagement::advancedpreview(QString strsql)
{
	ProgressBar*progress = new ProgressBar();
	progress->setWindowFlags(Qt::FramelessWindowHint);
	progress->setModal(true);
	progress->show();
	progress->setFixedWidth(220);
	progress->ui.label_2->setText(QString::fromLocal8Bit("正从数据库获取数据，请等待..."));
	progress->ui.progressBar->setVisible(false);
	progress->ui.label->setVisible(false);
	QCoreApplication::processEvents();
	QSqlQueryModel *model=new QSqlQueryModel();
	QString strSat =ui.comboBox->currentText();
	model->setQuery(strsql,*sql.db);
	if (model)
	{
		QCoreApplication::processEvents();
		QSortFilterProxyModel* proxy = new QSortFilterProxyModel(this);
		proxy->setSourceModel(model);
		ui.tableView_2->setModel(proxy);
	}
	ui.tableView_2->setSortingEnabled(true);
	progress->hide();
}
void ProductManagement::setCompleter(const QString &text) {
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
	for(int i = 0;i < vSat.size();++i){

		if (vSat[i].indexOf(text) != -1)
		{

			list.append(vSat[i]);
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
	namelistview->setGeometry(this->x()+215, this->y()+200, 50, 100);
	//namelistview->resize(100,200);
	namelistview->setFixedWidth(ui.comboBox->width());
	namelistview->show();
	//	connect(ui.comboBox,SIGNAL(editTextChanged (const QString & )),this,SLOT(setCompleter(const QString & )));  
}
void ProductManagement::completeText(const QModelIndex &index) {
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
ProductManagement::~ProductManagement()
{

}

