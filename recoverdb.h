#ifndef RECOVERDB_H
#define RECOVERDB_H

#include <QDialog>
#include "ui_recoverdb.h"
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

class Recoverdb : public QDialog
{
	Q_OBJECT

public:
	Recoverdb(QWidget *parent = 0);
	~Recoverdb();
	QSqlDatabase *dbrecover;
private:
	Ui::Recoverdb ui;
	private slots:
		void on_recoverButton_clicked();
		void on_openButton_clicked();
		void on_exitButton_clicked();
};

#endif // RECOVERDB_H
