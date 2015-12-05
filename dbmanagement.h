#ifndef DBMANAGEMENT_H
#define DBMANAGEMENT_H

#include <QWidget>
#include "ui_dbmanagement.h"
#include <QFile>
#include <QDir>
#include <QFileDialog>
class DBManagement : public QWidget
{
	Q_OBJECT

public:
	DBManagement(QWidget *parent = 0);
	~DBManagement();
private:
	Ui::DBManagement ui;
	private slots:
		void on_deleteButton_clicked();
		void on_openButton_2_clicked();
		void on_backupButton_clicked();
		void on_recoverButton_clicked();
};

#endif // DBMANAGEMENT_H
