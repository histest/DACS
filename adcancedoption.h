#ifndef ADCANCEDOPTION_H
#define ADCANCEDOPTION_H

#include <QWidget>
#include "ui_adcancedoption.h"
#include <QRadioButton>
#include <QComboBox>
#include <QLineEdit>
#include <QSpacerItem>
#include <QButtonGroup>
#include "datedlg.h"
class Adcancedoption : public QWidget
{
	Q_OBJECT

public:
	Adcancedoption(QWidget *parent = 0);
	~Adcancedoption();
	void initUI();
	QList<QLineEdit*> lineedits;
	QList<QComboBox*> namecomboxs;
	QList<QComboBox*> siftcomboxs;
	QList<QRadioButton*> andradiosbuttons;
	QList<QRadioButton*> orradiosbuttons;
	QSpacerItem *verticalSpacer;
	QStringList*siftlist;
	QStringList*namelist;
	QStringList paralist;
	QStringList satlist;
	int icount;
	int iFlag;
	bool IssiftPosition;
	bool IssiftID;
	bool Ismultisat;
	int timeID;
	double LongitudeCount;
	double LatitudeCount;
	long  deltadate;
	int deltasecs;
	int startYear;
	int startMonth;
	int startDay;
	QDateTime startdatetime;
	QDateTime enddatetime;
	datedlg* dateDlg; 
	int leap(int year);
	int count_day (int year, int month, int day, int flag);
	int delta_day(int startYear,int startMonth,int startDay,int endYear,int endMonth,int endDay);
signals:
	void getsql(QStringList sqllist);
private:
	Ui::Adcancedoption ui;
	private slots:
		void on_addButton_3_clicked();
		void on_addButton_clicked();
		void on_addButton_2_clicked();
		void on_okButton_clicked();
		void on_cancelButton_clicked();
		void on_delButton_clicked();
		void on_delButton_2_clicked();
		QString getsift(QString strsift);
		void setChoice(bool);
		void setChoice2(bool);
		void siftPositionCheck();
		void siftIDCheck();
		void on_startdateButton_clicked();
		void on_enddateButton_clicked();
};

#endif // ADCANCEDOPTION_H
