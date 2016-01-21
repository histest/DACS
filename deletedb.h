#ifndef DELETEDB_H
#define DELETEDB_H

#include <QDialog>
#include "ui_deletedb.h"
#include <QFile>
#include <QDir>
#include <QFileDialog>
 #include <QSqlTableModel> 
#include "datedlg.h"
class Deletedb : public QDialog
{
	Q_OBJECT

public:
	Deletedb(QWidget *parent = 0);
	~Deletedb();
	void initUI();
	datedlg* dateDlg; 
private:
	Ui::Deletedb ui;
	QSqlTableModel *model;
	QSqlTableModel *paramodel;
	QSqlTableModel *dictionarymodel;
	private slots:
		void on_deleteButton_clicked();
		void on_deleteallButton_clicked();
		void on_deleteButton_2_clicked();
		void on_previewButton_2_clicked();
		void on_previewButton_3_clicked();
		void on_startdateButton_clicked();
		void on_enddateButton_clicked();
		void on_saveButton_clicked();
		void on_saveButton_2_clicked();
		void on_radioButton_clicked();
		void on_radioButton_2_clicked();
		void on_previewButton_4_clicked();
		void on_deleteButton_3_clicked();
		void on_saveButton_3_clicked();
		void on_deleteAllButton_clicked();
		void on_deleteallButton_2_clicked();
};

#endif // DELETEDB_H
