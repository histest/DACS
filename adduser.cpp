#include "adduser.h"
#include "connectsql.h"
extern ConnectSql sql;
ADDUSER::ADDUSER(QWidget *parent): 
QDialog(parent)
{
	ui.setupUi(this);

}
 void ADDUSER::initUI()
 {
 }
 void ADDUSER::on_clearButton_clicked()
 {
	 ui.userEdit->clear();
	 ui.pswEdit->clear();
	 ui.pswEdit_2->clear();
 }
 void ADDUSER::on_okButton_clicked()
 {
	QSqlQuery queryuser(*sql.db);

	QString user =ui.userEdit->text().trimmed();
	QString pwd =ui.pswEdit->text().trimmed();
	QString pwd2=ui.pswEdit_2->text().trimmed();
	QString str = str.fromLocal8Bit("����");
	QString strAuthorized;
	if(ui.comboBox->currentIndex()==0)
	{
		strAuthorized="true";
	}
	if(ui.comboBox->currentIndex()==1)
	{
		strAuthorized="false";
	}
	if(user==NULL)
	{
		QString str2 = str.fromLocal8Bit("�������û�����");
		QMessageBox::warning(this,str,str2,QMessageBox::Ok);
		return;
	}
	if(pwd==NULL)
	{
		QString str2 = str.fromLocal8Bit("���������룡");
		QMessageBox::warning(this,str,str2,QMessageBox::Ok);
		return;
	}
	if (pwd!=pwd2)
	{
		QString str2 = str.fromLocal8Bit("����ȷ��ʧ�ܣ�");
		QMessageBox::warning(this,str,str2,QMessageBox::Ok);
		ui.pswEdit->clear();
		ui.pswEdit_2->clear();
		return;
	}
	queryuser.exec("select*from USERS where Name='"+user+"'");
	while(queryuser.next())
	{
		QString str2 = str.fromLocal8Bit("�û����Ѵ��ڣ�");
		QMessageBox::warning(this,str,str2,QMessageBox::Ok);
		ui.pswEdit->clear();
		ui.userEdit->clear();
		return;
	}

	queryuser.exec("select *from  USERS");
	int totalcount =0;
	QString ID;
	
	while(queryuser.next())
	{
		totalcount++;
	}
	
	 queryuser.prepare("INSERT INTO USERS (ID, Name, Password, IsAuthorized) "
		 "VALUES (:ID, :Name, :Password, :IsAuthorized)");
	 QString stree = QString::number(totalcount, 10); 
	 totalcount++;
	 queryuser.bindValue(":ID", totalcount);
	 queryuser.bindValue(":Name", user);
	 queryuser.bindValue(":Password", pwd);
	  queryuser.bindValue(":IsAuthorized",strAuthorized);
	 if(queryuser.exec())
	 {
		 QString str = str.fromLocal8Bit("��ʾ");
		 QString str2 = str.fromLocal8Bit("��ӳɹ���");
		 QMessageBox::information(this,str,str2);
		 ui.pswEdit->clear();
		 ui.pswEdit_2->clear();
	 }



 }
ADDUSER::~ADDUSER()
{

}
