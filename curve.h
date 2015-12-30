#ifndef CURVE_H
#define CURVE_H

#include <QWidget>
#include "ui_curve.h"
#include "qcustomplot/fcustomplot.h"
#include<QFileDialog>
#include "datedlg.h"
#include "qchartviewer.h"
#include <iostream>
#include "adcancedoption.h"
#include <QtGui>
class curve : public QWidget
{
	Q_OBJECT

public:
	curve(QWidget *parent = 0);
	~curve();
	void initUI();
	//void setupLineStyleChart();
	void setupChart();
	//void setupQuadraticDemo();
	QString satelliteNo[16];
	int satellitecount;
	bool IssiftPosition;
	bool IssiftID;
	bool IsBarchart;
	datedlg* dateDlg; 
	Adcancedoption*option;
	QStringList sqllist;
	QStringList satlist;
	QStringList paralist;
	int timeID;
	int paraID;
	bool IsMultisat;
	int deltadate;
	int deltasecs;
	int startYear;
	int startMonth;
	int startDay;
	int endYear;
	int endMonth;
	int endDay;
	QDateTime startdatetime;
	QDateTime enddatetime;
	QListView *namelistview; 
	QStringListModel *model; 
	QString strzone;
public slots:
	void refresh();
private:
	Ui::curve ui;
	FCustomPlot *plot1;
	QChartViewer *viewer;
	XYChart *chart;
	QString satellites[16];
	bool IsStop;
	public slots:
		void siftPositionCheck();
		void on_outButton_clicked();
		void on_outputButton_clicked();
		void siftIDCheck();
		void onChanged(int);
		void Changed();
		void on_pushButton_clicked();
		void on_startdateButton_clicked();
		void on_enddateButton_clicked();
		void on_refreshButton_clicked();
		void on_advancedButton_clicked();
		void setsql(QStringList);
		void setCompleter(const QString &text); 
		void completeText(const QModelIndex &index);
};

#endif // CURVE_H
