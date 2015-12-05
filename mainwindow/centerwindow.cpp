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
	QString str = str.fromLocal8Bit("数据管理");
    addWidget(str, "QssBuilder", uiElements);

    str = str.fromLocal8Bit("在轨数据分析");
    addWidget(str,"MathPlot",data);//mathPlot
	str = str.fromLocal8Bit("系统管理");	
    addWidget(str,"Voice", systemmanagement);//qssBuilder

	help*helpdoc =new help;
 	 str = str.fromLocal8Bit("帮助");
 	addWidget(str,"About", helpdoc);
 
    setAlignment(TopCenter);
}


void CenterWindow::addWiggleWiget()
{
	QString str = str.fromLocal8Bit("在轨数据收集与分析系统");
    FWigglyWidget* wiggleWidget = new FWigglyWidget(str, getNavgationBar());
    wiggleWidget->move(40, 5);
    wiggleWidget->setFixedSize(400, 60);
    wiggleWidget->setStyleSheet("background-color: transparent;\
                                 font-size: 20px;");
}
