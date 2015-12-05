#include "savedialog.h"
//#include "ui_savedialog.h"
#include <QPushButton>

SaveDialog::SaveDialog(QWidget *parent) :
    QDialog(parent)
{
  //  ui->setupUi(this);
	this->resize(165, 23);
	setWindowFlags(Qt::FramelessWindowHint);
	QString str = str.fromLocal8Bit("±£´æ");
	btn_save = new QPushButton(str,this);
	btn_save->setObjectName(QString::fromUtf8("btn_save"));
	btn_save->setGeometry(QRect(0, 0, 75, 23));
	str = str.fromLocal8Bit("È¡Ïû");
	btn_quit = new QPushButton(str,this);
	btn_quit->setObjectName(QString::fromUtf8("btn_quit"));
	btn_quit->setGeometry(QRect(89, 0, 75, 23));
	setStyleSheet("QPushButton{background-color: rgb(70,130,180);color: white;border-style: outset;border-color:rgb(27, 118, 150);border-width: 1px;border-radius: 5px;}"
		"QPushButton:pressed{background-color: rgb(50, 77, 92);color: white;}"
		"QPushButton:hover{	background-color: rgb(3,110,184);color: white;}");
}

SaveDialog::~SaveDialog()
{
  //  delete ui;
}
