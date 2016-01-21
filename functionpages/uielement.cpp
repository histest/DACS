
#include "uielement.h"
#include "QFramer/ftabwidget.h"
#include "QFramer/futil.h"
#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QPushButton>
#include <QRadioButton>
#include <QCheckBox>
#include <QTreeWidget>
#include <QTabWidget>
#include <QSplitter>
#include <QHeaderView>
#include <QProgressBar>
#include <QTimer>
#include <QGraphicsScene>
#include <QTimer>
#include <QApplication>
#include <QGridLayout>
#include <QWidget>
#include <QtGui/QIcon>
#include <QtCore/QTextCodec>
#include <QtGui/QMenuBar>
#include <QtGui/QMenu>
#include <QtGui/QFileDialog>
#include <QtGui/QColorDialog>
#include <osgQt/GraphicsWindowQt>

#include <iostream>

UIElement::UIElement(QWidget *parent) :
    FTabWidget(parent)
{
    initUI();
	connect(componentmanagement,SIGNAL(componentmanagement->refresh()), productmanagement, SLOT(productmanagement->initUI()));
}

void UIElement::initUI()
{
	
    ProductInfoManagement();
    SourceInfoManagement();
    ComponentInfoManagement();
	DictionaryInfoManagement();
    getButtons().at(1)->click();
	 getButtons().at(0)->click();
}

void UIElement::ProductInfoManagement()
{
	QTextCodec *codec = QTextCodec::codecForName("GBK");
	char *string = "���ݵ��뵼��";
	QString strText = codec->toUnicode(string);
	productmanagement = new ProductManagement;
    addWidget(strText, QString(""), productmanagement,1);

}

void UIElement::SourceInfoManagement()
{
	QString str = str.fromLocal8Bit("ԭʼ���ݹ���");
	sourcedatamanagement= new SourceDataManagement;
    addWidget(str, QString(""), sourcedatamanagement,2);
}


void UIElement::ComponentInfoManagement()
{
	QString str = str.fromLocal8Bit("оƬ����");
	componentmanagement = new ComponentManagement;
    addWidget(str, QString(""), componentmanagement,3);
}
void UIElement::DictionaryInfoManagement()
{
	QString str = str.fromLocal8Bit("�����ֵ�");
	dictionaryManagement = new DictionaryManagement;
	addWidget(str, QString(""), dictionaryManagement,8);
}
