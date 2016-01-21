#include "changepwd.h"
#include "connectsql.h"
extern ConnectSql sql;
Changepwd::Changepwd(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	initUI();
}
void Changepwd::initUI()
{
	QSqlQuery query(*sql.db);
	query.exec("select*from USERS order by ID");
	QStringList list;
	while(query.next())
	{
		list.append(query.value(1).toString());
	}
	ui.comboBox->addItems(list);
}
void Changepwd::on_clearButton_clicked()
{
	ui.oldpwdEdit->setText("");
	ui.newpwdEdit->setText("");
	ui.newpwdEdit_2->setText("");
}
void Changepwd::on_okButton_clicked()
 {
	 QString user =ui.comboBox->currentText().trimmed();
	 QString oldpwd =ui.oldpwdEdit->text().trimmed();
	 QString newpwd =ui.newpwdEdit->text().trimmed();
	 QString newpwd2 =ui.newpwdEdit_2->text().trimmed();
	 QString str = str.fromLocal8Bit("警告");

	 QSqlQuery queryuser(*sql.db);
	 queryuser.exec("select*from USERS where Name='admin'and Password='"+oldpwd+"'");

	 while(!queryuser.next())
	 {
		 QString str2 = str.fromLocal8Bit("管理员密码错误！");
		 QMessageBox::warning(this,str,str2,QMessageBox::Ok);
		 ui.oldpwdEdit->clear();
		 ui.newpwdEdit->clear();
		 ui.newpwdEdit_2->clear();
		 return;
	 }
	  if (newpwd=="")
	 {
		 QString str2 = str.fromLocal8Bit("请确认新密码！");
		 ui.oldpwdEdit->clear();
		 ui.newpwdEdit->clear();
		 ui.newpwdEdit_2->clear();
		 QMessageBox::warning(this,str,str2,QMessageBox::Ok);
		 return;
	 }
	 if (newpwd!=newpwd2)
	 {
		 QString str2 = str.fromLocal8Bit("新密码确认失败！");
		 ui.oldpwdEdit->clear();
		 ui.newpwdEdit->clear();
		 ui.newpwdEdit_2->clear();
		 QMessageBox::warning(this,str,str2,QMessageBox::Ok);
		 return;
	 }
	 //if (oldpwd==newpwd)
	 //{
		// QString str2 = str.fromLocal8Bit("与原密码相同！");
		// QMessageBox::warning(this,str,str2,QMessageBox::Ok);
		// ui.oldpwdEdit->clear();
		// ui.newpwdEdit->clear();
		// ui.newpwdEdit_2->clear();
		// return;
	 //}

	 bool bsucess= queryuser.exec("update USERS  set Password='"+newpwd+"'where Name='"+user+"'");
	 if(bsucess)
	 {
		 QString str = str.fromLocal8Bit("提示");
		 QString str2 = str.fromLocal8Bit("修改成功！");
		 QMessageBox::information(this,str,str2);
		 ui.oldpwdEdit->clear();
		 ui.newpwdEdit->clear();
		 ui.newpwdEdit_2->clear();
	 }

 }
Changepwd::~Changepwd()
{

}
