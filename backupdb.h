#ifndef BACKUPDB_H
#define BACKUPDB_H

#include <QDialog>
#include "ui_backupdb.h"
#include <QFile>
#include <QDir>
#include <QFileDialog>
#include<QSqlError>
#include<QSqlDatabase>
#include<QSqlQuery>
#include<QSqlQueryModel>
#include<QSqlTableModel>
#include<QSqlResult>
#include<QSqlRecord>
class Backupdb : public QDialog
{
	Q_OBJECT

public:
	Backupdb(QWidget *parent = 0);
	~Backupdb();
	QSqlDatabase *dbackup;
private:
	Ui::Backupdb ui;
	
	private slots:
		void on_backupButton_clicked();
		void on_openButton_clicked();
		void oraclebackup();
		void connectDB();
		void on_previewButton_clicked();
		void on_previewButton_2_clicked();
		void on_allButton_clicked();
};

#endif // BACKUPDB_H
