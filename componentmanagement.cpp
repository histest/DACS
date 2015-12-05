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
	QSqlQuery query(*sql.db);
	query.exec("select*from PARAMETER order by ID");
	QStringList list;
	QString lastChip;
	while(query.next())
	{
		if(lastChip!=query.value(3).toString())
			list.append(query.value(3).toString());
		lastChip=query.value(3).toString();

	}
	ui.comboBox->addItems(list);
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
	header<<QString::fromLocal8Bit("卫星编号")<<QString::fromLocal8Bit("轨道高度")<<QString::fromLocal8Bit("芯片名称")<<QString::fromLocal8Bit("规格")<<QString::fromLocal8Bit("封装")<<QString::fromLocal8Bit("额定电压")<<QString::fromLocal8Bit("额定电流")<<QString::fromLocal8Bit("抗辐射指标");  //<<"Height"
	ui.tableWidget->setRowCount(CSVLists.size()-1);
	ui.tableWidget->setColumnCount(8);  
	ui.tableWidget->setHorizontalHeaderLabels(header);  
	ui.tableWidget->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
	int row=0;
	Q_FOREACH(QString str, CSVLists) 
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
		query.exec("select*from PARAMETER where SATELLITENO  ='"+info[0]+"'");//and DATETIME = '"+info[0]+"'"
		if(query.next())
		{
			QMessageBox::information(this,QString::fromLocal8Bit("警告"),info[0]+QString::fromLocal8Bit("卫星已存在"));
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
		"VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?)");
	int row=0;
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
			querypara.exec();
			totalcount++;
		}
		row++;
	}
	QString str = str.fromLocal8Bit("提示");
	QString str2 = str.fromLocal8Bit("导入成功！");
	QMessageBox::information(this,str,str2);
	initUI();
	//query.exec("DELETE *FROM Satellite");删除
	emit refresh(str);
	Isfresh=true;
}
void ComponentManagement::on_previewButton_clicked()
{
	if (CSVLists.count()==0)
	{
		return;
	}
	QStringList header;  
	header<<QString::fromLocal8Bit("卫星编号")<<QString::fromLocal8Bit("轨道高度")<<QString::fromLocal8Bit("芯片名称")<<QString::fromLocal8Bit("规格")<<QString::fromLocal8Bit("封装")<<QString::fromLocal8Bit("额定电压")<<QString::fromLocal8Bit("额定电流")<<QString::fromLocal8Bit("抗辐射指标");  //<<"Height"
	ui.tableWidget->setRowCount(CSVLists.size()-1);
	ui.tableWidget->setColumnCount(8);  
	ui.tableWidget->setHorizontalHeaderLabels(header);  
	ui.tableWidget->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
	int row=0;
	Q_FOREACH(QString str, CSVLists) 
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
	/*QSqlQueryModel *model=new QSqlQueryModel();
	QString fileName = QFileDialog::getSaveFileName(this, tr("Excel file"), qApp->applicationDirPath (),
	tr("Excel Files (*.xls)"));
	if (fileName.isEmpty())
	return;

	ExportExcelObject obj(fileName, "mydata", ui.tableView_2);

	obj.addField(0, tr("ID"), "char(20)");
	obj.addField(1, tr("DateTime"), "char(20)");
	obj.addField(2, tr("Count"), "char(20)");
	obj.addField(3, tr("Longitude"), "char(20)");
	obj.addField(4, tr("Latitude"), "char(20)");
	obj.addField(5, tr("Height"), "char(20)");
	obj.addField(6, tr("Sun_x"), "char(20)");
	obj.addField(7, tr("Sun_y"), "char(20)");
	obj.addField(8, tr("Sun_z"), "char(20)");
	obj.addField(9, tr("Sun_s"), "char(20)");
	obj.addField(10, tr("SatelliteNo"), "char(20)");

	int retVal = obj.export2Excel();
	if( retVal > 0)
	{

		QMessageBox::information(this, tr("Done"),
			QString(tr("%1 records exported!")).arg(retVal)
			);
	}*/



	/*QString line_0("0, aaa, 000\n"); 
	QString line_1("1, bbb, 111\n"); 
	QString line_2("2, ccc, 222\n"); 

	if (csvFile.open(QIODevice::ReadWrite)) 
	{ 
	csvFile.write(line_0.toAscii()); 
	csvFile.write(line_1.toAscii()); 
	csvFile.write(line_2.toAscii()); 
	csvFile.close(); 
	} */
}
void ComponentManagement::on_queryButton_clicked()
{
	QSqlQueryModel *model=new QSqlQueryModel();
	ui.tableView_2->setModel(model);
}
void ComponentManagement::on_previewButton_2_clicked()
{
	QString strChipName = ui.comboBox->currentText();
	QSqlQueryModel *model=new QSqlQueryModel();
	QString strsql = "select * from PARAMETER where CHIPNAME = '"+strChipName+"' order by ID";
		////#"+str1+"# and #"+str2+"#"
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
void ComponentManagement::DisableInput()
{
	ui.lineEdit->setEnabled(false);
	ui.inputButton->setEnabled(false);
	//ui.previewButton->setEnabled(false);
	ui.openButton->setEnabled(false);
}
ComponentManagement::~ComponentManagement()
{

}
