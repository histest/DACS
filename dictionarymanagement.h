#ifndef DICTIONARYMANAGEMENT_H
#define DICTIONARYMANAGEMENT_H

#include <QWidget>
#include "ui_dictionarymanagement.h"
#include<QFileDialog>
#include "odbcexcel.h"
#include <QtGui>
class DictionaryManagement : public QWidget
{
	Q_OBJECT

public:
	DictionaryManagement(QWidget *parent = 0);
	~DictionaryManagement();
	void initUI();
	QListView *namelistview; 
	QStringListModel *model; 
private:
	Ui::DictionaryManagement ui;
	private slots:
		void on_openButton_clicked();
		void on_inputButton_clicked();
		void on_outButton_clicked();
		void on_queryButton_clicked();
		void on_previewButton_clicked();
		void on_previewButton_2_clicked();
		void setCompleter(const QString &text); 
		void completeText(const QModelIndex &index);
};

#endif // DICTIONARYMANAGEMENT_H
