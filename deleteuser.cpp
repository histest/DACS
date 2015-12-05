#include "deleteuser.h"
#include "connectsql.h"
extern ConnectSql sql;
Deleteuser::Deleteuser(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	initUI();
}
 void Deleteuser::initUI()
{
	ui.listWidget->clear();
	QSqlQuery query(*sql.db);
	query.exec("select*from USERS order by ID ");
	QStringList list;
	while(query.next())
	{
		if (query.value(1).toString()!="admin")
		{
			list.append(query.value(1).toString());
		}
	}
	ui.listWidget->addItems(list);
}
 void Deleteuser::on_clearButton_clicked()
 {
 }
 void Deleteuser::on_okButton_clicked()
 {
	 int ok = QMessageBox::warning(this,QString::fromLocal8Bit("警告"),QString::fromLocal8Bit("确定删除？"),QMessageBox::Yes,QMessageBox::No);
	 if(ok == QMessageBox::Yes)
	 {
		 QString str = str.fromLocal8Bit("警告");
		 QSqlQuery query(*sql.db);
		 QList<QListWidgetItem*> list=ui.listWidget->selectedItems();
		 bool bsucess;
		 for (int i=0;i<list.count();i++)
		 {
			 bsucess =  query.exec("delete from USERS where Name='"+list[i]->text()+"'");
			
		 }
		/* QString strID = QString::number(ID);
		 queryuser.exec("select*from User where ID >'"+strID+"'");
		 while(queryuser.next())
		 {
			 ID++;
			 QString strID2 = QString::number(ID);
			 int temp= ID-1;
			 QString strID3 = QString::number(temp);
			 query.exec("update USERS  set ID='"+strID3+"'where ID='"+strID2+"'");
		 }*/
		 if(bsucess)
		 {
			 QString str = str.fromLocal8Bit("提示");
			 QString str2 = str.fromLocal8Bit("删除成功！");
			 QMessageBox::information(this,str,str2);
			 initUI();
		 }
	 }
 }
 void Deleteuser::on_cancelButton_clicked()
 {
	 this->close();
 }
Deleteuser::~Deleteuser()
{

}
