#include "centerwindow.h"
#include "QFramer/fwigglywidget.h"
#include <QHBoxLayout>
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

CenterWindow::CenterWindow(QWidget *parent) :
    FCenterWindow(parent)
{
    initUI();
	connect(uiElements->componentmanagement,SIGNAL(uiElements->componentmanagement->refresh()), this, SLOT(uiElements->productmanagement.initUI()));
}

void CenterWindow::initUI()
{
    uiElements = new UIElement;
    qssBuilder = new QssBuilder;

	DataAnalysis*data = new DataAnalysis;
	SystemManagement*systemmanagement =new SystemManagement;
	QString str = str.fromLocal8Bit("���ݹ���");
    addWidget(str, "QssBuilder", uiElements);

    str = str.fromLocal8Bit("�ڹ����ݷ���");
    addWidget(str,"MathPlot",data);//mathPlot
	str = str.fromLocal8Bit("ϵͳ����");	
    addWidget(str,"Voice", systemmanagement);//qssBuilder

	help*helpdoc =new help;
 	 str = str.fromLocal8Bit("����");
 	addWidget(str,"About", helpdoc);
 
    setAlignment(TopCenter);
}


void CenterWindow::addWiggleWiget()
{
	QString str = str.fromLocal8Bit("�ڹ������ռ������ϵͳ");
    FWigglyWidget* wiggleWidget = new FWigglyWidget(str, getNavgationBar());
    wiggleWidget->move(40, 5);
    wiggleWidget->setFixedSize(400, 60);
    wiggleWidget->setStyleSheet("background-color: transparent;\
                                 font-size: 20px;");
}
