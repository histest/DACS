#include "componentmanagement.h"
#include "connectsql.h"
#include <QDesktopServices>
#include "mydefine.h"
extern bool Isfresh;
extern ConnectSql sql;
ComponentManagement::ComponentManagement(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	initUI();
	setStyleSheet("QPushButton{background-color: rgb(70,130,180);color: white;border-style: outset;border-color:rgb(27, 118, 150);border-width: 1px;border-radius: 0px;}"
		"QPushButton:pressed{background-color: rgb(50, 77, 92);color: white;}"
		"QPushButton:hover{	background-color: rgb(3,110,184);color: white;}"
		"QLineEdit{border-radius: 0px;}"
		"QHeaderView::section {background-color:white;color: black;padding-left: 4px;border: 1px solid #6c6c6c;};"
		"color: white;padding-left: 4px;border: 1px solid #6c6c6c;}"
		"QHeaderView::section:checked{background-color: white;color: black;}");
	ui.tableView_2->setStyleSheet("border: 1px solid grey;color: rgb(34, 125, 155);	background-color: transparent;selection-color: black;selection-background-color:rgb(34, 125, 155);");	
	connect(ui.inputButton,SIGNAL(clicked()), this, SIGNAL(refresh()));
}
void ComponentManagement::initUI()
{
	ui.comboBox->clear();
	ui.comboBox_2->clear();
	QSqlQuery query(*sql.db);
	query.exec("select*from PARAMETER order by ID");
	QStringList list;
	bool Isexist;
	while(query.next())
	{
		for (int i=0;i<list.count();i++)
		{
			if (query.value(1).toString()==list.at(i))
				Isexist=true;
		}
		if(!Isexist)
			list.append(query.value(1).toString());
		Isexist=false;
	}
	Isexist=false;
	query.exec("select*from SATELLITE_PUBLIC order by ID");
	QStringList satlist;
	while(query.next())
	{
		for (int i=0;i<satlist.count();i++)
		{
			if (query.value(1).toString()==satlist.at(i))
				Isexist=true;
		}
		if(!Isexist)
		{
			satlist.append(query.value(1).toString());
		}
		Isexist=false;
	}
	ui.comboBox->addItems(list);
	ui.comboBox_2->addItems(satlist);
	ui.comboBox_2->setEnabled(false);
}
QStringList CSVLists; 
void ComponentManagement::on_openButton_clicked()
{

	QString fileName = QFileDialog::getOpenFileName(this, tr("Excel file"), qApp->applicationDirPath (),
		tr("Excel Files (*.csv)"));
	if (fileName.isEmpty())
		return;
	//读取 csv
	QFile csvFile(fileName); 
	ui.lineEdit->setText(fileName);
	CSVLists.clear();

	if (csvFile.open(QIODevice::ReadWrite)) 
	{ 
		QTextStream stream(&csvFile); 
		while (!stream.atEnd()) 
		{ 
			CSVLists.push_back(stream.readLine()); 
		} 
		csvFile.close(); 
	} 
	ui.tableWidget->clearContents();

	if (CSVLists.count()==0)
	{
		return;
	}
	QStringList header;  
	header<<QString::fromLocal8Bit("芯片名称")<<QString::fromLocal8Bit("规格")<<QString::fromLocal8Bit("封装")<<QString::fromLocal8Bit("额定电压")<<QString::fromLocal8Bit("总剂量")<<QString::fromLocal8Bit("单粒子翻转")<<QString::fromLocal8Bit("单粒子锁定")<<QString::fromLocal8Bit("厂家")<<QString::fromLocal8Bit("备注");  //<<"Height"
	ui.tableWidget->setRowCount(CSVLists.size()-1);
	ui.tableWidget->setColumnCount(9);  
	ui.tableWidget->setHorizontalHeaderLabels(header);  
	ui.tableWidget->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
	int row=0;
	Q_FOREACH(QString str, CSVLists) 
	{ 
		if (row!=0)
		{
			QStringList info = str.split(",");
			for (int i=0;i<9;i++)
			{
				ui.tableWidget->setItem(row-1,i,new QTableWidgetItem(info[i]));  
			}	
			//ui.tableWidget->setItem(row-1,1,new QTableWidgetItem(info[1]));  
			//ui.tableWidget->setItem(row-1,2,new QTableWidgetItem(info[2]));  
			//ui.tableWidget->setItem(row-1,3,new QTableWidgetItem(info[3]));  
			//ui.tableWidget->setItem(row-1,4,new QTableWidgetItem(info[4]));  
			//ui.tableWidget->setItem(row-1,5,new QTableWidgetItem(info[5]));  
			//ui.tableWidget->setItem(row-1,6,new QTableWidgetItem(info[6]));  
			//ui.tableWidget->setItem(row-1,7,new QTableWidgetItem(info[7]));  
		}
		row++;
	}
}
void ComponentManagement::on_inputButton_clicked()
{
	if (CSVLists.count()==0)
	{
		return;
	}
	Q_FOREACH(QString str, CSVLists) 
	{ 
		QStringList info = str.split(",");
		QSqlQuery query(*sql.db);
		query.exec("select*from PARAMETER where  CHIPNAME  ='"+info[0]+"'");
		if(query.next())
		{
			QMessageBox::information(this,QString::fromLocal8Bit("警告"),info[0]+QString::fromLocal8Bit("已存在"));
			return;
		}
	}
	QSqlQuery querypara(*sql.db);
	QSqlQuery querysource(*sql.db);
	querypara.exec("select*from PARAMETER order by ID");
	QString ID;
	int totalcount =0;
	while(querypara.next())
	{
		totalcount++;
	}

	QString time ;
	QString satelliteNo ;
	querypara.prepare("INSERT INTO PARAMETER "
		"VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");
	int row=0;
	bool IsSuccess;
	Q_FOREACH(QString str, CSVLists) 
	{ 
		if (row!=0)
		{
			QStringList info = str.split(",");
			querypara.bindValue(0, totalcount);			
			querypara.bindValue(1, info[0]);
			querypara.bindValue(2, info[1]);
			querypara.bindValue(3, info[2]);
			querypara.bindValue(4, info[3]);
			querypara.bindValue(5, info[4]);
			querypara.bindValue(6, info[5]);
			querypara.bindValue(7, info[6]);
			querypara.bindValue(8, info[7]);
			querypara.bindValue(9, info[8]);
			IsSuccess = querypara.exec();
			totalcount++;
		}
		row++;
	}
	if (IsSuccess)
	{
		QString str = str.fromLocal8Bit("提示");
		QString str2 = str.fromLocal8Bit("导入成功！");
		QMessageBox::information(this,str,str2);
		initUI();
		emit refresh(str);
	}
	//query.exec("DELETE *FROM Satellite");删除
	Isfresh=true;
}
void ComponentManagement::on_previewButton_clicked()
{
	if (CSVLists.count()==0)
	{
		return;
	}
	QStringList header;  
	header<<QString::fromLocal8Bit("芯片名称")<<QString::fromLocal8Bit("规格")<<QString::fromLocal8Bit("封装")<<QString::fromLocal8Bit("额定电压")<<QString::fromLocal8Bit("总剂量")<<QString::fromLocal8Bit("单粒子翻转")<<QString::fromLocal8Bit("单粒子锁定")<<QString::fromLocal8Bit("厂家")<<QString::fromLocal8Bit("备注");  //<<"Height"
	ui.tableWidget->setRowCount(CSVLists.size()-1);
	ui.tableWidget->setColumnCount(9);  
	ui.tableWidget->setHorizontalHeaderLabels(header);  
	ui.tableWidget->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
	int row=0;
	Q_FOREACH(QString str, CSVLists) 
	{ 
		if (row!=0)
		{
			QStringList info = str.split(",");
			for (int i=0;i<9;i++)
			{
				ui.tableWidget->setItem(row-1,i,new QTableWidgetItem(info[i]));  
			}	
		}
		row++;
	}
}
void ComponentManagement::on_outButton_clicked()
{
	QAbstractItemModel* model=ui.tableView_2->model();
	if (model==NULL)
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
	dlg.selectFile("parameter.xls");
	if(dlg.exec()!= QDialog::Accepted)
		return;
	QString filePath=dlg.selectedFiles()[0];
	if(OdbcExcel::saveFromTable(filePath,ui.tableView_2,"")) {
		QString str = str.fromLocal8Bit("提示");
		QString str2 = str.fromLocal8Bit("保存成功");
		QMessageBox::information(this,str,str2);
	}
	else{
		QString str = str.fromLocal8Bit("错误");
	//	QString str2 = str.fromLocal8Bit("保存成功");
		QString msg="保存失败！\n\r"+OdbcExcel::getError();
		QMessageBox::critical(this,str,tr(msg.toUtf8()));
	}
}
void ComponentManagement::on_queryButton_clicked()
{
	QSqlQueryModel *model=new QSqlQueryModel();
	ui.tableView_2->setModel(model);
}
void ComponentManagement::on_previewButton_2_clicked()
{
	QString strChipName = ui.comboBox->currentText();
	QString strSatName = ui.comboBox_2->currentText();
	QSqlQueryModel *model=new QSqlQueryModel();
	QString strsql;
	QStringList chipList;
	if (ui.radioButton->isChecked())
	{
		 strsql= "select * from PARAMETER where CHIPNAME = '"+strChipName+"' order by ID";
	}
	else
	{
		QSqlQuery query(*sql.db);
		query.exec("select*from SATELLITE_PUBLIC where SATELLITENO = '"+strSatName+"' order by ID");
		while(query.next())
		{
			chipList.append(query.value(3).toString());
		}
		int count =chipList.count();
		if(count==1)
			strsql= "select * from PARAMETER where CHIPNAME = '"+chipList.at(0)+"' order by ID";
		if (count>1)
		{
			strsql= "select * from PARAMETER where ";
			for (int i=0;i<count;i++)
			{
				strsql +="CHIPNAME = '"+chipList.at(i)+"'";
				if(i!=count-1)
				strsql +=" or ";
			}
		}
	}
	model->setQuery(strsql,*sql.db);
	model->setHeaderData(0,Qt::Horizontal,tr("ID"));
	QString str = str.fromLocal8Bit("芯片名称");
	model->setHeaderData(1,Qt::Horizontal,str);
	str = str.fromLocal8Bit("规格");
	model->setHeaderData(2,Qt::Horizontal,str);
	str = str.fromLocal8Bit("封装");
	model->setHeaderData(3,Qt::Horizontal,str);
	str = str.fromLocal8Bit("额定电压");
	model->setHeaderData(4,Qt::Horizontal,str);
	str = str.fromLocal8Bit("总剂量");
	model->setHeaderData(5,Qt::Horizontal,str);
	str = str.fromLocal8Bit("单粒子翻转");
	model->setHeaderData(6,Qt::Horizontal,str);
	str = str.fromLocal8Bit("单粒子锁定");
	model->setHeaderData(7,Qt::Horizontal,str);
	str = str.fromLocal8Bit("厂家");
	model->setHeaderData(8,Qt::Horizontal,str);
	str = str.fromLocal8Bit("备注");
	model->setHeaderData(9,Qt::Horizontal,str);
	ui.tableView_2->setModel(model);
	ui.tableView_2->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
}
void ComponentManagement::DisableInput()
{
	ui.lineEdit->setEnabled(false);
	ui.inputButton->setEnabled(false);
	//ui.previewButton->setEnabled(false);
	ui.openButton->setEnabled(false);
}
void ComponentManagement::on_radioButton_clicked()
{
	if (ui.radioButton->isChecked())
	{
		ui.comboBox_2->setEnabled(false);
		ui.comboBox->setEnabled(true);
	}
}
void ComponentManagement::on_radioButton_2_clicked()
{
	if (ui.radioButton_2->isChecked())
	{
		ui.comboBox->setEnabled(false);
		ui.comboBox_2->setEnabled(true);
	}
}
ComponentManagement::~ComponentManagement()
{

}
