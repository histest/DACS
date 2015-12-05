#ifndef OSGDISPLAY_H
#define OSGDISPLAY_H

#include <QWidget>
#include "ui_osgdisplay.h"
#include <QtGui/QAction>
#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>
#include <osgViewer/api/win32/GraphicsWindowWin32>
#include <osgGA/TrackballManipulator>
#include <osgGA/KeySwitchMatrixManipulator>
#include <osgDB/DatabasePager>
#include <osgDB/Registry>
#include <osgDB/ReadFile>
#include <osgUtil/Optimizer>
#include <string>
#include <QDesktopServices>
#include "viewerwidget.h"
#include "odbcexcel.h"
#include "datedlg.h"
#include "adcancedoption.h"
#include <QtGui>
class osgdisplay : public QWidget
{
	Q_OBJECT

public:
	osgdisplay(QWidget *parent = 0);
	~osgdisplay();
	osg::Group*InitSceneGraph(void);
	void initUI();
	ViewerWidget* viewWidget;
	QString satelliteNo[16];
	int satellitecount;
	bool IssiftPosition;
	bool IssiftID;
	bool Isblank;
	datedlg* dateDlg; 
	double LongitudeCount;
	double LatitudeCount;
	Adcancedoption*option;
	QStringList sqllist;
	QStringList satlist;
	QStringList paralist;
	int timeID;
	int paraID;
	bool IsMultisat;
	QListView *namelistview; 
	QStringListModel *model; 
public slots:
	void refresh();
private:
	Ui::osgdisplay ui;
	bool IsStop;
	public slots:
		void DisplayOSG();
		void multiSatcheck();
		void siftPositionCheck();
		void siftIDCheck();
		void onChanged();
		void on_outButton_clicked();
		void on_outputButton_clicked();
		void on_startdateButton_clicked();
		void on_enddateButton_clicked();
		void on_refreshButton_clicked();
		void on_refreshButton_2_clicked();
		void comboBox_editTextChanged();
		void on_advancedButton_clicked();
		void setsql(QStringList);
		void setCompleter(const QString &text); 
		void completeText(const QModelIndex &index);
};

#endif // OSGDISPLAY_H
