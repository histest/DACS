#include "dictionarymanagement.h"
#include <QDesktopServices>
#include "connectsql.h"
#include <vector>  
using namespace std;
extern ConnectSql sql;
vector <QString> vtpara; 
DictionaryManagement::DictionaryManagement(QWidget *parent)
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
	namelistview = new QListView(this);
	model = new QStringListModel(this);
	namelistview->setWindowFlags(Qt::ToolTip);
	installEventFilter(namelistview);
	connect(namelistview, SIGNAL(clicked(const QModelIndex &)), this, SLOT(completeText(const QModelIndex &)));
	connect(ui.comboBox, SIGNAL(editTextChanged(const QString &)), this, SLOT(setCompleter(const QString &)));

}
void DictionaryManagement::initUI()
{
	ui.comboBox->clear();
	QSqlQuery query(*sql.db);
	query.exec("select*from DICTIONARY order by ID");
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
			vtpara.push_back(query.value(1).toString());
		Isexist=false;
	}
	ui.comboBox->addItems(list);
	//for (int i=0;i<namelist->count();i++)
	//{
	//	vtpara.push_back(namelist->at(i));
	//}
}
QStringList CSVListdictionary; 
void DictionaryManagement::on_openButton_clicked()
{

	QString fileName = QFileDialog::getOpenFileName(this, tr("Excel file"), qApp->applicationDirPath (),
		tr("Excel Files (*.csv)"));
	if (fileName.isEmpty())
		return;
	//读取 csv
	QFile csvFile(fileName); 
	ui.lineEdit->setText(fileName);
	CSVListdictionary.clear();

	if (csvFile.open(QIODevice::ReadWrite)) 
	{ 
		QTextStream stream(&csvFile); 
		while (!stream.atEnd()) 
		{ 
			CSVListdictionary.push_back(stream.readLine()); 
		} 
		csvFile.close(); 
	} 
	ui.tableWidget->clearContents();

	if (CSVListdictionary.count()==0)
	{
		return;
	}
	QStringList header;  
	header<<QString::fromLocal8Bit("序号")<<QString::fromLocal8Bit("名称")<<QString::fromLocal8Bit("数值")<<QString::fromLocal8Bit("解释");
	ui.tableWidget->setRowCount(CSVListdictionary.size()-1);
	ui.tableWidget->setColumnCount(4);  
	ui.tableWidget->setHorizontalHeaderLabels(header);  
	ui.tableWidget->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
	int row=0;
	Q_FOREACH(QString str, CSVListdictionary) 
	{ 
		if (row!=0)
		{
			QStringList info = str.split(",");
			ui.tableWidget->setItem(row-1,0,new QTableWidgetItem(info[0]));  
			ui.tableWidget->setItem(row-1,1,new QTableWidgetItem(info[1]));  
			ui.tableWidget->setItem(row-1,2,new QTableWidgetItem(info[2]));  
			ui.tableWidget->setItem(row-1,3,new QTableWidgetItem(info[3]));  
		}
		row++;
	}
}
void DictionaryManagement::on_inputButton_clicked()
{
	if (CSVListdictionary.count()==0)
	{
		return;
	}
	Q_FOREACH(QString str, CSVListdictionary) 
	{ 
		QStringList info = str.split(",");
		QSqlQuery query(*sql.db);
		QString strsql = QString("select*from DICTIONARY where  %1  ='"+info[1]+"' and %2  ='"+info[2]+"'and %3  ='"+info[3]+"'").arg("NAME").arg(QString::fromLocal8Bit("数值")).arg(QString::fromLocal8Bit("解释"));
		query.exec(strsql);
		if(query.next())
		{
			QMessageBox::information(this,QString::fromLocal8Bit("警告"),info[1]+QString::fromLocal8Bit("的")+info[2]+QString::fromLocal8Bit("解释")+QString::fromLocal8Bit("已存在"));
			return;
		}
	}
	QSqlQuery querypara(*sql.db);
	querypara.exec("select*from DICTIONARY order by ID");
	QString ID;
	int totalcount =0;
	while(querypara.next())
	{
		totalcount++;
	}

	QString time ;
	QString satelliteNo ;
	querypara.prepare("INSERT INTO DICTIONARY VALUES (?, ?, ?, ?)");
	int row=0;
	Q_FOREACH(QString str, CSVListdictionary) 
	{ 
		if (row!=0)
		{
			QStringList info = str.split(",");
			querypara.bindValue(0, totalcount);			
			querypara.bindValue(1, info[1]);
			querypara.bindValue(2, info[2]);
			querypara.bindValue(3, info[3]);
			querypara.exec();
			totalcount++;
		}
		row++;
	}
	QString str = str.fromLocal8Bit("提示");
	QString str2 = str.fromLocal8Bit("导入成功！");
	QMessageBox::information(this,str,str2);
	initUI();
}
void DictionaryManagement::on_previewButton_clicked()
{
	if (CSVListdictionary.count()==0)
	{
		return;
	}
	QStringList header;  
	header<<QString::fromLocal8Bit("序号")<<QString::fromLocal8Bit("名称")<<QString::fromLocal8Bit("数值")<<QString::fromLocal8Bit("解释");
	ui.tableWidget->setRowCount(CSVListdictionary.size()-1);
	ui.tableWidget->setColumnCount(4);  
	ui.tableWidget->setHorizontalHeaderLabels(header);  
	ui.tableWidget->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
	int row=0;
	Q_FOREACH(QString str, CSVListdictionary) 
	{ 
		if (row!=0)
		{
			QStringList info = str.split(",");
			ui.tableWidget->setItem(row-1,0,new QTableWidgetItem(info[0]));  
			ui.tableWidget->setItem(row-1,1,new QTableWidgetItem(info[1]));  
			ui.tableWidget->setItem(row-1,2,new QTableWidgetItem(info[2]));  
			ui.tableWidget->setItem(row-1,3,new QTableWidgetItem(info[3]));  
		}
		row++;
	}
}
void DictionaryManagement::on_outButton_clicked()
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
	dlg.selectFile("dictionary.xls");
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
void DictionaryManagement::on_queryButton_clicked()
{
	QSqlQueryModel *model=new QSqlQueryModel();
	ui.tableView_2->setModel(model);
}
void DictionaryManagement::on_previewButton_2_clicked()
{
	QString strName = ui.comboBox->currentText();
	QSqlQueryModel *model=new QSqlQueryModel();
	QString strsql =QString("select * from DICTIONARY where NAME = '"+strName+"' order by ID");//.arg(QString::fromLocal8Bit("名称"));
	model->setQuery(strsql,*sql.db);
	model->setHeaderData(0,Qt::Horizontal,tr("ID"));
	QString str = str.fromLocal8Bit("名称");
	model->setHeaderData(1,Qt::Horizontal,str);
	str = str.fromLocal8Bit("数值");
	model->setHeaderData(2,Qt::Horizontal,str);
	str = str.fromLocal8Bit("解释");
	model->setHeaderData(3,Qt::Horizontal,str);
	ui.tableView_2->setModel(model);
	ui.tableView_2->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
}
void DictionaryManagement::setCompleter(const QString &text) {
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


	QString strsql= "select * from PARAMETER where SATELLITENO like '"+text+"'";
	query.exec(strsql);
	for (int i =0;i<ui.comboBox->count();i++)
	{
		if (ui.comboBox->itemText(i)==text)
		{
			ui.comboBox->setCurrentIndex(i);
			return;
		}
	}

	QStringList list;
	for(int i = 0;i < vtpara.size();++i){

		if (vtpara[i].indexOf(text) != -1)
		{

			list.append(vtpara[i]);
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
	namelistview->setGeometry(this->x()+185, this->y()+185, 50, 100);
	namelistview->resize(100,200);
	namelistview->setFixedWidth(200);
	namelistview->show();
	//	connect(ui.comboBox,SIGNAL(editTextChanged (const QString & )),this,SLOT(setCompleter(const QString & )));  
}
void DictionaryManagement::completeText(const QModelIndex &index) {
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
DictionaryManagement::~DictionaryManagement()
{

}
