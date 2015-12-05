#ifndef USERMANAGEMENT_H
#define USERMANAGEMENT_H

#include <QWidget>
#include "ui_usermanagement.h"

class UserManagement : public QWidget
{
	Q_OBJECT

public:
	UserManagement(QWidget *parent = 0);
	~UserManagement();
	

private:
	Ui::UserManagement ui;
	void  initUI();
	private slots:
		void on_addButton_clicked();
		void on_changeButton_clicked();
		void on_deleteButton_clicked();
};

#endif // USERMANAGEMENT_H
