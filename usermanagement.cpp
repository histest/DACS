#include "usermanagement.h"
#include "connectsql.h"
#include "deleteuser.h"
#include "adduser.h"
#include "changepwd.h"
extern ConnectSql sql;
UserManagement::UserManagement(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	initUI();
	//setStyleSheet("QPushButton{background-image: url(:/skin/icons/dark/appbar.paper.png);border-color:grey;background-repeat:no-repeat;background-position: center top;color: black;border-style: outset;border-color:rgb(27, 118, 150);border-width: 1px;border-radius: 5px;text-align: center bottom;}"
	/*"QPushButton:pressed{background-color: rgb(50, 77, 92);color: white;}"
	"QPushButton:hover{	background-color: rgb(34, 125, 155);color: white;}");*/

	ui.addButton->setStyleSheet("QPushButton{background-image: url(:/skin/icons/dark/appbar.group.add.png);border-color:grey;background-repeat:no-repeat;background-position: center top;color: white;border-style: outset;border-width: 1px;border-radius: 5px;text-align: center bottom;}");
	ui.changeButton->setStyleSheet("QPushButton{background-image: url(:/skin/icons/dark/appbar.key.old.png);border-color:grey;background-repeat:no-repeat;background-position: center top;color: white;border-style: outset;border-width: 1px;border-radius: 5px;text-align: center bottom;}");
	ui.deleteButton->setStyleSheet("QPushButton{background-image: url(:/skin/icons/dark/appbar.group.delete.png);border-color:grey;background-repeat:no-repeat;background-position: center top;color: white;border-style: outset;border-width: 1px;border-radius: 5px;text-align: center bottom;}");
	///setStyleSheet("QPushButton{background-image: url(:/skin/icons/dark/appbar.paper.png);background-repeat:no-repeat;background-position: center top;background-color: rgb(240, 240, 240);color: rgb(70,130,180);text-align: center bottom;padding-bottom: 2px;border-width: 1px;border-radius: 5px;}"
	//	"QPushButton:hover{background-color: rgb(27, 118, 150);color: white;}"
	//	"QPushButton:pressed{background-color: rgb(70,130,180);color: white;}"
	//	"QPushButton:checked{background-color: rgb(34, 125, 155);color: white;}");//border-color:rgb(27, 118, 150);
}

 void UserManagement::initUI()
{
	/*ui.pswEdit->clear();
	ui.pwdEdit->clear();
	ui.newpwdEdit->clear();
	ui.comboBox->clear();
	ui.comboBox_2->clear();
	QSqlQuery query(*sql.db);
	query.exec("select*from USERS ");
	QStringList list;
	while(query.next())
	{
	list.append(query.value(1).toString());
	}
	ui.comboBox->addItems(list);
	ui.comboBox_2->addItems(list);*/
}
 void UserManagement::on_addButton_clicked()
 {
	 ADDUSER  adduser =new ADDUSER;
	 adduser.exec();
	/*QSqlQuery queryuser(*sql.db);

	QString user =ui.userEdit->text().trimmed();
	QString pwd =ui.pswEdit->text().trimmed();
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

	queryuser.exec("select*from USERS where Name='"+user+"'");
	while(queryuser.next())
	{
		QString str2 = str.fromLocal8Bit("用户名已存在！");
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
	
	 queryuser.prepare("INSERT INTO USERS (ID, Name, Password) "
		 "VALUES (:ID, :Name, :Password)");
	 QString stree = QString::number(totalcount, 10); 
	 totalcount++;
	 queryuser.bindValue(":ID", totalcount);
	 queryuser.bindValue(":Name", user);
	 queryuser.bindValue(":Password", pwd);
	 if(queryuser.exec())
	 {
		 QString str = str.fromLocal8Bit("提示");
		 QString str2 = str.fromLocal8Bit("添加成功！");
		 QMessageBox::information(this,str,str2);
		 initUI();
	 }
*/
	 //int count = totalcount;

 }
 void UserManagement::on_changeButton_clicked()
 {
	 Changepwd changepwd = new Changepwd;
	 changepwd.exec();
	 /*QString user =ui.comboBox->currentText().trimmed();
	 QString oldpwd =ui.oldpwdEdit->text().trimmed();
	 QString newpwd =ui.newpwdEdit->text().trimmed();
	 QString str = str.fromLocal8Bit("警告");

	 QSqlQuery queryuser(*sql.db);
	 queryuser.exec("select*from USERS where Name='"+user+"'and Password='"+oldpwd+"'");
	 
	 while(!queryuser.next())
	 {
		 QString str2 = str.fromLocal8Bit("密码错误！");
		 QMessageBox::warning(this,str,str2,QMessageBox::Ok);
		 ui.oldpwdEdit->clear();
		 ui.newpwdEdit->clear();
		 return;
	 }

	 if (oldpwd==newpwd)
	 {
		 QString str2 = str.fromLocal8Bit("与原密码相同！");
		 QMessageBox::warning(this,str,str2,QMessageBox::Ok);
		 ui.oldpwdEdit->clear();
		 ui.newpwdEdit->clear();
		 return;
	 }
	 bool bsucess= queryuser.exec("update USERS  set Password='"+newpwd+"'where Name='"+user+"'");
	 if(bsucess)
	 {
		 QString str = str.fromLocal8Bit("提示");
		 QString str2 = str.fromLocal8Bit("修改成功！");
		 QMessageBox::information(this,str,str2);
		 ui.oldpwdEdit->clear();
		 ui.newpwdEdit->clear();
	 }*/

 }
 void UserManagement::on_deleteButton_clicked()
 {
	 Deleteuser deleteuser =new Deleteuser;
	 deleteuser.exec();
	 /* QString user =ui.comboBox_2->currentText().trimmed();
	 QString pwd =ui.pwdEdit->text().trimmed();
	 QString str = str.fromLocal8Bit("警告");

	 QSqlQuery queryuser(*sql.db);
	 QSqlQuery query(*sql.db);
	 queryuser.exec("select*from USERS where Name='"+user+"'and Password='"+pwd+"'");

	 while(!queryuser.next())
	 {
	 QString str2 = str.fromLocal8Bit("密码错误！");
	 QMessageBox::warning(this,str,str2,QMessageBox::Ok);
	 ui.pwdEdit->clear();
	 return;
	 }
	 double ID;
	 while(queryuser.next())
	 {
	 ID=queryuser.value(0).toDouble();
	 }
	 bool bsucess=  queryuser.exec("delete * from USERS where Name='"+user+"'");
	 QString strID = QString::number(ID);
	 queryuser.exec("select*from User where ID >'"+strID+"'");
	 while(queryuser.next())
	 {
	 ID++;
	 QString strID2 = QString::number(ID);
	 int temp= ID-1;
	 QString strID3 = QString::number(temp);
	 query.exec("update USERS  set ID='"+strID3+"'where ID='"+strID2+"'");
	 }
	 if(bsucess)
	 {
	 QString str = str.fromLocal8Bit("提示");
	 QString str2 = str.fromLocal8Bit("删除成功！");
	 QMessageBox::information(this,str,str2);
	 initUI();
	 }*/

 }
UserManagement::~UserManagement()
{

}
