#include "systemmanagement.h"
#include "QFramer/ftabwidget.h"
#include "QFramer/futil.h"
#include <QtCore/QTextCodec>
#include "usermanagement.h"
#include "dbmanagement.h"
SystemManagement::SystemManagement(QWidget *parent):
	FTabWidget(parent)
{
	initUI();
}
void SystemManagement::initUI()
{
	UserManage();
	DBManage();	
	getButtons().at(0)->click();
}
void SystemManagement::UserManage()
{
	QTextCodec *codec = QTextCodec::codecForName("GBK");
	char *string = "�û�����";
	QString strText = codec->toUnicode(string);
	UserManagement *usermanagement =new UserManagement;
	addWidget(strText, QString(""), usermanagement,6);
}
void SystemManagement::DBManage()
{
	QTextCodec *codec = QTextCodec::codecForName("GBK");
	char *string = "���ݿ����";
	QString strText = codec->toUnicode(string);
	DBManagement*dbmanagement = new DBManagement;
	addWidget(strText, QString(""), dbmanagement,7);
}
