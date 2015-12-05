#ifndef DELETEUSER_H
#define DELETEUSER_H

#include <QDialog>
#include "ui_deleteuser.h"

class Deleteuser : public QDialog
{
	Q_OBJECT

public:
	Deleteuser(QWidget *parent = 0);
	~Deleteuser();

private:
	Ui::Deleteuser ui;
	void  initUI();
	private slots:
		void on_okButton_clicked();
		void on_clearButton_clicked();
		void on_cancelButton_clicked();
};

#endif // DELETEUSER_H
