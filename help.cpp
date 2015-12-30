#include "help.h"
#include "QProcess"
#include "QDesktopServices"
#include "QFramer/futil.h"
#include <QHBoxLayout>
help::help(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	setStyleSheet("QPushButton{background-color: rgb(70,130,180);color: white;border-style: outset;border-color:rgb(27, 118, 150);border-width: 1px;border-radius: 5px;}"
		"QPushButton:pressed{background-color: rgb(50, 77, 92);color: white;}"
		"QPushButton:hover{	background-color: rgb(3,110,184);color: white;}"
		"QLabel{ background-color: transparent;color: black;qproperty-alignment: AlignCenter;border-color:rgb(27, 118, 150);border-width: 1px;border-radius: 5px;}"
		"QTextBrowser{ background-color: transparent;black: white;border-color:rgb(27, 118, 150);border-width: 1px;border-radius: 5px;}");
}

void help::on_commandLinkButton_clicked()
{
	QString strfile;
	strfile = QCoreApplication::applicationDirPath();
	QString strPath = strPath.fromLocal8Bit("/在轨数据收集与分析系统需求分析.doc");
	strPath=strfile+strPath;
	QDesktopServices::openUrl ( QUrl::fromLocalFile(strPath));
}
void help::on_commandLinkButton_2_clicked()
{
	QString strfile;
	strfile = QCoreApplication::applicationDirPath();
	QString strPath = strPath.fromLocal8Bit("/在轨数据收集与分析系统设计报告.doc");
	strPath=strfile+strPath;
	QDesktopServices::openUrl ( QUrl::fromLocalFile(strPath));
}
void help::on_commandLinkButton_3_clicked()
{
	QString strfile;
	strfile = QCoreApplication::applicationDirPath();
	QString strPath = strPath.fromLocal8Bit("/在轨数据收集与分析系统测试细则.doc");
	strPath=strfile+strPath;
	QDesktopServices::openUrl ( QUrl::fromLocalFile(strPath));
}
void help::on_commandLinkButton_4_clicked()
{
	QString strfile;
	strfile = QCoreApplication::applicationDirPath();
	QString strPath = strPath.fromLocal8Bit("/在轨数据收集与分析系统测试报告.doc");
	strPath=strfile+strPath;
	QDesktopServices::openUrl ( QUrl::fromLocalFile(strPath));
}
void help::on_commandLinkButton_5_clicked()
{
	QString strfile;
	strfile = QCoreApplication::applicationDirPath();
	QString strPath = strPath.fromLocal8Bit("/在轨数据收集与分析系统使用说明书.doc");
	strPath=strfile+strPath;
	QDesktopServices::openUrl ( QUrl::fromLocalFile(strPath));
}
void help::on_commandLinkButton_6_clicked()
{
	QString strfile;
	strfile = QCoreApplication::applicationDirPath();
	QString strPath = strPath.fromLocal8Bit("/在轨数据收集与分析系统研制总结报告.doc");
	strPath=strfile+strPath;
	QDesktopServices::openUrl ( QUrl::fromLocalFile(strPath));
}
help::~help()
{

}
