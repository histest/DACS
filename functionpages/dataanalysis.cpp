#include "dataanalysis.h"
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
#include "osgdisplay.h"
#include <iostream>
DataAnalysis::DataAnalysis(QWidget *parent):
	 FTabWidget(parent)
{
	initUI();
}

void DataAnalysis::initUI()
{
	
    CurveDisplay();
    ThreeDemensionDisplay();
	getButtons().at(1)->click();
    getButtons().at(0)->click();
}

void DataAnalysis::CurveDisplay()
{
    QWidget* baseElement = new QWidget;
    QGridLayout* baseLayout = new QGridLayout;
    baseElement->setLayout(baseLayout);
	
	QTextCodec *codec = QTextCodec::codecForName("GBK");
	char *string = "二维分析";
	QString strText = codec->toUnicode(string);

	curve*curvedisplay = new curve;
    addWidget(strText, QString(""), curvedisplay,4);

}

void DataAnalysis::ThreeDemensionDisplay()
{
	osgdisplay* osg = new osgdisplay;
	QString str = str.fromLocal8Bit("三维分析");
	addWidget(str, QString(""), osg,5);
}
