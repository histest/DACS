#ifndef ADVANCEDSEARCH_H
#define ADVANCEDSEARCH_H

#include <QWidget>
#include "ui_advancedsearch.h"
#include <QRadioButton>
#include <QComboBox>
#include <QLineEdit>
#include <QSpacerItem>
#include <QButtonGroup>
#include <QtGui>
class QTextDocument;
class QTextEdit;
class QEvent;
class QObject;
#include "datedlg.h"
class Advancedsearch : public QWidget
{
	Q_OBJECT

public:
	Advancedsearch(QWidget *parent = 0);
	~Advancedsearch();
	void initUI();
	 QList<QLineEdit*> lineedits;
	 QList<QComboBox*> namecomboxs;
	  QList<QComboBox*> siftcomboxs;
	 QList<QRadioButton*> andradiosbuttons;
	 QList<QRadioButton*> orradiosbuttons;
	 QSpacerItem *verticalSpacer;
	QStringList*siftlist;
	QStringList*namelist;
	QStringList satlist;
	QStringList sqllist;
	 int icount;
	 int iFlag;
	 void setFlag(int);
	 QListView *namelistview; 
	 QStringListModel *model; 
	 void keyPressEvent(QKeyEvent *e);
	datedlg* dateDlg; 
signals:
	void getsql(QString strsql);

private:
	Ui::Advancedsearch ui;
	private slots:
		void on_addButton_clicked();
		void on_okButton_clicked();
		void on_cancelButton_clicked();
		void multiSatcheck();
		QString getsift(QString strsift);
		void setCompleter(const QString &text); 
		void completeText(const QModelIndex &index);
		void on_startdateButton_clicked();
		void on_enddateButton_clicked();
};

#endif // ADVANCEDSEARCH_H
