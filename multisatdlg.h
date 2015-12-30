#ifndef MULTISATDLG_H
#define MULTISATDLG_H

#include <QDialog>
#include "ui_multisatdlg.h"
#include <QComboBox>
#include <QLineEdit>
#include <QSpacerItem>
#include <QPushButton>
#include <QLabel>
#include <QSpacerItem>
#include <QButtonGroup>
#include <QtGui>
#include "datedlg.h"
class MultiSatDlg : public QDialog
{
	Q_OBJECT

public:
	MultiSatDlg(QWidget *parent = 0);
	~MultiSatDlg();
	void initUI();
	QString satellites[16];
	int satellitecount;
	QStringList satlist;
	QList<QLineEdit*> startlineedits;
	QList<QLineEdit*> endlineedits;
	QList<QPushButton*> startButtons;
	QList<QPushButton*> endButtons;
	QList<QComboBox*> satcomboxs;
	int iCount;
	QSpacerItem *verticalSpacer;
	datedlg* dateDlg; 
	QStringList sqllist;
private:
	Ui::MultiSatDlg* ui;
	
	private slots:
		void on_okButton_clicked();
		void on_cancelButton_clicked();
		void on_addButton_clicked();
		void on_addSatButton_clicked();
		void on_delButton_clicked();
		void setStartTime();
		void setEndTime();
};

#endif // MULTISATDLG_H
