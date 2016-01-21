#include "connectdb.h"
#include "ui_connectdb.h"
#include "connectsql.h"
#include "QSettings"
#include "QSysInfo"
ConnectSql sql;
ConnectDB::ConnectDB(QWidget *parent): 
QDialog(parent),
	ui(new Ui::ConnectDB)
{
	ui->setupUi(this);
	this->setFixedSize(385,325);
	initUI();
	connect(ui->comboBox,SIGNAL(currentIndexChanged(int)), this, SLOT(onChanged(int)));
	ui->widget->setStyleSheet("QWidget{background-color: rgb(70,130,180);}"
		"QPushButton{background-color: transparent;color: white;border-style: outset;border-color:rgb(27, 118, 150);}"
		"QPushButton:pressed{background-color: rgb(50, 77, 92);color: white;}"
		"QPushButton:hover{	background-color: rgb(3,110,184);color: white;}");
	setStyleSheet("QDialog{background-color:  rgb(240,240,240);}"
		"QDialog::QTitleBar{ background-color: rgb(70,130,180);}"
		"QPushButton{background-color: rgb(70,130,180);color: white;border-style: outset;}"
		"QPushButton:pressed{background-color: rgb(50, 77, 92);color: white;}"
		"QPushButton:hover{	background-color: rgb(3,110,184);color: white;}"
		"QLabel{background-color: transparent;color: black;property-alignment: AlignCenter;}"
		"QGroupBox {background-color: transparent;border: 2px solid rgb(70,130,180);border-width: 0.8px;border-radius: 5px;margin-top: 1px;}"
		"QGroupBox::title {subcontrol-origin: margin;subcontrol-position: top left;font-size: 15px;font-weight: bold; color:black;}"
		"QComboBox { background: transparent;border: 1px solid rgb(3,110,184);border-radius: 3px;padding: 1px 18px 1px 3px;min-width: 6em;}" 
		"QComboBox::hover{border-color:#5D8B9E;}"  
		"QComboBox:editable {background: rgb(3,110,184);}"  
		);
	ui->label->setStyleSheet("QLabel{ background-color: transparent;color: white;qproperty-alignment: AlignCenter;border-color:rgb(27, 118, 150);border-width: 1px;border-radius: 5px;}");
	//border-style: outset;border-width: 1px;border-radius: 5px;
}

void ConnectDB::initUI()
{

	setWindowFlags(Qt::FramelessWindowHint);//WindowMinMaxButtonsHintWindowTitleHint
	ui->groupBox_3->setVisible(false);
	ui->lineEdit->setEnabled(false);
	ui->cancelButton_2->setVisible(false);
	ui->exitButton->setVisible(false);
	ui->groupBox_4->setVisible(false);
	onChanged(0);

	QString strfile;
	strfile = QCoreApplication::applicationDirPath();
	QString sDbNm = strfile+"/DACS.accdb";  
	QString dsn = QString("DRIVER={Microsoft Access Driver (*.mdb, *.accdb)}; FIL={MS Access};DBQ=%1;").arg(sDbNm);//连接字符串  
	ui->plainTextEdit->setPlainText(sDbNm);

	IsAdmin=false;

	QSettings setting("oracle.ini",QSettings::IniFormat);//读配置文件
	setting.beginGroup("config");
	ui->editHostname->setText(setting.value("server").toString());
	ui->editUsername->setText(setting.value("username").toString());
	ui->editPassword->setText(setting.value("password").toString());
	ui->editPort->setText(setting.value("port").toString());
	ui->editDatabase->setText(setting.value("databasename").toString());
	setting.endGroup();
}
ConnectDB::~ConnectDB()
{
	delete ui;
}
void ConnectDB::on_okButton_clicked()
{
	QString strdb[5] ;
	if (ui->comboBox->currentIndex()==1)
	{
		strdb[0]=ui->editHostname->text();
		strdb[1]=ui->editUsername->text();
		strdb[2]=ui->editPassword->text();
		strdb[3]=ui->editPort->text();
		strdb[4]=ui->editDatabase->text();
	}
	else
	{
		strdb[0]=ui->plainTextEdit->toPlainText();
	}

	bool isconnect = sql.connect(ui->comboBox->currentIndex(),strdb);


	if (isconnect)
	{
		/*QString str = str.fromLocal8Bit("提示");
		QString str2 = str.fromLocal8Bit("连接数据库成功！");
		QMessageBox::information(this,str,str2);*/
		if (sql.dboracle==true)
		{
			QSettings settings("oracle.ini", QSettings::IniFormat);
			settings.beginGroup("config");
			settings.setValue("databasename",ui->editDatabase->text().trimmed());			
			settings.setValue("username", ui->editUsername->text().trimmed());
			settings.setValue("password", ui->editPassword->text().trimmed());
			settings.setValue("server", ui->editHostname->text().trimmed());
			settings.setValue("port", ui->editPort->text().trimmed());		
			settings.endGroup();
		}



		ui->groupBox_2->setVisible(false);

		ui->cancelButton->setVisible(false);
		ui->okButton->setVisible(false);
//		ui->label->setVisible(false);
		ui->groupBox_4->setVisible(true);
		QSqlQuery query(*sql.db);		
		query.exec("select*from USERS order  by ID");
		QStringList list;
		while(query.next())
		{
			list.append(query.value(1).toString());

		}
		ui->comboBox_2->addItems(list);
		//accept();
	}
	
}
void ConnectDB::on_loginButton_clicked()
{
	QString user = ui->comboBox_2->currentText().trimmed();
	QString pwd =ui->lineEdit_2->text().trimmed();
	QString str = str.fromLocal8Bit("警告");
	if(user==NULL)
	{
		QString str2 = str.fromLocal8Bit("请输入用户名！");
		QMessageBox::warning(this,str,str2,QMessageBox::Ok);
		return;
	}
	if(pwd==NULL)
	{
		QString str2 = str.fromLocal8Bit("请输入密码！");
		QMessageBox::warning(this,str,str2,QMessageBox::Ok);
		return;
	}

	//sql.connect(ui->comboBox_2->currentIndex());

	QSqlQuery query(*sql.db);
	query.exec("select * from USERS where Name='"+user+"' and Password='"+pwd+"'");
	QString strIsAuthorized;
	if(query.next())
	{
		if (user=="admin")
		{
			IsAdmin=true;
		}
		IsAuthorized=query.value(3).toBool();
		sql.struser=user;
		accept();
	}
	else
	{
		QString str2 = str.fromLocal8Bit("用户名或密码错误！");
		QMessageBox::warning(this,str,str2,QMessageBox::Ok);
		ui->lineEdit_2->clear();
		ui->comboBox_2->setFocus();
	}
}
void ConnectDB::on_cancelButton_2_clicked()
{
	this->close();
}
void ConnectDB::on_exitButton_clicked()
{
	this->close();
}
void ConnectDB::on_cancelButton_clicked()
{
	this->close();
}
void ConnectDB::onChanged(int index)
{
	if(index==1)
	{
		ui->groupBox->setVisible(true);
		ui->groupBox_3->setVisible(false);
		/*ui->editDatabase->setVisible(true);
		ui->editHostname->setVisible(true);
		ui->editPassword->setVisible(true);
		ui->editUsername->setVisible(true);
		ui->textLabel3->setVisible(true);

		ui->textLabel4->setVisible(true);
		ui->textLabel4_2->setVisible(true);
		ui->textLabel5->setVisible(true);
		ui->textLabel5_2->setVisible(true);
		ui->label_2->setVisible(true);
		ui->label_3->setVisible(true);
		ui->editPort->setVisible(true);
		ui->textLabel3_2->setVisible(false);
		ui->textLabel3_3->setVisible(false);

		ui->plainTextEdit->setVisible(false);
		ui->openButton->setVisible(false);
	ui->lineEdit->setVisible(false);*/
		
	}
	else
	{
		ui->groupBox->setVisible(false);
		ui->groupBox_3->setVisible(true);
		/*ui->editDatabase->setVisible(false);
		ui->editHostname->setVisible(false);
		ui->editPassword->setVisible(false);
		ui->editUsername->setVisible(false);
		ui->textLabel3->setVisible(false);

		ui->textLabel4->setVisible(false);
		ui->textLabel4_2->setVisible(false);
		ui->textLabel5->setVisible(false);
		ui->textLabel5_2->setVisible(false);
		ui->editPort->setVisible(false);

		ui->label_2->setVisible(false);
		ui->label_3->setVisible(false);

		ui->textLabel3_2->setVisible(true);
		ui->textLabel3_3->setVisible(true);
		ui->plainTextEdit->setVisible(true);
		ui->openButton->setVisible(true);
		ui->lineEdit->setVisible(true);*/
	}
}
void ConnectDB:: on_closeButton_clicked()
{
	this->close();
}
void ConnectDB:: on_openButton_clicked()
{
	QString str = str.fromLocal8Bit("选择数据库");
	QString fileName = QFileDialog::getOpenFileName(this, str, qApp->applicationDirPath (),
		tr("Access Files (*.accdb)"));
	if (fileName.isEmpty())
		return;
	//QString dsn = QString("DRIVER={Microsoft Access Driver (*.mdb, *.accdb)}; FIL={MS Access};DBQ=%1;").arg(fileName);//连接字符串  
	ui->plainTextEdit->setPlainText(fileName);
}

